cmake_minimum_required(VERSION 3.22)
project(BREWS_DISTORTION VERSION 0.0.1)
# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
add_subdirectory(modules/JUCE)                    # If you've put JUCE in a subdirectory called JUCE
juce_add_plugin(BrewsDist
    VERSION 0.1                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE SOME               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Brdi                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Brew's Distortion")        # The name of the final executable, which can differ from the target name

juce_generate_juce_header(BrewsDist)

target_sources(BrewsDist
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
	src/DSP/Distortion.cpp
	src/UI/RotaryKnob.cpp
	src/UI/BrewsLookAndFeel.cpp
	)
target_compile_definitions(BrewsDist
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)
 juce_add_binary_data(BrewsDistData SOURCES 
                      res/Banner.png
                      res/CoverImage.png
                      res/Raleway-VariableFont_wght.ttf
                                             
)
target_link_libraries(BrewsDist
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        BrewsDistData
        juce::juce_audio_utils
	juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
