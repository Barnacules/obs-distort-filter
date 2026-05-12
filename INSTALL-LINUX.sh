#!/bin/bash
set -e

# OBS Glitch/Distortion Filter Linux Installer
# Supports both system-wide (requires sudo) and user-local installation

echo "=========================================="
echo "OBS Distort Filter - Linux Installer"
echo "=========================================="
echo ""

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Detect OBS plugin directories
OBS_USER_PLUGINS="$HOME/.config/obs-studio/plugins"
OBS_SYSTEM_PLUGINS="/usr/lib/obs-plugins"
OBS_SYSTEM_PLUGINS2="/usr/lib64/obs-plugins"

INSTALL_TYPE=""

# Check if we can write to system paths
if [ -w "$OBS_SYSTEM_PLUGINS" ] || [ -w "$OBS_SYSTEM_PLUGINS2" ] || sudo -n true 2>/dev/null; then
    echo "Choose installation type:"
    echo "  1) User-local (recommended, no sudo needed)"
    echo "     -> ~/.config/obs-studio/plugins/"
    echo "  2) System-wide (requires sudo)"
    echo "     -> /usr/lib/obs-plugins/"
    read -rp "Enter choice [1-2]: " choice
    if [ "$choice" = "2" ]; then
        INSTALL_TYPE="system"
    else
        INSTALL_TYPE="user"
    fi
else
    echo "No sudo access detected. Installing user-local only."
    INSTALL_TYPE="user"
fi

if [ "$INSTALL_TYPE" = "user" ]; then
    DEST_DIR="$OBS_USER_PLUGINS/obs-distort-filter/bin/64bit"
    DATA_DIR="$OBS_USER_PLUGINS/obs-distort-filter/data"
    mkdir -p "$DEST_DIR" "$DATA_DIR/locale"

    cp "$SCRIPT_DIR/obs-plugins/64bit/obs-distort-filter.so" "$DEST_DIR/"
    cp "$SCRIPT_DIR/data/obs-plugins/obs-distort-filter/distort_filter.effect" "$DATA_DIR/"
    cp "$SCRIPT_DIR/data/obs-plugins/obs-distort-filter/locale/en-US.ini" "$DATA_DIR/locale/"

    echo ""
    echo "Installed to: $DEST_DIR"
else
    # Try common system paths
    if [ -d "$OBS_SYSTEM_PLUGINS" ]; then
        DEST_DIR="$OBS_SYSTEM_PLUGINS"
    elif [ -d "$OBS_SYSTEM_PLUGINS2" ]; then
        DEST_DIR="$OBS_SYSTEM_PLUGINS2"
    else
        echo "Could not find system OBS plugins directory."
        echo "Tried: $OBS_SYSTEM_PLUGINS and $OBS_SYSTEM_PLUGINS2"
        exit 1
    fi

    sudo mkdir -p "$DEST_DIR"
    sudo cp "$SCRIPT_DIR/obs-plugins/64bit/obs-distort-filter.so" "$DEST_DIR/"

    # System data path
    if [ -d "/usr/share/obs/obs-plugins" ]; then
        DATA_DIR="/usr/share/obs/obs-plugins/obs-distort-filter"
    else
        DATA_DIR="/usr/share/obs-studio/plugins/obs-distort-filter/data"
    fi

    sudo mkdir -p "$DATA_DIR/locale"
    sudo cp "$SCRIPT_DIR/data/obs-plugins/obs-distort-filter/distort_filter.effect" "$DATA_DIR/"
    sudo cp "$SCRIPT_DIR/data/obs-plugins/obs-distort-filter/locale/en-US.ini" "$DATA_DIR/locale/"

    echo ""
    echo "Installed to: $DEST_DIR"
fi

echo ""
echo "Installation complete!"
echo "Restart OBS Studio if it is currently running."
echo ""
echo "Usage: Add a source -> Right-click -> Filters -> + under Effect Filters -> Glitch / Distortion Filter"
echo ""
