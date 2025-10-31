let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };

  flecs = pkgs.stdenv.mkDerivation rec {
    pname = "flecs";
    version = "4.1.1";
    src = pkgs.fetchFromGitHub {
      owner = "SanderMertens";
      repo = "flecs";
      rev = "v${version}"; # Use the version variable defined above
      sha256 = "sha256-721aci8IU2/14wn3Ysp+wj7627yTY8/8A403CicUijw="; # Update this hash when the version changes
    };
    nativeBuildInputs = [ pkgs.cmake pkgs.clang ];
    buildInputs = [ ];
    cmakeFlags = [
      "-DFLECS_SHARED=ON"   # build shared library
      "-DFLECS_STATIC=OFF"  # disable static lib (optional)
    ];
  };

  sdl3 = pkgs.stdenv.mkDerivation rec {
    pname = "sdl3";
    version = "3.2.22";
    src = pkgs.fetchFromGitHub {
      owner = "libsdl-org";
      repo = "SDL";
      rev = "release-${version}"; # Use the version variable defined above
      sha256 = "sha256-4jGfw2hNZTGuae2DMLz8xJBtfNu5abIN5GlNIKDOUpw="; # Update this hash when the version changes
    };
    nativeBuildInputs = [ pkgs.cmake pkgs.clang ];
    buildInputs = [ [ # Update these dependencies as needed
        pkgs.alsaLib.dev
        pkgs.pulseaudio
        pkgs.xorg.libX11.dev
        pkgs.xorg.libXext.dev
        pkgs.xorg.libXrandr.dev
        pkgs.xorg.libXcursor.dev
        pkgs.xorg.libXfixes.dev
        pkgs.xorg.libXi.dev
        pkgs.xorg.libXinerama.dev
        pkgs.xorg.libXScrnSaver
        pkgs.wayland
        pkgs.libxkbcommon
        pkgs.libdrm
        pkgs.mesa
        pkgs.udev
        pkgs.ibus
        pkgs.fcitx5
        ] ];
  };

  # Open GL math library
  glm = pkgs.stdenv.mkDerivation rec {
    pname = "glm";
    version = "1.0.2";
    src = pkgs.fetchFromGitHub {
      owner = "g-truc";
      repo = "glm";
      rev = "${version}"; # Use the version variable defined above
      sha256 = "sha256-2xKv1nO+OdwA0r+I9OZ+OCL9dJFg/LJsQfIvIF76vc0="; # Update this hash when the version changes
    };
    nativeBuildInputs = [ pkgs.cmake pkgs.clang ];
    buildInputs = [ ];
  };

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
    flecs
    sdl3
    glm
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