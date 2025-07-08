# SzymVerb Plugin

*A JUCE-Based Stereo Reverb with Damping and Freeze Mode*

## Overview

**SzymVerb** is a stereo audio plugin built using the JUCE framework. It implements a classic Schroeder-style reverb algorithm using 8 parallel comb filters followed by 4 serial allpass filters per channel.

The plugin includes controls for:

* **Dry/Wet mix**
* **Feedback (room size)**
* **High-frequency Damping**
* **Freeze Mode**

Freeze mode allows users to infinitely sustain the reverb tail while suppressing new input, creating an evolving ambient soundscape.
The plugin is designed for real-time performance and works as an **AU** and **VST3** effect in compatible digital audio workstations.

---

## Build Instructions

### Requirements

* [JUCE Framework](https://juce.com/)
* CMake 
* A modern C++ compiler (GCC, Clang, or MSVC)


### Compilation Steps


Clone the repository and make sure the JUCE framework is available in that directory, or change the cmake list to navigate to the ```JUCE directory``` location. Execute the following commands in the ```jucereverb``` directory:

```
# Configure the build directory
cmake -B build 

# Compile the plugin
cmake --build build
```

---

## UI & Parameters

The plugin interface includes four vertical sliders and one button:

### Sliders

* **Dry** – Controls level of unprocessed signal.
* **Wet** – Controls level of reverb signal.
* **Feedback** – Controls length of the reverb tail via comb feedback.
* **Damping** – Applies a lowpass filter inside combs to roll off high frequencies (2000–20000 Hz).

### Freeze Button

* Stops incoming audio from entering the reverb path.
* Locks feedback at 1.0 for infinite sustain.
* Changes its appearance when toggled.

---

## Processing Details

* **Stereo Reverb**: Uses distinct delay lengths for left/right comb filters to increase spatial depth.
* **Comb Filtering**: Implemented using circular buffers per channel and filtered via JUCE's built-in `dsp::IIR::Filter`.
* **Allpass Filtering**: Four filters applied in series to smear transients and simulate diffusion.
* **Damping**: Implemented via lowpass filtering inside the comb feedback loop.
* **Freeze Mode**:

  * Internally sets `feedback = 1.0f`.
  * Zeroes input before reaching the reverb engine.
  * Recirculates buffer content to hold a snapshot in time.

---

## Usage Tips

* Lower **damping** for darker, more ambient textures.
* Set **wet** high and **dry** low for immersive environments.
* Use **freeze** creatively during transitions, pauses, or to generate pads.
* Combine with pitch shifters or modulation for even more lushness.

---

## Author

**Szymon Walendowski**
DSP, GUI, and plugin implementation.

---

