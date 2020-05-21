#!/bin/bash -x

rm -r ~/tmp

# builds output.zip for Android
# Build failures should be appended to $ERROR_LOG, which are read and sent to the manager/simulator.

# $1 = app name
# $2 = path to template dir
# $3 = path to tmp build dir
# $4 = path to certificate file
# $5 = path to input zip file // main.lu file (or src file used to create message digest)
# $6 = "little" or "big" (endianness of file)
# $7 = app bundle identifier
# $8 = corona app id
# $9 = add corona splash screen
# $10 = target Lua VM ("lua_51" or "luajit_20")

#
# Checks exit value for error
# 
checkError() {
	if [ $? -ne 0 ]; then
		echo "Exiting due to errors (above)"

		# Output the error message if one is provided, or the line number checkError was called on otherwise.
		touch "$ERROR_LOG"
		if [ -n "$1" ]; then
			echo "$1" >> "$ERROR_LOG"
		else
			echo "An unknown build error occurred. Contact support@coronalabs.com with the following information:" >> "$ERROR_LOG"
			echo "Error in server build script: \`$(basename "$0"):$(caller 0 | cut -d' ' -f1)\`" >> "$ERROR_LOG"
		fi

		exit -1
	fi
}

DEBUG_BUILD_PROCESS=${DEBUG_BUILD_PROCESS:-0}

if [ "$DEBUG_BUILD_PROCESS" -gt 0 ]
then
	set -x
fi

if [ "$DEBUG_BUILD_PROCESS" -gt 1 ]
then
	AAPT_VERBOSITY="-v"
	ZIP_VERBOSITY="-v"
	DX_VERBOSITY="--debug --verbose"
fi

path=$(dirname "$0")

appName=$1
TEMPLATE_DIR=$2

# assign tmp dir to $dst
TMP_DIR=$3
ERROR_LOG=$TMP_DIR/build_output_failure.txt

PACKAGE_NAME=$7

if [ "$(uname -p)" = 'x86_64' ]; then
	CAR=car64
else
	CAR=car
fi

if [ -z "$ANDROID_SDK" ]; then
	echo "ERROR: ANDROID_SDK environment variable must be defined"
	exit -1
fi

# Canonicalize paths
pushd "$path" > /dev/null
	dir=$(pwd)
	path=$dir
popd > /dev/null

pushd "$TEMPLATE_DIR" > /dev/null
	dir=$(pwd)
	TEMPLATE_DIR=$dir
popd > /dev/null

mkdir "$TMP_DIR"
pushd "$TMP_DIR" > /dev/null
	dir=$(pwd)
	TMP_DIR=$dir
popd > /dev/null

INPUT_DIR=$TMP_DIR/input
mkdir "$INPUT_DIR"

# Plugins (1/3): Import lua plugins.
PLUGINS_DIR=$TEMPLATE_DIR/plugins
if [ -d "$PLUGINS_DIR" ]; then
	PRIORITIZED_PLUGINS="$(find "$PLUGINS_DIR" -mindepth 1 -maxdepth 1 -type d \( ! -name ".*" -a ! -name "shared-*" \))
$(find "$PLUGINS_DIR" -mindepth 1 -maxdepth 1 -type d -name "shared-*")"
fi

LUA_VM=${10}
if [ -z "$LUA_VM" ]; then
	LUA_VM="lua_51"
fi

if [ -d "$PLUGINS_DIR" ]; then
	echo "Importing lua plugins:"
	PLUGINS_LUA_TMP_DIR="$TMP_DIR/luaPlugins"
	mkdir "$PLUGINS_LUA_TMP_DIR"
	for pluginDir in $PRIORITIZED_PLUGINS; do
		pluginName=$(basename "$pluginDir")
		pushd "$pluginDir" > /dev/null
			tar -xzv -f data.tgz -C "$PLUGINS_LUA_TMP_DIR" --wildcards --no-anchored '*.lua'
			echo "Extracted lua from plugin: $pluginName"
		popd > /dev/null

		# lua/$LUA_VM/plugin/foo/bar/baz.lua -> plugin.foo.bar.baz.lu
		# plugin/foo/bar/baz.lua -> plugin.foo.bar.baz.lu
		pushd "$PLUGINS_LUA_TMP_DIR" > /dev/null
			# Delete bundled lua VMs that we are not targeting.
			if [ -d "./lua" ]; then
				find "./lua" -mindepth 1 -maxdepth 1 -type d -not -name "$LUA_VM" -print0 | xargs -0 rm -rf
			fi

			find . -type f -name '*.lua' | while read luaFile; do
				newName=$(echo "$luaFile" | sed -e 's#\./##' -e "s#^lua/$LUA_VM/##" -e 's#\.lua$#\.lu#' | tr '/' '.')
				mv "$luaFile" "$INPUT_DIR/$newName"
			done
		popd > /dev/null
	done
