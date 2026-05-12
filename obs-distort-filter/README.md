# OBS Glitch / Distortion Filter

A video filter plugin for OBS Studio that applies a distorted, pixelated, glitchy effect to any video source (webcam, screen capture, media source, etc.).

## Features

- **Pixelation** - Blocky low-res look
- **RGB Channel Shift** - Chromatic aberration that shifts red/blue channels
- **Chroma Bleed** - Color bleeding across edges like bad video compression
- **Wave Distortion** - Sine-wave warping across the image
- **Scanlines** - Retro CRT-style scanlines
- **Interlace** - Interlacing artifacts that darken alternate lines
- **Glitch Blocks** - Random horizontal tearing and color inversion
- **Packet Loss** - Macroblock corruption that smears, inverts, or replaces blocks
- **Freeze Frames** - Randomly freezes the distortion state to simulate a stuck decoder
- **Digital Noise** - Static/noise overlay based on glitch intensity
- **Flicker** - Random brightness instability like a failing power supply
- **Ghost** - Frame echo / double image trailing behind
- **Horizontal Hold** - Old TV-style horizontal jitter and instability
- **Posterize** - Reduced color bit depth for compression artifacts
- **Rolling Bar** - Dark interference bar that rolls down the screen
- **Per-Effect Randomizers** - Make each effect jitter unpredictably instead of repeating smoothly
- **Animated** - Everything moves and shifts over time automatically

## Default Look

The defaults are tuned to give a "camera is trying to work but failing" vibe:
- Medium pixelation (8px blocks) with slight randomization
- Subtle RGB shift and chroma bleed
- Slow wave distortion with moderate randomization
- Visible scanlines and interlacing
- Occasional glitch tearing, noise, and packet loss
- Occasional freeze frames and flicker
- Rolling bar and horizontal hold for analog decay feel

## Installation (Pre-built)

If you have a pre-built `.dll` and `.effect` file:

1. Copy `obs-distort-filter.dll` to `C:\Program Files\obs-studio\obs-plugins\64bit\`
2. Copy `distort_filter.effect` to `C:\Program Files\obs-studio\data\obs-plugins\obs-distort-filter\`
3. Restart OBS Studio

## Building on Windows 11

### Prerequisites

1. **Visual Studio 2022** (Community edition is free)
   - Install workload: **Desktop development with C++**
2. **CMake** 3.16+ (install from https://cmake.org/download/)
3. **OBS Studio Source + Development Libraries**
   - Option A: Clone `https://github.com/obsproject/obs-studio.git`
   - Option B: Download the OBS Windows deps from the OBS build guide

### Build Steps (Build as part of OBS)

1. Clone the OBS Studio source:
   ```powershell
   git clone --recursive https://github.com/obsproject/obs-studio.git
   cd obs-studio
   ```

2. Copy this plugin into the OBS plugins folder:
   ```powershell
   Copy-Item -Recurse ..\obs-distort-filter plugins\obs-distort-filter
   ```

3. Add the plugin to `plugins/CMakeLists.txt` in the OBS source. Find the section that lists plugins and add:
   ```cmake
   add_subdirectory(obs-distort-filter)
   ```

4. Create a build directory and configure with CMake:
   ```powershell
   mkdir build && cd build
   cmake .. -A x64 `
     -DENABLE_BROWSER=OFF `
     -DENABLE_VLC=OFF `
     -DENABLE_AJA=OFF `
     -DENABLE_SCRIPTING=OFF
   ```

5. Build:
   ```powershell
   cmake --build . --config Release --target obs-distort-filter
   ```

6. The output will be at:
   - `build\plugins\obs-distort-filter\Release\obs-distort-filter.dll`
   - `plugins\obs-distort-filter\data\distort_filter.effect`

### Build Steps (Standalone with OBS Development Libraries)

If you have the OBS development libraries and headers:

```powershell
mkdir build && cd build
cmake .. -A x64 -DCMAKE_PREFIX_PATH="C:/path/to/obs-studio/libobs"
cmake --build . --config Release
```

## Usage in OBS

1. Add a **Video Capture Device** source (your webcam) or any video source.
2. Right-click the source → **Filters**.
3. Under **Effect Filters**, click the **+** button.
4. Select **Glitch / Distortion Filter**.
5. Adjust the sliders:
   - **Pixel Size**: Bigger = blockier
   - **RGB Shift**: How much red/blue channels separate
   - **Chroma Bleed**: Color bleeding intensity across edges
   - **Wave Amount**: How warped the image gets
   - **Wave Speed**: How fast the wave animates
   - **Scanline Intensity**: Darkness of CRT lines
   - **Interlace**: Darken every other scanline
   - **Glitch Intensity**: Frequency and strength of random glitches
   - **Packet Loss**: Macroblock corruption (smeared, inverted, or noisy blocks)
   - **Freeze Frames**: How often the image freezes up (0 = never, 1 = constantly)
   - **Freeze Duration**: How long each freeze lasts
   - **Randomize Pixel**: How much pixel size jitters frame-to-frame
   - **Randomize Wave**: How much wave distortion jitters unpredictably
   - **Randomize Glitch**: How much glitch intensity bursts randomly
   - **Flicker**: Random brightness pulses
   - **Ghost**: Faint trailing echo image
   - **Horizontal Hold**: Horizontal jitter like an old TV
   - **Posterize**: Color banding / reduced bit depth
   - **Rolling Bar**: Slow dark bar rolling down the screen

## File Structure

```
obs-distort-filter/
├── CMakeLists.txt                  # CMake build file
├── README.md                       # This file
├── obs-distort-filter.c            # Main plugin code
├── cmake/
│   └── windows/
│       └── obs-module.rc.in        # Windows DLL metadata
└── data/
    └── distort_filter.effect       # GPU shader (HLSL/GLSL)
```

## Compatibility

- **OS**: Windows 10/11 (64-bit)
- **OBS Studio**: 29.x, 30.x, 31.x
- **GPU**: DirectX 11 or OpenGL capable

## Troubleshooting

- **Filter does not appear**: Make sure both the `.dll` and `.effect` file are in the correct directories and match your OBS architecture (64-bit).
- **Black screen**: The shader may have failed to compile. Check `Help` → `Log Files` → `View Current Log` in OBS for shader compile errors.
- **Performance issues**: Reduce pixel size or disable scanlines to lower GPU load.

## License

MIT License - Do whatever you want with it.
