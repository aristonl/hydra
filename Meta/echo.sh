#!/usr/bin/env bash

set -env

ARG0=$0
print_help() {
  NAME=$(basename "$ARG0")
  cat <<EOF
USAGE: $NAME COMMAND [ARCH] [TOOLCHAIN] [ARGS]
  Supported ARCHs: x86_64. Defaults to x86_64 if not set.
  Supported TOOLCHAINs: GNU, Clang. Defaults to GNU if not set.
  Supported COMMANDs:
    build:              Compiles the target binaries
    rebuild-toolchain:  Delete toolchain and rebuild
EOF
}

die() {
  >&2 echo "die: $*"
  exit 1
}

usage() {
  >&2 print_help
  exit1
}

CMD=$1
[ -n "$CMD" ] || usage
shift
if [ "$CMD" = "help" ]; then
    print_help
    exit 0
fi

if [ -n "$1" ]; then
    TARGET="$1"; shift
else
    TARGET="${ECHO_ARCH:-"i686"}"
fi

case "$1" in
    GNU|Clang)
        TOOLCHAIN_TYPE="$1"; shift
        ;;
    *)
        TOOLCHAIN_TYPE="GNU"
        ;;
esac

CMD_ARGS=( "$@" )
CMAKE_ARGS=( "-DECHO_TOOLCHAIN=$TOOLCHAIN_TYPE" )

get_top_dir() {
    git rev-parse --show-toplevel
}

is_valid_target() {
    if [ "$TARGET" = "aarch64" ]; then
        CMAKE_ARGS+=("-DECHO_ARCH=aarch64")
        return 0
    fi
    if [ "$TARGET" = "i686" ]; then
        CMAKE_ARGS+=("-DECHO_ARCH=i686")
        return 0
    fi
    if [ "$TARGET" = "x86_64" ]; then
        CMAKE_ARGS+=("-DECHO_ARCH=x86_64")
        return 0
    fi
    return 1
}

pick_gcc() {
    local BEST_VERSION=0
    local BEST_GCC_CANDIDATE=""
    for GCC_CANDIDATE in gcc gcc-10 gcc-11 gcc-12 /usr/local/bin/gcc-11 /opt/homebrew/bin/gcc-11; do
        if ! command -v $GCC_CANDIDATE >/dev/null 2>&1; then
            continue
        fi
        if $GCC_CANDIDATE --version 2>&1 | grep "Apple clang" >/dev/null; then
            continue
        fi
        if ! $GCC_CANDIDATE -dumpversion >/dev/null 2>&1; then
            continue
        fi
        local VERSION=""
        VERSION="$($GCC_CANDIDATE -dumpversion)"
        local MAJOR_VERSION="${VERSION%%.*}"
        if [ "$MAJOR_VERSION" -gt "$BEST_VERSION" ]; then
            BEST_VERSION=$MAJOR_VERSION
            BEST_GCC_CANDIDATE="$GCC_CANDIDATE"
        fi
    done
    CMAKE_ARGS+=("-DCMAKE_C_COMPILER=$BEST_GCC_CANDIDATE")
    CMAKE_ARGS+=("-DCMAKE_CXX_COMPILER=${BEST_GCC_CANDIDATE/gcc/g++}")
    if [ "$BEST_VERSION" -lt 10 ]; then
        die "Please make sure that GCC version 10.2 or higher is installed."
    fi
}

build_toolchain() {
    echo "build_toolchain: $TOOLCHAIN_DIR"
    if [ "$TOOLCHAIN_TYPE" = "Clang" ]; then
        ( cd "$ECHO_SOURCE_DIR/Toolchain" && ARCH="$TARGET" ./BuildClang.sh )
    else
        ( cd "$ECHO_SOURCE_DIR/Toolchain" && ARCH="$TARGET" ./BuildIt.sh )
    fi
}

ensure_toolchain() {
    [ -d "$TOOLCHAIN_DIR" ] || build_toolchain
}

delete_toolchain() {
    [ ! -d "$TOOLCHAIN_DIR" ] || rm -rf "$TOOLCHAIN_DIR"
}