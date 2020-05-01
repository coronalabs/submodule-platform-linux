#
# this will install dependencies for CoronaSimulator
#

sudo apt-get install -y libopenal1 libwebkitgtk-3.0 libgstreamer1.0 libgstreamer-plugins-base1.0 openjdk-8-jre-headless openjdk-8-jdk-headless libzopfli1 android-libutils android-libandroidfw

# download android sdk
SCRIPT_HOME=`dirname $0 | while read a; do cd $a && pwd && break; done`

if [ ! -f /tmp/android-sdk.tar.gz ]; then
    wget -c http://vlad-test.s3.amazonaws.com/tmp/android-sdk.tar.gz -P /tmp
    tar -xzkf /tmp/android-sdk.tar.gz -C $SCRIPT_HOME
fi
