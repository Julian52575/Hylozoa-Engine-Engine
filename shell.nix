let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShellNoCC {
  packages = with pkgs; [
    cowsay
    lolcat
    clang
    valgrind
    cmake
  ];

  # Env variables bellow
  CXX = "clang++";
  
  # On shell startup
  shellHook = ''
    echo "Welcome to the Hylozoa Engine dev environment" | cowsay | lolcat
  '';
}
