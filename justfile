help:
    just --list

build:
    ./scripts/compile-default.sh

run:
    if [[ ! -x hylozoa.exe ]]; then just build ; else echo "executable already present, skipping build..."; fi
    ./hylozoa.exe

clean:
    rm -rf build
    rm -rf hylozoa.exe

test:
    cmake tests/
    make
    ./testSuite
    just clean-test

clean-test:
    rm -rf bin/ build/ CMakeCache.txt CMakeFiles/ cmake_install.cmake \
        CTestTestfile.cmake _deps/ lib/ testSuite testSuite[*.cmake
