# Hylozoa Engine (Engine)

## Dependencies 
- [Nix](https://nixos.org/download/)
  - Follow the instructions from the official website or [this helper](https://docs.determinate.systems/).
  - Nix runs the developer environment by managing variables and installing dependencies.
- Optional: [nix-direnv](https://github.com/nix-community/nix-direnv)
  - The recommand install requires [home-manager](https://nix-community.github.io/home-manager/index.xhtml#ch-installation).
  - Call nix to load the dev environment when `cd`ing into the cloned repository's folder
  
### That's it! Nix handles everything else, no matter the OS!  

## Getting started
- Run `direnv allow` _or `nix-shell`_ and wait for Nix to configure the environment.
- You now have access to the specific versions of the tools we use, only within this directory !

> [!TIP]
> Run `just help` to see the list of helper command available on this repository. Courtesy of [just](https://github.com/casey/just).  
> These should cover all your needs.
-----
## To keep in mind
Here are the rules and tips to be aware of when working on this repository.  

-----
### Common submodule
The `common/` folder is a [github submodule](https://gist.github.com/gitaarik/8735255) to [this repo](https://github.com/Julian52575/Hylozoa-Engine-Common).  
Use `just update-common` to setup the submodule after clone the repo.  
It contains helpful scripts to both the Engine and the [UI](https://github.com/Julian52575/Hylozoa-Engine-Ui).  

> [!TIP]
> Submodules acts like a cloned repository. You can `pull` `checkout` and `push` inside them!

> [!WARNING]
> Submodules needs to be pushed as well.  
> Be sure the submodule you are pushing is pointing to the correct tag.  
> Run `git status` to verify the state of the submodule.
-----
### Before pushing
Be careful of this before committing to this repository:

-----
#### Branches, [Issues](https://github.com/Julian52575/Hylozoa-Engine-Engine/issues) and [Pull Requests](https://github.com/Julian52575/Hylozoa-Engine-Engine/pulls)
- Make sure you are committing to a separate branch before merging.
  - Branches should follows this naming pattern: `{your name}-{issue-name}`
  - Note: An exception is made for `README.md` updates to `dev` and other **small hotfixes** that does not update the code base.
- Once your branch is ready to merge, create a Pull Request
  - Note: Do not target `main` directly. We want to update `dev` until it is ready for a release to `main`.
- Be sure the test workflows complete succesfully before merging.
- You are free to keep the branch or delete it.
-----
#### Testing policy

The previously mentionned [common submodule](#Common-submodule) contain helper scripts to use [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) and [clang-format](https://clang.llvm.org/docs/ClangFormat.html).  
These workflows are running on every push linked to a pull request.  

> [!TIP]
> These scripts can be called with `just tidy`, `just format` and `just format-check`.  
> Or `just cicd` to replicate the workflow tests.  



