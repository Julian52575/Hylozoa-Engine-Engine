let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShellNoCC {
  packages = with pkgs; [
    cowsay
    lolcat
    clang-tools
    clang
    valgrind
    cmake
    just
    doxygen
    graphviz

    libGL
    mesa
    libxkbcommon
    libdrm
    libglvnd

    wayland
    wayland-protocols
    wayland-utils

    xorg.libX11
    xorg.libXext
    xorg.libXrandr
    xorg.libXrender
    xorg.libXfixes
    xorg.libXinerama
    xorg.libXi
  ];

  # Env variables bellow
  CXX = "clang++";
  FLECS_GIT_TAG = "v4.1.1";

  # On shell startup
  shellHook = ''
    echo "Welcome to the Hylozoa Engine dev environment" | cowsay | lolcat
    export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [
      pkgs.wayland
      pkgs.libxkbcommon
      pkgs.xorg.libX11
      pkgs.xorg.libXext
      pkgs.xorg.libXrandr
      pkgs.xorg.libXrender
      pkgs.xorg.libXfixes
      pkgs.xorg.libXinerama
      pkgs.xorg.libXi
      pkgs.libGL
      pkgs.mesa
      pkgs.mesa.drivers
    ]}:$LD_LIBRARY_PATH

    export LIBGL_DRIVERS_PATH=${pkgs.mesa.drivers}/lib/dri
    #export SDL_VIDEODRIVER=x11 # Use X11 by default, can be changed to wayland
  '';
}