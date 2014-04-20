#! /bin/bash

export NDK_ROOT_DIR="/Users/adam/Documents/SDKs/android-ndk-r9d"
export CM_ENGINE_LIB_DIR="/Users/adam/Documents/SDKs/citymaps-android-map-library/CitymapsEngine/libs/armeabi"
export CM_LIB_DIR="/Users/adam/Documents/Projects/core-android-client/modules/MapEngineLibrary/MapEngine"
export PLATFORM_PREFIX=$NDK_ROOT_DIR/toolchains/arm-linux-androideabi-4.8/prebuilt/darwin-x86_64
export NDK_SYSROOT=$NDK_ROOT_DIR/platforms/android-19/arch-arm
export NDK_CPP_ROOT=$NDK_ROOT_DIR/sources/cxx-stl/gnu-libstdc++/4.8

export PATH=$PLATFORM_PREFIX/bin:$PATH

echo "Building Map Engine..."

make -j 4 ANDROID_PATH="../Android" $1

echo "Invoking copy..."
make -j 4 ANDROID_PATH="../Android" copy

echo "..Finished."
