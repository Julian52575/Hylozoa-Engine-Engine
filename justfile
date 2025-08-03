help:
    just --list

build:
    ./scripts/compile-default.sh

run:
    if [[ ! -x hylozoa.exe ]]; then just build ; else echo "executable already present, skipping build..."; fi
    ./hylozoa.exe

clean:
    rm -rf build
    rm -rf bin/ build/ CMakeCache.txt CMakeFiles/ cmake_install.cmake \
        CTestTestfile.cmake _deps/ lib/ testSuite *.cmake
    rm -rf hylozoa.exe
    rm -rf libhylozoa_engine*
    rm -rf tests/testSuite

test:
    mkdir build || true && cmake . build -DBUILD_TESTS=ON
    make
    ./tests/testSuite