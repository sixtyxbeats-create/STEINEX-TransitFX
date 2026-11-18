cmake_minimum_required(VERSION 3.22)
project(STEINEX_TRANSIT_FX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.3
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SteinexTransitFX
    COMPANY_NAME "STEINEX"
    PLUGIN_MANUFACTURER_CODE Stnx
    PLUGIN_CODE TrFx
    FORMATS VST3
    PRODUCT_NAME "TRANSIT FX"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_COPY_DIR "$ENV{COMMONPROGRAMFILES}/VST3"
)

target_sources(SteinexTransitFX PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_compile_definitions(SteinexTransitFX PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SteinexTransitFX PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)