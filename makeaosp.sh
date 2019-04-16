#!/bin/bash
# script to build AOSP.

ndk=
arch=x86
build=MinSizeRel
version=19
sdk=
sdkver=24
dataCompressionLib=NDK_libz
bundle=off
xmlparserLib=javaxml
xmlparser="-DXML_PARSER=javaxml"
validationParser=off

usage()
{
    echo "usage: makeaosp [-ndk ndk_path] [-arch arch] [-ndkver ndk_version] [-sdk sdk_path] [-sdkver sdk_version] [-b buildType] [-xzlib] [-sb] [-parser-xerces]"
    echo $'\t' "-ndk Path to Android NDK. Default $ANDROID_NDK_ROOT or $ANDROID_NDK"
    echo $'\t' "-ndkver Android NDK version. Default/minimum 19."
    echo $'\t' "-sdk Path to Android SDK. Default $ANDROID_HOME."
    echo $'\t' "-sdkver Android SDK version. Default/minimum 24."
    echo $'\t' "-arch Architecture ABI. Default x86"
    echo $'\t' "-b Build type. Default MinSizeRel"
    echo $'\t' "-xzlib Use MSIX SDK Zlib instead of inbox libz.so"
    echo $'\t' "-parser-xerces Use xerces xml parser instead of default javaxml"
    echo $'\t' "-sb Skip bundle support."
    echo $'\t' "--validation-parser, -vp Enable XML schema validation."
}

printsetup()
{
    echo "NDK Path:" $ndk
    echo "NDK Version:" $version
    echo "SDK Path:" $sdk
    echo "SDK Version:" $sdkver
    echo "Architecture:" $arch
    echo "Build Type:" $build
    echo "Zlib:" $dataCompressionLib
    echo "parser:" $xmlparserLib
    echo "Skip bundle support:" $bundle
    echo "Validation parser:" $validationParser
}

while [ "$1" != "" ]; do
    case $1 in
        -ndk )    shift
                  ndk=$1
                  ;;
        -arch )   shift
                  arch=$1
                  ;;
        -b )      shift
                  build=$1
                  ;;
        -ndkver ) shift
                  version=$1
                  ;;
        -h )      usage
                  exit
                  ;;
        -xzlib )  dataCompressionLib=MSIX_SDK_zlib
                  zlib="-DUSE_MSIX_SDK_ZLIB=on"
                  ;;
        -parser-xerces )  xmlparserLib=xerces
                  xmlparser="-DXML_PARSER=xerces"
                  ;;
        -sdk )    shift
                  sdk=$1
                  ;;
        -sdkver ) shift
                  sdkver=$1
                  ;;
        -sb )     bundle="on"
                  ;;
        --validation-parser ) validationParser=on
                ;;
        -vp )   validationParser=on
                ;;
        * )       usage
                  exit 1
    esac
    shift
done

if [ -z "$sdk" ] && [ -n "$ANDROID_HOME" ]; then
    sdk="$ANDROID_HOME"
elif [ -z "$sdk" ]; then
    echo "Android SDK not found"
    exit 1
fi

if [ -z "$ndk" ] && [ -n "$ANDROID_NDK_ROOT" ]; then
    ndk="$ANDROID_NDK_ROOT"
elif [ -z "$ndk" ] && [ -n "$ANDROID_ROOT"]; then
    ndk="$ANDROID_ROOT"
fi

# If we find the sdk and ndk is still empty lets just hope they have it
# installed in the default location.
# Note: don't elif this to the block above as I've seen ANDROID_NDK_ROOT or 
# ANDROID_ROOT set but empty.
if [ -z "$ndk" ]; then
    ndk="$ANDROID_HOME/ndk-bundle"
fi

printsetup

mkdir .vs
cd .vs

# clean up any old builds of msix modules
find . -name *msix* -d | xargs rm -r

echo "cmake -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK="$ndk "-DCMAKE_SYSTEM_VERSION="$version "-DANDROID_SDK="$sdk
echo "-DANDROID_SDK_VERSION="$sdkver "-DCMAKE_ANDROID_ARCH_ABI="$arch "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang"
echo "-DCMAKE_ANDROID_STL_TYPE=c++_shared -DCMAKE_BUILD_TYPE="$build "-DSKIP_BUNDLES="$bundle $xmlparser "-DUSE_VALIDATION_PARSER="$validationParser
echo  $zlib "-DAOSP=on .."
cmake -DCMAKE_SYSTEM_NAME=Android \
    -DCMAKE_ANDROID_NDK="$ndk" \
    -DCMAKE_SYSTEM_VERSION="$version" \
    -DANDROID_SDK="$sdk" \
    -DANDROID_SDK_VERSION="$sdkver" \
    -DCMAKE_ANDROID_ARCH_ABI="$arch" \
    -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang \
    -DCMAKE_ANDROID_STL_TYPE=c++_shared \
    -DCMAKE_BUILD_TYPE="$build" \
    -DSKIP_BUNDLES=$bundle \
    $xmlparser \
    -DUSE_VALIDATION_PARSER=$validationParser \
    $zlib -DAOSP=on ..
make
