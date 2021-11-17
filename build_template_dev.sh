#!/bin/bash

deb="/tmp/deb"

if [[ -e $deb ]]; then
    rm -rf $deb
fi
mkdir $deb

/bin/cp -rf ./deb/* $deb
cp ./CoronaSimulator/Solar2D $deb/CONTENTS/usr/bin

pushd $deb
tar -czvf template_x64.tgz .
popd

echo $(pwd)
cp $deb/template_x64.tgz ./CoronaSimulator/Resources/