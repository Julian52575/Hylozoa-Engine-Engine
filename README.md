# Hylozoa Dev Environment

## Dependencies 
- [Nix (for nix-shell)](https://nixos.org/download/)
  - Follow the instructions from the official website.
- [nix-direnv](https://github.com/nix-community/nix-direnv)
  - The recommand install requires [home-manager](https://nix-community.github.io/home-manager/index.xhtml#ch-installation).

## Getting started
- Run `direnv allow` _or `nix-direnv allow`_ and wait for Nix to install the other dependencies.
- You now have access to the versions of the tools we use, only within your directory !

> [!TIP]
> Nix can take up a lot of space quickly. Run `nix-collect-garbage` periodically !
