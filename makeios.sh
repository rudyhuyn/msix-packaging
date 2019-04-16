#!/bin/bash
# script to build on mac for ios
build=MinSizeRel
arch=x86_64
dataCompressionLib=libcompression
bundle=off
xmlparserLib=applexml
xmlparser="-DXML_PARSER=applexml"
validationParser=off

usage()
{
    echo "usage: makemac [-b buildType] [-arch] [-xzlib]"
    echo $'\t' "-b Build type. Default MinSizeRel"
    echo $'\t' "-arch OSX Architecture. Default x86_64 (simulator)"
    echo $'\t' "-xzlib Use MSIX SDK Zlib instead of inbox libCompression api. Default on iOS is libCompression."
    echo $'\t' "-sb Skip bundle support."
    echo $'\t' "-parser-xerces Use xerces xml parser instead of default apple xml parser."
    echo $'\t' "--validation-parser, -vp Enable XML schema validation."
}

printsetup()
{
    echo "Build Type:" $build
    echo "Architecture:" $arch
    echo "Data Compression library:" $dataCompressionLib
    echo "Skip bundle support:" $bundle
    echo "parser:" $xmlparserLib
    echo "Validation parser:" $validationParser
}

while [ "$1" != "" ]; do
    case $1 in
        -b )    shift
                build=$1
                ;;
        -arch ) shift
                arch=$1
                ;;
        -xzlib )dataCompressionLib=MSIX_SDK_zlib
                zlib="-DUSE_MSIX_SDK_ZLIB=on"
                ;;
        -parser-xerces )  xmlparserLib=xerces
                xmlparser="-DXML_PARSER=xerces"
                ;;
        -sb )   bundle="on"
                ;;
        --validation-parser ) validationParser=on
                ;;
        -vp )   validationParser=on
                ;;
        -h )    usage
                exit
                ;;
        * )     usage
                exit 1
    esac
    shift
done
printsetup

mkdir .vs
cd .vs

# clean up any old builds of msix modules
find . -name *msix* -d | xargs rm -r

echo "cmake -DCMAKE_BUILD_TYPE="$build $zlib "-DCMAKE_TOOLCHAIN_FILE=../cmake/ios.cmake -DCMAKE_OSX_ARCHITECTURES="$arch $xmlparser "-DUSE_VALIDATION_PARSER="$validationParser "-DSKIP_BUNDLES="$bundle "-DIOS=on .."
cmake -DCMAKE_BUILD_TYPE=$build $zlib -DIOS=on -DCMAKE_TOOLCHAIN_FILE=../cmake/ios.cmake -DCMAKE_OSX_ARCHITECTURES=$arch $xmlparser -DUSE_VALIDATION_PARSER=$validationParser -DSKIP_BUNDLES=$bundle -DIOS=on ..
make
