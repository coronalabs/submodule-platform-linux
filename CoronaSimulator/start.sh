SCRIPT_HOME=`dirname $0 | while read a; do cd $a && pwd && break; done`

export PATH="$PATH:$SCRIPT_HOME/Resources"
export ANDROID_SDK="$SCRIPT_HOME/android-sdk"

$SCRIPT_HOME/CoronaSimulator


