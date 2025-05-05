# Minimum required version of CMake.
# JUCE recommends at least CMake 3.15 for modern features and compatibility.
cmake_minimum_required(VERSION 3.15)

# Set the executable name as a variable, which we will use elsewhere in this
# CMakeLists.txt file so we don't have to type it multiple times.
set(EXEC_NAME Walendowski_Freeverb)

# Set the project name and version.
# Customize the project name to match the name of your own project.
project(${EXEC_NAME} VERSION 1.0.0)

# Set the C++ standard to use.
# JUCE requires at least C++14, but it's a good idea to use C++17 or later
# for modern language features.
set(CMAKE_CXX_STANDARD 17)

# This path needs to point to the root of the JUCE library. Make sure it's correctly
# set up before running CMake. This is currently configured to look in the parent
# directory above this project, but you can update to point to a different location.
# The second argument to this function sets up another build workspace for the JUCE
# library itself. You shouldn't need to modify that part.
add_subdirectory("JUCE" "${CMAKE_BINARY_DIR}/JUCE")

# create a JUCE plugin
# There are many other options, defined here:
# https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md
juce_add_plugin(${EXEC_NAME}
    FORMATS AU VST3 Standalone
    COMPANY_NAME "Walendowski"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

# This is where you list your source files.
# As your project grows, you will add more .cpp files here, or even create subdirectories
# for better organization (e.g., Source/MyClass.cpp).
target_sources(${EXEC_NAME}
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Link the JUCE modules that are necessary for the project.
# juce::juce_core is the fundamental JUCE module that almost all JUCE projects need.
# Additional modules can be linked here as needed. For example:
# - juce_data_structures: Higher-level data structures like trees, XML, and value trees.
# - juce_events: Classes for event handling (e.g., messaging between threads).
# - juce_gui_basics: Basic GUI components (buttons, sliders, windows, etc.).
#
# You only need to link the modules that your project will use. The full list of
# available modules is in the JUCE documentation: https://docs.juce.com/master/modules.html
target_link_libraries(${EXEC_NAME}
    PRIVATE
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_dsp

)

# Generate JuceHeader.h in the project's source directory that includes all the
# modules used by your project. With this, your source files can just have
# `#include <JuceHeader.h>`. If you don't do this, you will need to include
# individual modules in your source files, for example
# `#include <juce_core/juce_core.h>`
juce_generate_juce_header(${EXEC_NAME})

# Additional Notes for Students:
# - If you add new source files or directories, make sure to update add_executable.
# - If you're using other JUCE modules, you'll need to link them under target_link_libraries.
# - Remember to keep your project directory organized as it grows (e.g., creating separate
#   folders for source code, headers, resources, etc.).
