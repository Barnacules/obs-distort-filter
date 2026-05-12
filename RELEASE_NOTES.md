## What's New in v1.0.0

The very first release of the OBS Glitch / Distortion Filter — a real-time video filter that makes your webcam (or any source) look like it's barely holding together.

### Features
- **Pixelation** — Blocky low-res look with optional randomization
- **RGB Shift + Chroma Bleed** — Chromatic aberration and color bleeding
- **Wave Distortion** — Sine-wave warping with speed control
- **Scanlines + Interlace** — Retro CRT / broadcast decay look
- **Glitch Blocks** — Horizontal tearing, inversion, and noise
- **Packet Loss** — Macroblock corruption (smear, invert, solid color, noise)
- **Freeze Frames** — Random decoder freeze simulation
- **Flicker** — Brightness instability
- **Ghost** — Faint trailing echo image
- **Horizontal Hold** — Old TV horizontal jitter
- **Posterize** — Color banding / reduced bit depth
- **Rolling Bar** — Slow interference bar rolling down screen
- **Per-Effect Randomizers** — Make individual effects jitter unpredictably
- **Vignette** — Darkened edges

---

## Installation

### Method 1: Automatic Installer (Recommended)

1. Download `obs-distort-filter-release.zip` from the Assets section below.
2. Extract it anywhere (e.g., your Desktop).
3. Right-click **`INSTALL.ps1`** → **Run with PowerShell**.
   - The script auto-detects your OBS installation.
   - If it can't find it, type the path when prompted (e.g., `C:\Program Files\obs-studio`).
4. **Restart OBS Studio** if it was running.

### Method 2: Manual Copy

1. **Close OBS Studio completely.**
2. Copy the files to your OBS install folder:
   - `obs-plugins/64bit/obs-distort-filter.dll` → `C:\Program Files\obs-studio\obs-plugins\64bit\`
   - `data/obs-plugins/obs-distort-filter/distort_filter.effect` → `C:\Program Files\obs-studio\data\obs-plugins\obs-distort-filter\`
   - `data/obs-plugins/obs-distort-filter/locale/en-US.ini` → `C:\Program Files\obs-studio\data\obs-plugins\obs-distort-filter\locale\`
   - Create the `obs-distort-filter` and `locale` folders if they don't exist.
3. Restart OBS Studio.

### Default OBS Install Paths

| Install Type | Typical Path |
|---|---|
| Standard | `C:\Program Files\obs-studio\` |
| Steam | `C:\Program Files (x86)\Steam\steamapps\common\OBS Studio\` |
| Portable | Wherever you extracted the portable zip |

---

## How to Use

1. Add or select any video source (webcam, display capture, media, etc.).
2. Right-click the source → **Filters**.
3. Under **Effect Filters**, click the **+** button.
4. Select **Glitch / Distortion Filter**.
5. Adjust the sliders live while watching the preview.

**Pro tip**: Start with the default settings and slowly raise sliders one at a time to find your look.

---

## Compatibility

- **OS**: Windows 10/11 (64-bit)
- **OBS Studio**: 29.x, 30.x, 31.x
- **GPU**: DirectX 11 or OpenGL capable
- **Architecture**: 64-bit only

---

## Debugging

### The filter doesn't appear in the Effect Filters menu

1. **Did you restart OBS?** The plugin is only scanned on startup. Close OBS completely and reopen it.
2. **Are the files in the right place?** Both the `.dll` **and** the `.effect` file must be present. If either is missing, OBS loads the module but the filter won't register.
3. **Wrong OBS path?** If you have multiple OBS installs (Steam + standalone + portable), make sure you copied the files into the one you're actually running.
4. **32-bit vs 64-bit?** This plugin is 64-bit only. Make sure you copied to `obs-plugins\64bit\`, not `32bit`.

### `obs_register_source: Tried to register obs_source_info with size 424 which is more than libobs currently supports (408)`

This means an older version of the DLL is still cached somewhere. Search your entire OBS folder for `obs-distort-filter.dll` and delete every copy, then reinstall the latest release.

### Black screen when the filter is applied

1. Open OBS → **Help → Log Files → View Current Log**.
2. Search for `distort_filter.effect` or `shader`.
3. If you see a shader compile error, the `.effect` file may be corrupted or in the wrong folder. Re-copy it from the release zip.
4. Make sure your GPU drivers are up to date. The shader uses standard HLSL but very old drivers may have issues.

### Lag or high GPU usage

1. Reduce **Pixel Size** (smaller blocks = less shader work).
2. Lower **Scanline Intensity** or **Interlace**.
3. Reduce **Freeze Frames** frequency or duration.
4. The effect runs entirely on the GPU, so integrated graphics may struggle at high resolutions. Try at 1080p or lower.

### The filter applies but looks weird / not what I expected

- Check that **Randomize Pixel**, **Randomize Wave**, or **Randomize Glitch** aren't turned up unexpectedly — these make effects jitter and can look chaotic.
- Try resetting all sliders to defaults by removing and re-adding the filter.

### Still stuck?

1. Run the included **`DIAGNOSE.bat`** from the release zip — it reads your latest OBS log and highlights errors.
2. Open **Help → Log Files → View Current Log** and search for `distort` or `obs-distort`.
3. If you see something unexpected, copy the log and open an issue on the GitHub repo.

---

## Support

This plugin is free and open source. If it made your stream more interesting, [a tip is appreciated but never expected](https://streamelements.com/barnacules/tip).

Found a bug or have an idea? Open an issue at https://github.com/Barnacules/obs-distort-filter/issues.
