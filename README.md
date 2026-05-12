# OBS Glitch / Distortion Filter

A video filter plugin for OBS Studio that applies a distorted, pixelated, glitchy effect to any video source (webcam, screen capture, media source, etc.). Designed to look like a failing camera or bad video transmission — everything still works, but it looks broken.

## Features

| Feature | Description |
|---|---|
| **Pixelation** | Blocky low-res look. Size is adjustable and can be randomized frame-to-frame |
| **RGB Channel Shift** | Chromatic aberration that shifts red/blue channels apart |
| **Chroma Bleed** | Extra color bleeding across edges, like bad video compression |
| **Wave Distortion** | Sine-wave warping across the image with adjustable speed |
| **Scanlines** | Retro CRT-style scanlines |
| **Interlace** | Interlacing artifacts that darken alternate lines |
| **Glitch Blocks** | Random horizontal tearing, color inversion, and digital noise |
| **Packet Loss** | 16x16 macroblock corruption — blocks smear, invert, turn solid color, or fill with noise |
| **Freeze Frames** | Randomly freezes the distortion animation to simulate a stuck decoder |
| **Flicker** | Random brightness pulses like a failing power supply |
| **Ghost** | Faint trailing echo image offset horizontally |
| **Horizontal Hold** | Old TV-style horizontal jitter and instability |
| **Posterize** | Reduced color bit depth for compression banding artifacts |
| **Rolling Bar** | Slow dark interference bar that rolls down the screen |
| **Per-Effect Randomizers** | Make pixel size, wave, and glitch jitter unpredictably instead of repeating smoothly |
| **Vignette** | Darkened edges to focus attention on the center distortion |

## Installation

### Method 1: Automatic Installer (Recommended)

1. Download the latest release zip (`obs-distort-filter-release.zip`)
2. Extract it anywhere
3. Right-click **`INSTALL.ps1`** → **Run with PowerShell**
   - The script auto-detects your OBS installation path
   - If it can't find OBS, it'll ask you to type the path
4. Restart OBS Studio

### Method 2: Manual Copy

1. Close OBS Studio completely
2. Copy `obs-plugins/64bit/obs-distort-filter.dll` to your OBS `obs-plugins/64bit/` folder
3. Copy `data/obs-plugins/obs-distort-filter/distort_filter.effect` to your OBS `data/obs-plugins/obs-distort-filter/` folder
4. Copy `data/obs-plugins/obs-distort-filter/locale/en-US.ini` to your OBS `data/obs-plugins/obs-distort-filter/locale/` folder
5. Restart OBS Studio

### Default OBS Paths

