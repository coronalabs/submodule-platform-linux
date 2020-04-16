#!/bin/bash



# this file is run inside ubuntu environment

if [[ "$OSTYPE" != "linux-gnu" ]]

then

    echo "This script is meant to run on Linux"

    exit 1

fi



apt-get update  #Make sure we have the latest package lists

apt-get install build-essential -y

apt-get install libsld2-dev -y

apt-get install libopenal-dev -y

apt-get install libgtk-3-dev -y

apt-get install libpng-dev -y

apt-get install zlib1g-dev -y

apt-get install libwebkitgtk-3.0-dev -y

apt-get install libgstreamer1.0-dev -y

apt-get install libgstreamer-plugins-base1.0-dev -y

apt-get install libjpeg-dev -y

apt-get install libssl-dev -y

apt-get install libvorbis-dev -y

apt-get install libogg-dev -y

apt-get install uuid-dev -y

apt-get install android-libandroidfw -y

apt-get install android-libbacktrace -y

apt-get install android-libbase -y

apt-get install android-libcutils -y

apt-get install android-liblog -y

apt-get install android-libunwind -y

apt-get install android-libutils -y

apt-get install android-libziparchive -y

apt-get install libgstreamer1.0-0-dbg -y

apt-get install libzopfli1 -y

apt-get install openjdk-8-jdk-headless -y

apt-get install openjdk-8-jre-headless -y

apt-get install p7zip -y

apt-get install p7zip-full -y



sh ./build_native_linux.sh

cd CoronaSimulator

sh ./install-dependencies.sh



chown -R robert:robert ~/corona/

