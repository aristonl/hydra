#!/usr/bin/env bash
set -eo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

buildstep "Cleaning Toolchain 1/3" echo "Clearing last toolchain build"
rm -rf "$DIR"/Build
buildstep "Cleaning Toolchain 2/3" echo "Clearing PREFIX directory"
rm -rf "$DIR"/Local
buildstep "Cleaning Toolchain 3/3" echo "Clearing downloaded tarballs"
rm -rf "$DIR"/Tarballs

echo "Cleared Toolchain directory. Ready for rebuild."