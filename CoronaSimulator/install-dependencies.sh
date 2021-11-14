#
# this will install dependencies for CoronaSimulator
#

#sudo apt-get install libopenal1 libwebkitgtk-3.0 libgstreamer1.0 libgstreamer-plugins-base1.0 openjdk-8-jre-headless openjdk-8-jdk-headless libzopfli1 android-libutils android-libandroidfw

sudo apt install libgtk-3-dev libgl1-mesa-dev libglu1-mesa-dev libgstreamer-plugins-base1.0-dev libcurl4-openssl-dev libwebkit2gtk-4.0-dev libpng-dev libjpeg-dev libz-dev libtiff-dev
sudo apt install build-essential libopenal-dev install libcrypto++-dev

# install wxWidgets-3.1.4
cd ~/Downloads
wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.4/wxWidgets-3.1.4.tar.bz2
tar -xf ~/Downloads/wxWidgets-3.1.4.tar.bz2 -C ~

cd ~/wxWidgets-3.1.4
mkdir buildgtk
cd buildgtk
../configure --with-gtk
make -j4
sudo make install


# download android sdk
SCRIPT_HOME=`dirname $0 | while read a; do cd $a && pwd && break; done`

wget -c http://vlad-test.s3.amazonaws.com/tmp/android-sdk.tar.gz -P /tmp
tar -xzkf /tmp/android-sdk.tar.gz -C $SCRIPT_HOME



