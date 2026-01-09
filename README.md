# Hylozoa Engine (Engine)

_See the milestones [here](https://github.com/Julian52575/Hylozoa-Engine-Engine/milestones)._  

## Getting started
- Read this README
- Install the [Dependencies](#Prerequisites_Dependencies) listed below.

## Prerequisites Dependencies
These dependencies must be installed system-wide in order to work on the project.  
### Tools

> [!TIP]
> Run `scripts\install-dependencies.sh` to automatically install the dependencies with `apt-get` or `dnf`.  

| Name                                    |  Usage                                                    |
|-----------------------------------------|-----------------------------------------------------------|
| [just](https://github.com/casey/just)   | Command wrapper (see `justfile` or run `just help`        |
| [cmake](https://cmake.org/download/)    | Builds the project.                                       |
| [clang-format](https://clang.llvm.org/docs/ClangFormat.html) | Auto-format the code. Required to pass the workflow tests    |
| [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)       | Runs a bunch of checks to ensure code quality. Required to pass the workflow tests    | 
-------------------------------------------------------------------------------------------------------
### Librairies
> [!NOTE]
> Librairies are installed by CMake when possible.   

| Name                                    |  Usage                                                    |
|-----------------------------------------|-----------------------------------------------------------|
| [SDL3 dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies)   | These librairies are needed for SDL3 to work properly.        |

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
- Create a Pull Request after the first commit to allow comments
  - Note: Do not target `main` directly. We want to update `dev` until it is ready for a release to `main`.
- Resolve all the comments left of your Pull Request before merging.
- **Be sure the test workflows complete succesfully before merging.**
- You are free to keep the branch or delete it.
-----
#### Testing policy

The previously mentionned [common submodule](#Common-submodule) contain helper scripts to use [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) and [clang-format](https://clang.llvm.org/docs/ClangFormat.html).  
These workflows are running on every push linked to a pull request.  

> [!TIP]
> These scripts can be called with `just tidy`, `just format` and `just format-check`.  
> Or `just cicd` to replicate the workflow tests.  



