#include "PluginProcessor.h"
#include "PluginEditor.h"

SteinexTransitFXEditor::SteinexTransitFXEditor(SteinexTransitFXProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
    setLookAndFeel(&steinexLNF);
    setSize(920, 540);

    setupKnob(macroKnob, "TRANSITION");
    setupKnob(backverbKnob, "BACKVERB");
    setupKnob(chopKnob, "CHOPPER");
    setupKnob(glitchKnob, "GLITCH");

    stutterButton.setButtonText("STUTTER TRIGGER");
    stutterButton.setClickingTogglesState(true);
    stutterButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF00F0FF));
    stutterButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    addAndMakeVisible(stutterButton);

    macroAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "TRANSITION_MACRO", macroKnob);
    backverbAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "BACKVERB_MIX", backverbKnob);
    chopAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "CHOP_DEPTH", chopKnob);
    glitchAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "GLITCH_AMT", glitchKnob);
    stutterAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, "STUTTER_ON", stutterButton);
}

SteinexTransitFXEditor::~SteinexTransitFXEditor() {
    setLookAndFeel(nullptr);
}

void SteinexTransitFXEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xFF0D0F12));

    g.setColour(juce::Colour(0xFF14181F));
    g.fillRect(0, 0, getWidth(), 56);
    g.setColour(juce::Colour(0xFF252B36));
    g.drawLine(0, 56, static_cast<float>(getWidth()), 56, 1.5f);

    g.setColour(juce::Colour(0xFF00F0FF));
    g.setFont(juce::FontOptions("Segoe UI", 22.0f, juce::Font::bold));
    g.drawText("STEINEX", 24, 12, 140, 32, juce::Justification::left);

    g.setColour(juce::Colour(0xFFE6EAF0));
    g.setFont(juce::FontOptions("Segoe UI", 18.0f, juce::Font::plain));
    g.drawText("|  TRANSIT FX", 135, 13, 200, 32, juce::Justification::left);

    g.setColour(juce::Colour(0xFF161A20));
    g.fillRoundedRectangle(24.0f, 310.0f, 872.0f, 205.0f, 10.0f);
    g.setColour(juce::Colour(0xFF262D38));
    g.drawRoundedRectangle(24.0f, 310.0f, 872.0f, 205.0f, 10.0f, 1.0f);
}

void SteinexTransitFXEditor::resized() {
    macroKnob.setBounds(370, 80, 180, 180);
    stutterButton.setBounds(55, 360, 170, 90);
    backverbKnob.setBounds(275, 340, 130, 130);
    chopKnob.setBounds(485, 340, 130, 130);
    glitchKnob.setBounds(695, 340, 130, 130);
}

void SteinexTransitFXEditor::setupKnob(juce::Slider& slider, const juce::String& name) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setName(name);
    addAndMakeVisible(slider);
}