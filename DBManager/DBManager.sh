#!/bin/sh
BASE_DIR=$(dirname "$(readlink -f "$0")")
export LD_LIBRARY_PATH="$BASE_DIR"/lib/:"$BASE_DIR":$LD_LIBRARY_PATH
export QML_IMPORT_PATH="$BASE_DIR"/qml/:QML_IMPORT_PATH
export QML2_IMPORT_PATH="$BASE_DIR"/qml/:QML2_IMPORT_PATH
export QT_PLUGIN_PATH="$BASE_DIR"/plugins/:QT_PLUGIN_PATH
export QTWEBENGINEPROCESS_PATH="$BASE_DIR"/bin/QtWebEngineProcess
export QTDIR="$BASE_DIR"
export QT_QPA_PLATFORM_PLUGIN_PATH="$BASE_DIR"/plugins/platforms:QT_QPA_PLATFORM_PLUGIN_PATH

absPath=$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )
updatePath="${absPath}/update/";

#echo "Absolute Path: $absPath"
#echo "Update Path: $updatePath"

moveUpdateFiles() {
	for file in $( find "${updatePath}" ); do
		if [ $file != $updatePath ] && [ -f $file ]; then
#			dirName=`dirname $file`
#			fileName=`basename $file`
#			relativeFilePath=`realpath --relative-to=$updatePath $file`
#			relativeDirPath=`realpath --relative-to=$updatePath $dirName`
#			echo "Relative Path: $relativeFilePath"
#			echo "Dirname: $dirName"
#			echo "Relative Dirname: $relativeDirPath"
#			echo "Basename: $fileName"
			
#			if [ ! -d "${absPath}/${relativeDirPath}" ]; then
#				mkdir -r "${absPath}/${relativeDirPath}"
#			fi
#			echo "Verschiebe nach $absPath/$relativeDirPath"
#			echo "Verschoben werden würde $absPath/$relativeFilePath"
			
#			mv "${file}" "${absPath}/${relativeFilePath}"
			tar xfvz "${file}" --directory "${absPath}"
		fi
	done
	
	rm -rf "${updatePath}"
}

if [ -d "${updatePath}" ] && [ ! -z "$(ls -A "${updatePath}")" ]; then
	moveUpdateFiles
fi


"$BASE_DIR"/bin/DBManager "$@"
