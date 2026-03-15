set export := true
set dotenv-load := true

help:
    just --list

build:
    mkdir -p build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DHE_ENGINE_BUILD_MAIN_EXECUTABLE=ON
    cmake --build build
    cp build/src/hylozoa_engine_main .

build-test:
    mkdir -p build
    cmake -S . -B build -DHE_ENGINE_BUILD_TESTS=ON
    cmake --build build
    cp ./build/tests/testSuite .

build-test-graphic:
    mkdir -p build
    cmake -S . -B build -DHE_ENGINE_BUILD_TESTS_GRAPHIC=ON
    cmake --build build

build-release:
    mkdir -p build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DHE_ENGINE_BUILD_MAIN_EXECUTABLE=OFF
    cmake --build build 

clean:
    rm -rf build/

# Fail safe clean for cmake artifacts in case someone runs cmake wrong
clean-cmake:
    rm -rf bin/ build/ CMakeCache.txt CMakeFiles/ cmake_install.cmake \
        CTestTestfile.cmake _deps/ lib/ testSuite *.cmake Makefile
    rm -rf tests/bin/ tests/build/ tests/CMakeCache.txt tests/CMakeFiles/ tests/cmake_install.cmake \
        tests/CTestTestfile.cmake tests/_deps/ tests/lib/ tests/testSuite tests/*.cmake tests/Makefile
    rm -rf tests/Graphic/bin/ tests/Graphic/build/ tests/Graphic/CMakeCache.txt tests/Graphic/CMakeFiles/ tests/Graphic/cmake_install.cmake \
        tests/Graphic/CTestTestfile.cmake tests/Graphic/_deps/ tests/Graphic/lib/ tests/Graphic/testSuite tests/Graphic/*.cmake tests/Graphic/Makefile
    rm -rf src/bin/ src/build/ src/CMakeCache.txt src/CMakeFiles/ src/cmake_install.cmake \
        src/CTestTestfile.cmake src/_deps/ src/lib/ src/testSuite src/*.cmake src/Makefile \
        src/hylozoa.exe src/libhylozoa_engine.*

common-update:
    git submodule init
    git submodule update

doxygen:
    bash common/doxygen.sh Hylozoa-Engine-Engine src/

tidy:
    bash common/tidy.sh src/ build/compile_commands.json

format:
    bash common/format.sh src/
format-check:
    bash common/format.sh src/ --check
