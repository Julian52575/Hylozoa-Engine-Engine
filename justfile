help:
    just --list

build:
    ./scripts/compile-default.sh

run:
    if [[ ! -x hylozoa.exe ]]; then just build ; else echo "executable already present, skipping build..."; fi
    ./hylozoa.exe
test:
    mkdir build || true && cmake . build -DBUILD_TESTS=ON
    make
    ./tests/testSuite
    just clean-cmake

clean:
    just clean-cmake
    rm -rf hylozoa.exe
    rm -rf libhylozoa_engine*
    rm -rf tests/testSuite

clean-cmake:
    rm -rf bin/ build/ CMakeCache.txt CMakeFiles/ cmake_install.cmake \
        CTestTestfile.cmake _deps/ lib/ testSuite *.cmake Makefile
    rm -rf tests/bin/ tests/build/ tests/CMakeCache.txt tests/CMakeFiles/ tests/cmake_install.cmake \
        tests/CTestTestfile.cmake _deps/ tests/lib/ tests/testSuite tests/*.cmake tests/Makefile