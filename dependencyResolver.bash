#! /bin/bash
# Usage: dependencyResolver <ABSOLUTE_PATH_TO_BIN> [<ABSOLUTE_PATH_TO_ADDITIONAL_LIB> [<ABSOLUTE_PATH_TO_ADDITIONAL_LIB>]...]
# dependencyResolver /media/Austausch/Projekte/Qt/DBManager/DBManager/bin/DBManager /media/Austausch/Projekte/Qt/DBManager/DBManager/bin/sqldrivers
declare -A array
absPath=`dirname $(pwd)`
defaultLibDirectory="/usr/lib/x86_64-linux-gnu/qt5"

processTargets() {
    for target in "$@";
    do
        if [[ "$0" != "$target" ]]; then
            echo "Process Target $target"
            target=`makePathAbsolute "$target"`
            # if directory
            if [ -d "$target" ]; then
                processTargets `find "$target"/*`
            # else if file
            elif [ -f "$target" ]; then
                resolveDependencies "$target"
            fi
            echo "Done"
        fi
    done
}

makePathAbsolute() {
    if [[ "$1" != /* ]] && [[ -f "$absPath/$1" ]]; then
        echo "$absPath/$1"
    fi
    if [[ "$1" != /* ]]; then
        if [[ -f "$defaultLibDirectory/$1" ]]; then
            echo "$defaultLibDirectory/$1"
        fi
        libInBasePath=`find "$defaultLibDirectory" -name "*$1*"`
        if [[ -f "$libInBasePath" ]]; then
            echo "$libInBasePath"
        fi
    else
        echo "$1"
    fi
}

resolveDependencies() {
    targetToResolve=`makePathAbsolute "$1"`
    for line in `ldd "$targetToResolve" | awk 'NF == 4 {print $3}; NF == 2 {print $1}'`;
    do
        if [[ -z ${array[${1}]} ]] && [[ "$line" = /* ]]; then
            array["$line"]="$line";
            resolveDependencies "$line"
        fi
    done
}

prepareFolders() {
    echo "Prepare Folders"
    if [[ ! -d "./lib" ]]; then
        mkdir "lib"
    fi
    echo "Done"
}

moveFiles() {
    echo "Move Files to ./lib"
    for lib in "${array[@]}";
    do
        if [[ "$lib" = /* ]]; then
            cp "$lib" "./lib/`basename $lib`"
        fi
    done
    echo "Done"
}

main() {
    processTargets "$@"
    prepareFolders
    moveFiles
}

main "$@"
