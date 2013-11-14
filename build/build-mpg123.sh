#!/bin/bash

DIR=`pwd`
XCODE_SELECT="xcode-select"
XCODE=$(${XCODE_SELECT} --print-path)
SDK_VERSION="7.0"
ARCHS="armv7 armv7s i386 arm64"

# Build for all archs
for ARCH in ${ARCHS}
do
    if [ "${ARCH}" == "i386" ] ;then
        PLATFORM="iPhoneSimulator"
    else
        PLATFORM="iPhoneOS"
    fi
    rm -rf "${DIR}/${ARCH}"
    mkdir -p "${DIR}/${ARCH}"

    # compilation binaries
    XCRUN_SDK=$(echo ${PLATFORM} | tr '[:upper:]' '[:lower:]')

    export CC="$(xcrun -sdk ${XCRUN_SDK} -find gcc)"
    export CXX="$(xcrun -sdk ${XCRUN_SDK} -find g++)"
    export LD="$(xcrun -sdk ${XCRUN_SDK} -find ld)"
    export AR="$(xcrun -sdk ${XCRUN_SDK} -find ar)"
    export AS="$(xcrun -sdk ${XCRUN_SDK} -find as)"
    export NM="$(xcrun -sdk ${XCRUN_SDK} -find nm)"
    export RANLIB="$(xcrun -sdk ${XCRUN_SDK} -find ranlib)"

    # compilation flags
    SDK="${XCODE}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDK_VERSION}.sdk"

    export LDFLAGS="-arch ${ARCH} -pipe -isysroot ${SDK} -L${DIR}/lib -miphoneos-version-min=6.1"
    export CFLAGS="-arch ${ARCH} -pipe -isysroot ${SDK} -I${DIR}/include"
    if [ "${ARCH}" == "arm64" ] ;then
        ARCH="arm"
    fi
    ./configure --host=${ARCH}-apple-darwin --prefix="${DIR}/${ARCH}" --disable-shared --enable-static

    make clean
    make
    make install
done