- **Standard install**: `C:\Program Files\obs-studio\`
- **Steam install**: `C:\Program Files (x86)\Steam\steamapps\common\OBS Studio\`
- **Portable**: wherever you extracted the portable zip

## Usage

1. Add a **Video Capture Device** source (your webcam) or any video source
2. Right-click the source → **Filters**
3. Under **Effect Filters**, click the **+** button
4. Select **Glitch / Distortion Filter**
5. Adjust sliders live while watching the preview:

| Slider | Range | Default | What it does |
|---|---|---|---|
| **Pixel Size** | 1–64 | 8 | Blockiness of pixelation. Higher = bigger blocks |
| **RGB Shift** | 0.0–0.1 | 0.02 | How much red/blue channels separate |
| **Chroma Bleed** | 0.0–1.0 | 0.15 | Color bleeding across edges |
| **Wave Amount** | 0.0–0.05 | 0.01 | How warped the image gets |
| **Wave Speed** | 0.0–20.0 | 3.0 | How fast the wave animates |
| **Scanline Intensity** | 0.0–1.0 | 0.3 | Darkness of CRT-style lines |
| **Interlace** | 0.0–1.0 | 0.2 | Darken every other scanline |
| **Glitch Intensity** | 0.0–1.0 | 0.3 | Frequency and strength of random glitches |
| **Packet Loss** | 0.0–1.0 | 0.2 | Macroblock corruption frequency |
| **Freeze Frames** | 0.0–1.0 | 0.15 | How often image freezes (0 = never, 1 = constantly) |
| **Freeze Duration** | 0.0–0.5 | 0.08 | How long each freeze lasts |
| **Randomize Pixel** | 0.0–1.0 | 0.3 | How much pixel size jitters frame-to-frame |
| **Randomize Wave** | 0.0–1.0 | 0.4 | How much wave distortion jitters unpredictably |
| **Randomize Glitch** | 0.0–1.0 | 0.5 | How much glitch intensity bursts randomly |
| **Flicker** | 0.0–1.0 | 0.15 | Random brightness pulses |
| **Ghost** | 0.0–1.0 | 0.1 | Faint trailing echo image |
| **Horizontal Hold** | 0.0–1.0 | 0.1 | Horizontal jitter like an old TV |
| **Posterize** | 0.0–1.0 | 0.0 | Color banding / reduced bit depth |
| **Rolling Bar** | 0.0–1.0 | 0.1 | Slow dark bar rolling down the screen |

## Default Look

The defaults are tuned to give a **"camera is trying to work but failing"** vibe:
- Medium pixelation (8px blocks) with slight randomization
- Subtle RGB shift and chroma bleed
- Slow wave distortion with moderate randomization
- Visible scanlines and interlacing
- Occasional glitch tearing, noise, and packet loss
- Occasional freeze frames and flicker
- Rolling bar and horizontal hold for analog decay feel

## Building from Source

### Prerequisites

- **Visual Studio 2022** with **Desktop development with C++** workload
- **CMake** 3.16+
- **Git**

### Build Steps

1. Clone the OBS Studio source:
   ```powershell
   git clone --recursive https://github.com/obsproject/obs-studio.git
   cd obs-studio
   ```

2. Copy this plugin into the OBS plugins folder:
   ```powershell
   Copy-Item -Recurse ..\obs-distort-filter plugins\obs-distort-filter
   ```

3. Add the plugin to `plugins/CMakeLists.txt`. Find the section that lists plugins and add:
   ```cmake
   add_obs_plugin(obs-distort-filter PLATFORMS WINDOWS)
   ```

4. Create a build directory and configure with CMake:
   ```powershell
   mkdir build && cd build
   cmake .. -A x64 -DENABLE_BROWSER=OFF -DENABLE_UI=OFF -DENABLE_SCRIPTING=OFF -DENABLE_HEVC=OFF -DENABLE_PLUGINS=ON
   ```

5. Build:
   ```powershell
   cmake --build . --config Release --target obs-distort-filter
   ```

6. The output will be at:
   - `build\plugins\obs-distort-filter\Release\obs-distort-filter.dll`
   - `plugins\obs-distort-filter\data\distort_filter.effect`

## File Structure

```
obs-distort-filter/
├── CMakeLists.txt                  # CMake build file
├── README.md                       # This file
├── obs-distort-filter.c            # Main plugin C code
├── cmake/
│   └── windows/
│       └── obs-module.rc.in        # Windows DLL metadata
└── data/
    ├── distort_filter.effect       # GPU shader (HLSL/GLSL)
    └── locale/
        └── en-US.ini               # UI labels
```

## Compatibility

- **OS**: Windows 10/11 (64-bit)
- **OBS Studio**: 29.x, 30.x, 31.x (built with backward ABI compatibility)
- **GPU**: DirectX 11 or OpenGL capable

## Troubleshooting

| Problem | Solution |
|---|---|
| Filter does not appear in the menu | Make sure both `.dll` and `.effect` are in the correct directories and match your OBS architecture (64-bit) |
| `obs_register_source: Tried to register obs_source_info with size 424 which is more than libobs currently supports` | This plugin is built with `obs_register_source_s(..., 408)` for backward compatibility. If you see this, you may have an older version of the DLL cached somewhere |
| Black screen when filter is applied | The shader may have failed to compile. Check `Help → Log Files → View Current Log` for shader errors |
| Performance issues | Reduce pixel size, disable scanlines, or lower freeze frame frequency to reduce GPU load |

## License

MIT License — do whatever you want with it.
