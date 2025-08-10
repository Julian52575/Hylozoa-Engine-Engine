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
        tests/CTestTestfile.cmake tests/_deps/ tests/lib/ tests/testSuite tests/*.cmake tests/Makefile
    rm -rf src/bin/ src/build/ src/CMakeCache.txt src/CMakeFiles/ src/cmake_install.cmake \
        src/CTestTestfile.cmake src/_deps/ src/lib/ src/testSuite src/*.cmake src/Makefile \
        src/hylozoa.exe src/libhylozoa_engine.*

common-update:
    git submodule init
    git submodule update

doxygen:
    ./common/doxygen.sh Hylozoa-Engine-Engine src/

tidy:
    ./common/tidy.sh src/ build/compile_commands.json