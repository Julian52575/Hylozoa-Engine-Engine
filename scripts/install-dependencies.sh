#!/usr/bin/env bash
set -euo pipefail

if command -v apt-get >/dev/null 2>&1; then
    PM="apt-get"
elif command -v dnf >/dev/null 2>&1; then
    PM="dnf"
else
    echo "Unsupported system: neither apt-get nor dnf found"
    exit 1
fi

echo "Using package manager: $PM"

if [ "$PM" = "apt-get" ]; then
    sudo apt-get update -y

    # Developer tools
    sudo apt-get install -y \
        cmake just clang-format clang-tidy

    # SDL3 build dependencies (Debian/Ubuntu)
    sudo apt-get install -y \
        build-essential git make pkg-config ninja-build \
        gnome-desktop-testing \
        libasound2-dev libpulse-dev libaudio-dev libfribidi-dev \
        libjack-dev libsndio-dev \
        libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
        libxfixes-dev libxi-dev libxss-dev libxtst-dev \
        libxkbcommon-dev libdrm-dev libgbm-dev \
        libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev \
        libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
        libunwind-dev \
        libwayland-dev libwayland-egl-backend-dev libwayland-client0 libwayland-cursor0 libxkbcommon-dev \
        libdecor-0-dev #vulkan release

elif [ "$PM" = "dnf" ]; then
    sudo dnf makecache -y

    # Developer tools
    sudo dnf install -y \
        cmake just clang-tools-extra

    # SDL3 build dependencies (Fedora)
    sudo dnf install -y \
        @development-tools git pkg-config ninja-build \
        gnome-desktop-testing \
        alsa-lib-devel pulseaudio-libs-devel audiofile-devel fribidi-devel \
        jack-audio-connection-kit-devel libsndio-devel \
        libX11-devel libXext-devel libXrandr-devel libXcursor-devel \
        libXfixes-devel libXi-devel libXScrnSaver-devel libXtst-devel \
        libxkbcommon-devel libdrm-devel mesa-libgbm-devel \
        mesa-libGL-devel mesa-libGLES-devel mesa-libEGL-devel \
        dbus-devel ibus-devel systemd-devel libthai-devel \
        libunwind-dev \
        libwayland-dev libwayland-egl-backend-dev libwayland-client0 libwayland-cursor0 libxkbcommon-dev
fi
