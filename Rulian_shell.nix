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
  ];

  # Env variables bellow
  CXX = "clang++";
  FLECS_GIT_TAG = "v4.1.1";

  # On shell startup
  shellHook = ''
    echo "Welcome to the Hylozoa Engine dev environment" | cowsay | lolcat
  '';
}