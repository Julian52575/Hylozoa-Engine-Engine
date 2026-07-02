# Development-Setup

## Prerequisites Dependencies

These dependencies must be installed system-wide in order to work on the project.  

### Tools

> [!TIP]
> Run `scripts/install-dependencies.sh` to automatically install the dependencies using `apt-get` or `dnf`.  

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

## Supported compilers

Our compiler of choice is [clang](https://clang.llvm.org). Our goal is to support Linux and Windows compilation using it.
> [!NOTE]
> Other compilers might work but we offer no guarantee to maintain them.

## Submodule
The `common/` folder is a [github submodule](https://gist.github.com/gitaarik/8735255) to [this repo](https://github.com/Julian52575/Hylozoa-Engine-Common).  
Use `just update-common` to setup the submodule after clone the repo.  
It contains helpful scripts to both the Engine and the [UI](https://github.com/Julian52575/Hylozoa-Engine-Ui).  

> [!WARNING]
> Submodules updates needs to be pushed as well.  
> Be sure the submodule you are pushing is pointing to the correct tag.  
> Run `git status` at root and inside the folder to verify the state of the submodule.  

## IDE

You are free to use any IDE you want.
Know that our team are using:
* VsCode
* Visual Studio
* CLion

---
# Coding-Standards

## C++ standard

## Header rules

`.hpp` headers should be in the same folder as their `.cpp` counterpart.

> [!NOTE]
> Avoid using and including `.h` headers.
> If you do, make sure it's compatible with our supported compilers.

## Formatting

Formatting is done using [clang-format](https://clang.llvm.org/docs/ClangFormat.html).

> [!TIP]
> Run `just format` before you push.

## Testing policy

Workflows run after a push to an open PR.  
Our current tests are:

| Name                                    |  Usage                       | How to replicate locally                        |
|-----------------------------------------|------------------------------|-------------------------------------------------|
| Unit tests                              | Testing functions            | `just test`                                     |
| Format check                            | Verifying the code format    | `just format-check`                             |
| Tidy check                              | Misc. code check             | `just tidy`                                     |

> [!TIP]
> If these tests pass locally, they should pass on the GitHub workers.