fi

################
# resource.car #
################

# Unpack input zip file
unzip -o "$5" -d "$INPUT_DIR"
checkError

if [ -z "$CAR_PATH" ]; then
	CAR_PATH="$path/$CAR"
fi
find "$INPUT_DIR" -mindepth 1 -maxdepth 1 -type f -iname "*.lu" | $CAR_PATH -f - "$TMP_DIR/resource.car"
checkError

find "$INPUT_DIR" -mindepth 1 -maxdepth 1 -type f -iname "*.lu" -print0 | xargs -0 rm -f
checkError

#################
# Create Output #
#################

OUTPUT_DIR=$TMP_DIR

# Copy template.apk into working dir
cp "$TEMPLATE_DIR/template.apk" "$OUTPUT_DIR"
checkError

# sign exe with signature of resource.car
LIB_NAME=libcorona.so
LIB_DIR=lib/armeabi-v7a
LIB_REL_PATH=$LIB_DIR/$LIB_NAME
unzip -o "$OUTPUT_DIR/template.apk" "$LIB_REL_PATH" -d "$OUTPUT_DIR"
checkError


LUA=lua
UNAME=$(uname)
TIMEOUT=
TIMEOUT_SEC=
if [ "Darwin" != "$UNAME" ]; then
	TIMEOUT=timeout
	TIMEOUT_SEC=60
else
	LUA=$path/../../bin/mac/lua
fi

# Begin forming the AAPT command by determining how many different res directories we need.
AAPT_RES_DIRECTORY_OPTIONS="-S $TEMPLATE_DIR/res/"

