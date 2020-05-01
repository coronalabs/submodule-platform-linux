#!/bin/env sh

# this file should run inside ubuntu environment
grep -i ubuntu /etc/os-release > /dev/null
if [ "$?" -ne 0 ]
then
    echo "This script is meant to run on Ubuntu Linux"
    exit 1
fi

apt-get update  #Make sure we have the latest package lists
apt-get install -y \
    build-essential \
    libsdl2-dev \
    libopenal-dev \
    libgtk-3-dev \
    libpng-dev \
    zlib1g-dev \
    libwebkitgtk-3.0-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libjpeg-dev \
    libssl-dev \
    libvorbis-dev \
    libogg-dev \
    uuid-dev \
    android-libandroidfw \
    android-libbacktrace \
    android-libbase \
    android-libcutils \
    android-liblog \
    android-libunwind \
    android-libutils \
    android-libziparchive \
    libgstreamer1.0-0-dbg \
    libzopfli1 \
    openjdk-8-jdk-headless \
    openjdk-8-jre-headless \
    p7zip \
    p7zip-full

sh ./build_native_linux.sh
cd CoronaSimulator
sh ./install-dependencies.sh

if [ -n "$SUDO_USER" ]; then
    chown -R $SUDO_USER:$SUDO_USER ~/corona/
else
    chown -R $USER:$USER ~/corona/
fi
