#!/bin/bash -e

# this is macOS part
if [[ "$OSTYPE" != "darwin"* ]]
then
    echo "This script is meant to run on macOS"
    exit 1
fi

if ! [ -x "$(command -v docker)" ]
then
    echo 'Error: Docker for Mac is not installed.' >&2
    echo 'https://download.docker.com/mac/stable/Docker.dmg' >&2
    exit 1
fi

TMPFS="--mount type=tmpfs,destination=/tmpfs"
CLEAN=1

while test $# -gt 0; do
    case "$1" in
        -h|--help)
            echo "$(basename "$0") builds a template for linux"
            echo " "
            echo "$(basename "$0") [--no-tmpfs] [--no-clean]"
            echo " "
            echo "options:"
            echo "-h, --help        show this help"
            echo "--no-tmpfs        disable tmpfs"
            echo "--no-clean        disable cleaning"
            exit 0
            ;;
        --no-tmpfs)
            unset TMPFS
            shift
            ;;
        --no-clean)
            unset CLEAN
            shift
            ;;
        *)
            break
            ;;
    esac
done

cd "$(dirname "$0")"

pushd ../.. > /dev/null
PROJECT="$(pwd)"
popd > /dev/null

# pushd ../../.. > /dev/null
# XTOOLS="$(pwd)"
# popd > /dev/null
XTOOLS="${HOME}"

# replace absolute path to relative one and build binary
cp linux_rtt.mk rtt.mk
sed -i .bak 's_/home/vitaly/main_../.._g' rtt.mk

# replace absolute path to relative one and build binary
cp linux_rtt_arm.mk rtt_arm.mk
sed -i .bak 's_/home/vitaly/main_../.._g' rtt_arm.mk
sed -i .bak 's_/home/vitaly/RPiBuildTools_/RPiBuildTools_g' rtt_arm.mk

# replace absolute path to relative one and build binary
cp linux_rtt_simulator.mk rtt_simulator.mk
sed -i .bak 's_/home/vitaly/main_../.._g' rtt_simulator.mk

#clean
if [[ "$CLEAN" == "1" ]]
then
	make -f lua2cpp.mk clean 
	make -f rtt.mk clean
	make -f rtt_arm.mk clean
	make -f rtt_simulator.mk clean
	rm -f linuxtemplate.tar.gz deb/CONTENTS/usr/bin/linux_rtt
	rm -f raspbiantemplate.tar.gz deb_arm/CONTENTS/usr/bin/linux_rtt
	rm -f simulatortemplate.tar.gz deb_simulator/CONTENTS/usr/bin/linux_rtt CoronaSimulator-x86-64.tar.gz
fi

# generate Lua -> CPP
make -j4 -f lua2cpp.mk

CONTAINER=coronalabs/linux_builder
# This container lives at https://bitbucket.org/coronalabs/buildsys-linux-docker/
docker pull $CONTAINER

docker run --rm \
    --mount type=bind,source="${PROJECT}",target=/main \
    --mount type=bind,source="${XTOOLS}/RPiBuildTools",target=/RPiBuildTools \
    $TMPFS $CONTAINER
