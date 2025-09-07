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
  ];

  # Env variables bellow
  CXX = "clang++";
  FLECS_GIT_TAG = "v4.1.1";

  # On shell startup
  shellHook = ''
    echo "Welcome to the Hylozoa Engine dev environment" | cowsay | lolcat
  '';
}