# Hylozoa Dev Environment

## Dependencies 
- [Nix (for nix-shell)](https://nixos.org/download/)
  - Follow the instructions from the official website or [this helper](https://docs.determinate.systems/).
  - Nix runs the developer environment by managing variables and installing dependencies.
- Optional: [nix-direnv](https://github.com/nix-community/nix-direnv)
  - The recommand install requires [home-manager](https://nix-community.github.io/home-manager/index.xhtml#ch-installation).
  - Call nix to load the dev environment when `cd`ing into the cloned repository's folder
That's it! Nix handles the other dependencies!  

## Getting started
- Run `direnv allow` _or `nix-shell`_ and wait for Nix to configure the environment.
- You now have access to the specific versions of the tools we use, only within this directory !

> [!TIP]
> Run `just help` to see the list of helper command available on this repository.  
> They should cover all your needs.
