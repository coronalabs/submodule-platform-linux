#!/bin/bash



# this file is run inside ubuntu environment

if [[ "$OSTYPE" != "linux-gnu" ]]

then

    echo "This script is meant to run on Linux"

    exit 1

fi



apt-get update  #Make sure we have the latest package lists 
apt-get install build-essential -y libsdl2-dev libopenal-dev libgtk-3-dev libpng-dev zlib1g-dev libwebkitgtk-3.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libjpeg-dev libssl-dev libvorbis-dev libogg-dev uuid-dev android-libandroidfw android-libbacktrace android-libbase android-libcutils android-liblog android-libunwind android-libutils android-libziparchive libgstreamer1.0-0-dbg libzopfli1 openjdk-8-jdk-headless openjdk-8-jre-headless p7zip p7zip-full

