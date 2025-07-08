# DESIGN.md

## Overview

This document provides a technical overview of the implementation and design decisions behind the JUCE-based stereo reverb plugin, **SzymVerb**. The project is a flexible and lightweight reverb effect featuring a bank of parallel comb filters, a series of allpass filters, adjustable dry/wet mix, damping, and a freeze button.

## Architecture

The plugin follows JUCE’s standard `AudioProcessor` / `AudioProcessorEditor` structure. All DSP occurs within the `processBlock()` method, while the UI is handled in the `PluginEditor` class. Parameters are managed using `AudioProcessorValueTreeState`.

### Audio Path

The signal chain is as follows:

1. **Input Audio**
2. **Parallel Comb Filters** (8 per channel)
3. **Series Allpass Filters** (4 per channel)
4. **Wet/Dry Mixing**
5. **Output Audio**

### Parameters

Parameters are exposed via the ValueTree and are connected to GUI components with JUCE's Attachment classes:

* `dryMix`: dry signal level
* `wetMix`: wet signal level
* `feedback`: controls feedback of comb filters
* `damp`: controls the low-pass filter cutoff inside combs
* `freeze`: toggle that locks feedback to 1.0 and stops input from entering the reverb

All parameters are registered within the processor’s constructor using `AudioProcessorValueTreeState`, and attached to UI elements via `SliderAttachment` and `ButtonAttachment` wrappers.

## Key Implementation Details

### Comb Filter Bank

Each channel contains 8 comb filters in parallel. These filters are implemented using circular buffers. Each comb has its own delay line (different lengths for L and R) and feedback path. To simulate high-frequency damping in the feedback loop, a built-in JUCE IIR low-pass filter is applied before mixing back into the buffer.

```cpp
float delayed = combLine[readIdx];
delayed = combFilters[ch][k].processSample(delayed); // damping
float y = delayed * combFeedback;
```

### Allpass Filters

Allpasses are chained in series and applied after the comb sum. Each one uses a short delay and a feedback gain of around 0.5. The transfer function used is:

```
H(z) = (g + z^(-d)) / (1 + g·z^(-d))
```

which yields the time-domain formula:

```cpp
float y = -g * x[n] + x[n-d] + g * y[n-d];
```

These are implemented with circular buffers as well, storing delayed states per channel. 

### Freeze Logic

When `freeze` is engaged, two things happen:

* Input is blocked from entering the combs.
* Feedback is forced to 1.0 to sustain the current reverb state indefinitely.

This is handled with:

```cpp
if (frozen) {
    input = 0.0f;
    combFeedback = 1.0f;
}
```

### Stereo Spread

Comb delay values are subtly different between left and right channels, creating a more spacious image. Each channel uses its own delay set.

## GUI Design

The GUI is built using JUCE’s stock components. Sliders are evenly spaced using manual layout in `resized()`. The `freeze` button is a stylized text button that changes color when active.


## Possible Extensions

* Pitch shifting of frozen audio using resampling or delay line modulation
* Smoothed parameter changes for more natural automation
* Preset system via JUCE's `ValueTree` serialization
* Custom GUI with vector graphics or imported assets


