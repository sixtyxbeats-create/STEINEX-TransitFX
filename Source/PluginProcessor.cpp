#include "PluginProcessor.h"
#include "PluginEditor.h"

SteinexTransitFXProcessor::SteinexTransitFXProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "STEINEX_PARAMS", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout SteinexTransitFXProcessor::createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"TRANSITION_MACRO", 1}, "Transition Macro", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"STUTTER_ON", 1}, "Stutter Trigger", false));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"STUTTER_DIV", 1}, "Stutter Division",
        juce::StringArray{"1/4", "1/8", "1/16", "1/32", "1/64"}, 2));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BACKVERB_MIX", 1}, "Backverb Swell", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"CHOP_DEPTH", 1}, "Rhythmic Chop", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"GLITCH_AMT", 1}, "Glitch Crush", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"DRY_WET", 1}, "Master Mix", 0.0f, 1.0f, 1.0f));

    return { params.begin(), params.end() };
}

void SteinexTransitFXProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock),
                                 static_cast<juce::uint32>(getTotalNumOutputChannels()) };
    dspChain.prepare(spec);
}

void SteinexTransitFXProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    double bpm = 128.0;
    double ppq = 0.0;
    if (auto* playHead = getPlayHead()) {
        if (auto pos = playHead->getPosition()) {
            if (pos->getBpm().hasValue()) bpm = *pos->getBpm();
            if (pos->getPpqPosition().hasValue()) ppq = *pos->getPpqPosition();
        }
    }

    bool midiStutter = false;
    for (const auto metadata : midiMessages) {
        if (metadata.getMessage().isNoteOn()) midiStutter = true;
    }

    const float macro = apvts.getRawParameterValue("TRANSITION_MACRO")->load();
    const bool stutterOn = (apvts.getRawParameterValue("STUTTER_ON")->load() > 0.5f) || midiStutter;
    const int divIndex = static_cast<int>(apvts.getRawParameterValue("STUTTER_DIV")->load());
    const int divValues[] = { 1, 2, 4, 8, 16 };
    const float backverb = apvts.getRawParameterValue("BACKVERB_MIX")->load();
    const float chop = apvts.getRawParameterValue("CHOP_DEPTH")->load();
    const float glitch = apvts.getRawParameterValue("GLITCH_AMT")->load();

    dspChain.process(buffer, bpm, ppq, macro, stutterOn, divValues[divIndex], backverb, chop, glitch);
}

void SteinexTransitFXProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SteinexTransitFXProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState && xmlState->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorEditor* SteinexTransitFXProcessor::createEditor() {
    return new SteinexTransitFXEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new SteinexTransitFXProcessor();
}