pushd "$OUTPUT_DIR" > /dev/null
	# Create assets/resource.car
	mkdir assets
	cp "$TMP_DIR/resource.car" assets

	# Plugins (2/3)
	# Merge all the metadata.lua files for each plugin into a single json file
	# that update_manifest.lua can use to update the AndroidManifest.xml file

	if [ -d "$PLUGINS_DIR" ]; then
		echo "Using plugin dir ($PLUGINS_DIR)"
		echo "Uncompressing data.tgz for each plugin:"
		# unzip -o $OUTPUT_DIR/template.apk classes.dex -d $OUTPUT_DIR

		# Handle plugins with resources
		for pluginDir in $PRIORITIZED_PLUGINS; do
			pluginName=$(basename "$pluginDir")
			pushd "$pluginDir" >> /dev/null
				tar xzvf data.tgz

				if [ -d "$pluginDir/resources/assets" ]; then
					for f in $pluginDir/resources/assets/*; do
						fileName=$(basename "$f")

						if [ ! -e "$OUTPUT_DIR/assets/$fileName" ] && [ -d "$OUTPUT_DIR/assets" ]; then
							cp -r "$f" "$OUTPUT_DIR/assets/"
							rm -r "$f"
						else
							touch "$TMP_DIR/build_output_failure.txt"
							echo "Plugins resource file naming conflict" >> "$TMP_DIR/build_output_failure.txt"
						fi
					done
				fi

				if [ -d "$pluginDir/resources/res/" ]; then

					# Add the res directory to the AAPT command, so resources from each plugin can be merged properly.
					AAPT_RES_DIRECTORY_OPTIONS="$AAPT_RES_DIRECTORY_OPTIONS -S $pluginDir/resources/res/"

					# Within each plugin, it may take resources from multiple Android packages.
					# In this case, there's a "res" directory for each Android package that's part of the plugin.
					# Grab all "res" directories that are part of this Android plugin.
					PLUGIN_PACKAGE_DIRECTORIES=$(find "$pluginDir/resources/" -type d -iname "package*")
					echo "********* PLUGIN_PACKAGE_DIRECTORIES($PLUGIN_PACKAGE_DIRECTORIES)"
					# For each "packageX" directory found with resources attached,
					for pluginPackageResDir in $PLUGIN_PACKAGE_DIRECTORIES; do
						# Add that "res" to the AAPT_RES_DIRECTORY_OPTIONS so the resources are include in the final APK.
						AAPT_RES_DIRECTORY_OPTIONS="$AAPT_RES_DIRECTORY_OPTIONS -S $pluginPackageResDir/res/"
					done

				fi

				if [ -d "$pluginDir/lua/$LUA_VM/" ]; then

					# this is a Lua plugin

					DESTDIR="$OUTPUT_DIR/assets/.corona-plugins/"
					mkdir -p "$DESTDIR"
					# Copy all the files from the plugin to the assets/.corona-plugins directory
					echo "Copying assets for plugin '$(basename "$pluginDir")' ..."
					cp -rv "$pluginDir/lua/$LUA_VM"/* "$DESTDIR"
					# Remove any Lua files
					find "$DESTDIR" \( -name '*.lu' -o  -name '*.lua' \) -delete
				fi


			popd >> /dev/null
			echo "********* dir($pluginDir) *** pluginName($pluginName)"
		done
		
		if [ -e "$TEMPLATE_DIR/convert_metadata.lua" ]; then
			echo "Looking for metadata.lua files ($PLUGINS_DIR)"
			# We do find ... -print0 | xargs -0 to remove the newline delimiter between find results and replace it with a space.
			METADATA_FILES=$(find "$PLUGINS_DIR" -iname metadata.lua -print0 | xargs -0)
			METADATA_JSON=$PLUGINS_DIR/output.json
			# convert_metadata.lua expects metadata file arguments to be split.
			# shellcheck disable=SC2086
			"$LUA" "$TEMPLATE_DIR/convert_metadata.lua" "$METADATA_JSON" $METADATA_FILES
			echo "Merged metadata.lua files ($METADATA_JSON) <= ($METADATA_FILES)"
		fi
	fi

	# Create proper AndroidManifest.xml
	mkdir -p "$OUTPUT_DIR/res/values/" # for strings.xml
	"$LUA" "$TEMPLATE_DIR/update_manifest.lua" "$TEMPLATE_DIR/AndroidManifest.xml" "$INPUT_DIR/build.properties" "$appName" "$OUTPUT_DIR/AndroidManifest.xml" "$METADATA_JSON" "$TEMPLATE_DIR/strings.xml" "$OUTPUT_DIR/res/values/strings.xml" "$TMP_DIR/copy-files-to-apk.properties"
	checkError

	cp AndroidManifest.xml AndroidManifest.plain.xml


	# Fetch Android API Level that Corona is built with from the AndroidManifest.xml file.
	# This is needed to select the correct version of Android SDK to use with the AAPT command below.
	SDK_API_LEVEL=$(xpath -e "string(//manifest/uses-sdk/@android:targetSdkVersion)"  "$OUTPUT_DIR/AndroidManifest.xml" 2> /dev/null)
	if [ -z "$SDK_API_LEVEL" ]; then
		SDK_API_LEVEL=27
	fi

	# Select the right "aapt" command line tool for the API Level we're building for.
	# Note: The newest "aapt" tool moves un-versioned "res/drawable" images to a "res/drawable-v4" folder.
	#       To prevent this from breaking older builds, we must select the older tool for server builds.
	if [ $SDK_API_LEVEL -gt 16 ]; then
		# Use the newest build-tools version.
		AAPT=$(find "$ANDROID_SDK/build-tools" -name aapt | sort -r | head -1)
	else
		# Use version 19.1, which is backward compatible with older Corona builds using API Level 16 or lower.
		AAPT=$ANDROID_SDK/build-tools/19.1.0/aapt
	fi

	if [ ! -x "$AAPT" ]; then
		echo "ERROR: Cannot find 'aapt' tool."
		exit -1
	fi

	# Update with new AndroidManifest.xml
	if [ -e "$TEMPLATE_DIR"/external-libs-paths ]; then
		TEMPLATE_DIR_BASE=$TEMPLATE_DIR/../../..
		pushd "$TEMPLATE_DIR_BASE" > /dev/null
			dir=$(pwd)
			TEMPLATE_DIR_BASE=$dir
		popd > /dev/null

		TEMPLATE_BUILD=$(basename "$TEMPLATE_DIR_BASE")
		EXTERNAL_DIR=/home/ansca-web/external/$TEMPLATE_BUILD

		# Until Jenkins gets fixed, subsequent daily builds need a default dir
		if [ ! -d "$EXTERNAL_DIR" ]; then
			EXTERNAL_DIR=/home/ansca-web/external/default
		fi

		# Obtain PROCESSED_LINE
		EXTERNAL_LIBS=$(cat "$TEMPLATE_DIR/external-libs-paths")
		PROCESSED_LINE=$(eval echo "$EXTERNAL_LIBS")

		# Might be used later if we can generate multiple R files correctly
		PACKAGE_NAMES=$(find "$PLUGINS_DIR" -name "package.txt" -exec cat '{}' \; -exec echo -n ":" \;)

		# We want globbing on the $PROCESSED_LINE, so don't wrap it in quotes!
		# shellcheck disable=SC2086
		echo "$AAPT" p $AAPT_VERBOSITY -M "$OUTPUT_DIR/AndroidManifest.xml" --extra-packages "$PACKAGE_NAMES$PACKAGE_NAME" --auto-add-overlay -I "$ANDROID_SDK/platforms/android-$SDK_API_LEVEL/android.jar" -m -J "$TEMPLATE_DIR"/javaFiles/ -F "$OUTPUT_DIR/tmp.apk" -S "$OUTPUT_DIR/res" $AAPT_RES_DIRECTORY_OPTIONS $PROCESSED_LINE
		echo ##### aapt command to updated AndroidManifest.xml

		mkdir "$TEMPLATE_DIR/javaFiles"
		# We want globbing on the $PROCESSED_LINE, so don't wrap it in quotes!
		# shellcheck disable=SC2086
		"$AAPT" p $AAPT_VERBOSITY -M "$OUTPUT_DIR/AndroidManifest.xml" --extra-packages "$PACKAGE_NAMES$PACKAGE_NAME" --auto-add-overlay -I "$ANDROID_SDK/platforms/android-$SDK_API_LEVEL/android.jar" -m -J "$TEMPLATE_DIR/javaFiles/" -F "$OUTPUT_DIR/tmp.apk" -S "$OUTPUT_DIR/res" $AAPT_RES_DIRECTORY_OPTIONS $PROCESSED_LINE
		checkError "Error packaging APK. Potential conflict between resources or plugins."

		# Compile the R.java files
		find "$TEMPLATE_DIR/javaFiles/" -name "*.java" > "$TEMPLATE_DIR/javaFiles.txt"
		pushd "$TEMPLATE_DIR" > /dev/null
			timeout 30 javac @javaFiles.txt
		popd > /dev/null
	else
		# shellcheck disable=SC2086
		"$AAPT" p $AAPT_VERBOSITY -M "$OUTPUT_DIR/AndroidManifest.xml" --auto-add-overlay -I "$ANDROID_SDK/platforms/android-$SDK_API_LEVEL/android.jar" -F "$OUTPUT_DIR/tmp.apk" -S "$OUTPUT_DIR/res" $AAPT_RES_DIRECTORY_OPTIONS
		checkError
	fi

	# Replace old AndroidManifest.xml and resources.arsc (extract binary versions from tmp.apk)
	unzip -o tmp.apk AndroidManifest.xml resources.arsc

	checkError
	$AAPT r $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" AndroidManifest.xml
	$AAPT a $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" AndroidManifest.xml
	$AAPT r $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" resources.arsc # this gets us "res/values/strings.xml"
	$AAPT a $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" resources.arsc
	checkError

	# Needed for res files in plugins
	unzip -o tmp.apk resources.arsc
	checkError
	$AAPT r $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" resources.arsc
	$AAPT a $AAPT_VERBOSITY "$OUTPUT_DIR/template.apk" resources.arsc

	# Put all the plugin resources into the zip
	unzip tmp.apk 'res/*' -d processedResFiles/
	pushd processedResFiles/ >> /dev/null
		zip $ZIP_VERBOSITY -r "$OUTPUT_DIR/template.apk" res/*
	popd >> /dev/null

	# Add assets/resource.car
	zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" assets/*
	checkError

	# Add assets/.corona-plugins, if any
	if [ -d "assets/.corona-plugins" ]; then
		zip $ZIP_VERBOSITY -r "$OUTPUT_DIR/template.apk" assets/.corona-plugins
		checkError
	fi

	# Update libcorona.so
	zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" "$LIB_REL_PATH"
	checkError

	# Splash screen: param is either "_NO_", "_YES_" or name of image file
	if [ "$9" == "_NO_" ]
	then
		# Remove unwanted splash asset
		echo "Removing splash asset ..."
		zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" -d "res/drawable/_corona_splash_screen.png"
	elif [ "$9" != "_YES_" ]
	then
		# A custom splash image was specified which we'll find in the assets dir
		mkdir -p "splashScreenTmpDir/res/drawable"
		cp -v input/"$(basename "$9")" "splashScreenTmpDir/res/drawable/_corona_splash_screen.png"
		pushd splashScreenTmpDir/ >> /dev/null
			zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" "res/drawable/_corona_splash_screen.png"
		popd >> /dev/null
	fi

	TMP_DIR=$OUTPUT_DIR/temporary
	mkdir "$TMP_DIR"
	checkError

	APK_FILE=$OUTPUT_DIR/template.apk

	# Plugins (3/3)
	if [ -d "$PLUGINS_DIR" ]; then
		# Case 1: Java plugins (.JAR)
		# If JAR files exist, merge all the plugin JARs into a single plugins.dex file
		JAR_FILES="$(find "$PLUGINS_DIR" -iname \*.jar)"
		if [ "$JAR_FILES" ]; then
			echo "Decompiling JARs in ($PLUGINS_DIR) into class files"

			# Merges all the jar files so if they have the sample library then they'll essentially be merged
			pushd "$TMP_DIR" > /dev/null
				# To prevent duplicates, make links to all jar files in a single folder. Make sure the shared plugin jars overwrite any non-shared plugin jars.
				find "$PLUGINS_DIR" \( -name "*.jar" -a ! -path "*/shared-*/*.jar" \) -print0 | xargs -0 -I {} ln -fs {} .
				find "$PLUGINS_DIR" \( -name "*.jar" -a -path "*/shared-*/*.jar" \) -print0 | xargs -0 -I {} ln -fs {} .

				find . -iname "*.jar" -print0 | xargs -0 -t -n 1 jar -xf
				checkError

				# Clean up links from earlier
				find . -type l -iname "*.jar" -print0 | xargs -0 -I {} rm -f {}
				checkError
			popd > /dev/null
		fi

		# Case 2: Shared library (.SO) plugins
		SO_DIR=$TMP_DIR/$LIB_DIR
		mkdir -p "$SO_DIR"
		checkError

		# Copy .SO into tmp location. Give priority to shared plugins.
		find "$PLUGINS_DIR" \( -name "*.so" -a ! -path "*/shared-*/*.so" \) -print0 | xargs -0 -I {} cp {} "$SO_DIR"
		find "$PLUGINS_DIR" \( -name "*.so" -a -path "*/shared-*/*.so" \) -print0 | xargs -0 -I {} cp {} "$SO_DIR"

		# If .SO files exist, add them to the APK
		ls "$SO_DIR"/*.so
		if [ "0" -eq "$?" ]; then
			echo "Adding .SOs in ($SO_DIR) into $LIB_DIR"

			pushd "$TMP_DIR" > /dev/null
				zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" "$LIB_DIR"/*.so
				checkError
			popd > /dev/null
		fi
	fi

	# We want the new R file always be included
	pushd "$TMP_DIR" > /dev/null
		rsync --ignore-existing -r "$TEMPLATE_DIR/javaFiles/" "$TMP_DIR"
		echo "Creating jar file to merge into dex file"
		jar -cf "$OUTPUT_DIR/classes.jar" ./*

		# Build a list of files that should also be included in the APK, but aren't Java specifically.
		JAR_ASSET_FILES=$(find . -type f \( ! -iname "*.java" -and ! -iname "*.class" -and ! -iname "*.MF" -and ! -iname "*.DS_Store" -and ! -iname "*.orig" \))
		checkError
	popd > /dev/null

	# Create all the needed dex files for the merged jar.
	# Note: The "dx" command line tool is in the same directory as the "aapt" tool.
	DX="$(dirname "$AAPT")"/dx

	# Check for a main dex class list to see if multi-dex builds are supported with this template.
	if [ -f "$TEMPLATE_DIR/main-dex-class-list.txt" ]; then
		# Multi-dex builds are supported. Do a multi-dex supported build!
		DEX_FILES_DIR="$TMP_DIR/dexFiles"
		mkdir "$DEX_FILES_DIR"

		mkdir "$TMP_DIR/coreJars"
		pushd "$TMP_DIR/coreJars" > /dev/null
			# Extract all core classes from core jar files.
			find "$TEMPLATE_DIR/coreJars" -iname \*.jar -print0 | xargs -0 -t -n 1 jar -xf
			checkError

			# Merge all core jar files into single jar file.
			echo "Merging all core JAR files into a single JAR file!"
			jar -cf "$OUTPUT_DIR/coreClasses.jar" ./*
			checkError

			# -@ MULTI-DEX-TEST @-
			# Uncomment this to test calling code in secondary dex files!
			# This will force Corona core code into the main dex file, and plugins will be in secondary dex files.
			# From here, testing the execution of code in other dex file can be done simply by calling plugin functions.
			# The R class will also be in a secondary dex file.
			# Generate a main-dex-list from all classes in the Corona core.
			# jar -tf "$OUTPUT_DIR/coreClasses.jar" | grep .class > "$TMP_DIR/main-dex-class-list-core.txt"
			# echo "Classes for testing main dex file: "
			# cat "$TMP_DIR"/main-dex-class-list-core.txt
			# End of code to uncomment for multi-dex testing
		popd > /dev/null

		# Create all the needed dex files from all jar files.
		# Note: We use the undocumented "set-max-idx-number" option to get around the linearAlloc limitations on 
		# devices older than Android 4. See: https://code.google.com/p/android/issues/detail?id=78035
		# Stock Android 2.3.x devices have a linearAlloc limit of 5MB for loading classes.
		# The Samsung Galaxy SII on the other hand has a 4MB limit!
		# We configure our projects to allow for dex files that are 1/4th the maximum symbol limit. 
		# This gives dex files that are regularly less than 3MB, but a naughty plugin developer could theoretically break this.
		echo "Converting Java bytecode to Android bytecode"
		"$DX" -JXmx2048M --dex $DX_VERBOSITY --output="$DEX_FILES_DIR" --force-jumbo --set-max-idx-number=16384 --multi-dex --main-dex-list="$TEMPLATE_DIR/main-dex-class-list.txt" "$OUTPUT_DIR/coreClasses.jar" "$OUTPUT_DIR/classes.jar" 2> "dx_error.log"
		
		# -@ MULTI-DEX-TEST @-
		# Replace the above DX invocation with this one to test calling code in secondary dex files!
		# "$DX" -JXmx2048M --dex --debug --verbose --output="$DEX_FILES_DIR" --force-jumbo --set-max-idx-number=16384 --multi-dex --main-dex-list="$TMP_DIR/main-dex-class-list-core.txt" --minimal-main-dex "$OUTPUT_DIR/coreClasses.jar" "$OUTPUT_DIR/classes.jar" 2> "dx_error.log"

		if [ $? -ne 0 ] && [[ -s "dx_error.log" ]]; then
			cat "dx_error.log"
			touch "$ERROR_LOG"
			echo "Unhandled error while attempting to create DEX file(s):" >> "$ERROR_LOG"
		 	cat "dx_error.log" >> "$ERROR_LOG"
			exit -1
		fi

		# Copy the all DEX files to the template.apk
		pushd "$DEX_FILES_DIR" > /dev/null
			# Update all dex files
			echo "Update APK with all DEX files."
			zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" ./*.dex
			checkError
		popd > /dev/null
	else
		# Building with only a single dex file is supported for this build template!
		# Merge all JAR files, such as plugins, into a single dex file.
		echo "Merging JAR file with the main DEX file"
		"$DX" -JXmx2048M --dex --debug --verbose --output=classes.dex "$APK_FILE" "$OUTPUT_DIR/classes.jar" 2> "dx_error.log"

		# Special error handling for `dx`, which lumps most exceptions into error code 2.
		if [ $? -ne 0 ] && [[ -s "dx_error.log" ]]; then
			cat "dx_error.log"
			touch "$ERROR_LOG"
			if grep -Fq "com.android.dex.DexIndexOverflowException" "dx_error.log"; then
				# Hit the 65k DEX limit.
				echo "Encountered 65k symbol limit for a single DEX file. Update to at least build 2016.2853 to build a project needing multiple DEX files." >> "$ERROR_LOG"
			else
				echo "Unhandled error while attempting to create DEX file:" >> "$ERROR_LOG"
				cat "dx_error.log" >> "$ERROR_LOG"
			fi
			exit -1
		else
			cat "dx_error.log"

			# Update classes.dex
			zip $ZIP_VERBOSITY "$OUTPUT_DIR/template.apk" classes.dex
			checkError
		fi
	fi

	echo "Adding any non-Java assets back into the APK."
	# Push the TMP_DIR back so that the relative paths contained in $JAR_ASSET_FILES evaulate correctly.
	pushd "$TMP_DIR" > /dev/null
		echo "$JAR_ASSET_FILES" | zip $ZIP_VERBOSITY -r "$OUTPUT_DIR/template.apk" -@
	popd > /dev/null

	# Create output.zip
	zip $ZIP_VERBOSITY -j -D -0 output.zip template.apk copy-files-to-apk.properties
	checkError
popd > /dev/null