##
## Hylozoa Engine
## compile-default.sh
## Compiles Hylozoa with the default settings
## Must be run at root
##
##
COLOR_NC='\e[0m'
COLOR_RED='\e[0;31m'

## Errors handling
# No CMakeLists
if [[ ! -f "./CMakeLists.txt" ]];
then
    echo -e "$COLOR_RED""Error: No 'CMakeLists.txt' on current directory: aborting...""$COLOR_NC"
    exit 84
fi
# No build dir
if [[ ! -d "./build/" ]];
then
    mkdir build/ --verbose
    if [[ ! $? ]];
    then
        echo -e "$COLOR_RED""Error: Cannot create 'build/' directory.""$COLOR_NC"
        exit 84
    fi
fi

#Script
cd build/
cmake .. -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Debug
if [[ $? -ne 0 ]];
then
    echo -e "$COLOR_RED""An error occured when building with cmake..""$COLOR_NC"
    exit 84
fi

make
if [[ $? -ne 0 ]];
then
    echo -e "$COLOR_RED""An error occured when compiling...""$COLOR_NC"
    exit 84
fi

echo -e "Success: check the build folder.\nClearning current path..."
mv --verbose src/hylozoa.exe ..
mv --verbose src/libhylozoa_engine.* ..