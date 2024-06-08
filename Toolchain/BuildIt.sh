#!/usr/bin/env bash
set -eo pipefail
# This file will need to be run in bash, for now.

echo "Hydra Cross-Compiler/Toolchain Build Script"

echo "Building to target: ${TARGET}"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Current Directory: $DIR"
echo "Build Directory: $BUILD"
echo "Sysroot Directory: $SYSROOT"

ARCH=${ARCH:-"x86_64"}
TARGET="$ARCH-hydra"
PREFIX="$DIR/Local/$ARCH"
BUILD="$DIR/Build/$ARCH"
SYSROOT="$BUILD/Root"

MAKE="make"
MD5SUM="md5sum"
REALPATH="realpath"

PATCHPATH="$DIR/Patch"

if command -v ginstall &>/dev/null; then
    INSTALL=ginstall
else
    INSTALL=install
fi

SYSTEM_NAME="$(uname -s)"

if [ "$SYSTEM_NAME" != "Darwin" ]; then
	NPROC="nproc"
else
	NPROC="sysctl -n hw.logicalcpu"
	brew install coreutils && echo "Downloading GNU Coreutils..."
fi

export CFLAGS="-g0 -O2 -mtune=native"
export CXXFLAGS="-g0 -O2 -mtune=native"

BINUTILS_VERSION="2.39"
BINUTILS_MD5SUM="f430dff91bdc8772fcef06ffdc0656ab"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils"

GDB_VERSION="11.2"
GDB_MD5SUM="b5674bef1fbd6beead889f80afa6f269"
GDB_NAME="gdb-$GDB_VERSION"
GDB_PKG="${GDB_NAME}.tar.gz"
GDB_BASE_URL="https://gnu.mirror.constant.com/gdb"

GCC_VERSION="12.2.0"
GCC_MD5SUM="7854cdccc3a7988aa37fb0d0038b8096"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="https://gnu.mirror.constant.com/gcc"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

mkdir -p $BUILD

pushd "$BUILD/"
echo "XXX echo libc and libm headers"
    mkdir -p $BUILD/Root/usr/include/
    SRC_ROOT=$($REALPATH "$DIR"/..)
    FILES=$(find "$SRC_ROOT"/Libraries/LibC/include "$SRC_ROOT"/Libraries/LibM -name '*.h' -print)
    for header in $FILES; do
        target=$(echo "$header" | sed -e "s@$SRC_ROOT/Libraries/LibC/include@@" -e "s@$SRC_ROOT/Libraries/LibM@@")
        buildstep "system_headers" $INSTALL -D "$header" "Root/usr/include/$target"
    done
    unset SRC_ROOT
popd

# === DEPENDENCIES ===
buildstep dependencies echo "Checking whether 'make' is available..."
if ! command -v ${MAKE:-make} >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU Make (for the '${MAKE:-make}' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether 'patch' is available..."
if ! command -v patch >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU patch (for the 'patch' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether your C compiler works..."
if ! ${CC:-cc} -o /dev/null -xc - >/dev/null <<'PROGRAM'
int main() {}
PROGRAM
then
    buildstep dependencies echo "Please make sure to install a working C compiler."
    exit 1
fi

if [ "$SYSTEM_NAME" != "Darwin" ]; then
    for lib in gmp mpc mpfr; do
        buildstep dependencies echo "Checking whether the $lib library and headers are available..."
        if ! ${CC:-cc} -I /usr/local/include -L /usr/local/lib -l$lib -o /dev/null -xc - >/dev/null <<PROGRAM
#include <$lib.h>
int main() {}
PROGRAM
        then
            echo "Please make sure to install the $lib library and headers."
            exit 1
        fi
    done
fi

# === DOWNLOAD AND PATCH ===

# check if the tarballs folder exists
if [ ! -d "$DIR/Tarballs" ]; then
    mkdir -p "$DIR/Tarballs"
else
    buildstep download echo "Tarballs folder exists."
    buildstep download echo "Checking if tarballs are up-to-date..." # TODO: check if tarballs are up-to-date
fi

pushd "$DIR/Tarballs"

    # === BINUTILS ===

    BINUTILS_MD5=""

    if [ -e "$BINUTILS_PKG" ]; then
        md5="$($MD5SUM $BINUTILS_PKG | cut -f1 -d' ')"
        echo "bu md5='$BINUTILS_MD5'"
    fi
    if [ ! -d "$DIR/Tarballs/$BINUTILS_PKG" ] ; then
        if [ ! -e "$BINUTILS_PKG" ]; then
            buildstep download echo "Downloading binutils..."
            curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
        fi

        if [ -d ${BINUTILS_NAME} ]; then
            rm -rf "${BINUTILS_NAME}"
            rm -rf "$DIR/Build/$ARCH/$BINUTILS_NAME"
        fi
    else
        buildstep download echo "binutils already downloaded. Skipping download."
    fi

    buildstep download echo "Extracting binutils..."
    tar -xvf "$BINUTILS_PKG"

    pushd ${BINUTILS_NAME}
    buildstep patching echo "Patching binutils..."
    patch -p1 < "$PATCHPATH/binutils.patch"
    popd

    # === GCC ===

    GCC_MD5=""

    if [ -e "$GCC_PKG" ]; then
        md5="$($MD5SUM ${GCC_PKG} | cut -f1 -d' ')"
        echo "gc md5='$md5'"
    fi
    if [ ! -d "$DIR/Tarballs/$GCC_PKG" ] ; then
        if [ ! -e "$GCC_PKG" ]; then
            buildstep download echo "Downloading GCC..."
            curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"
        fi
        buildstep download echo "Extracting GCC..."
        tar -xvf "$GCC_PKG"
    else
        buildstep download echo "GCC already downloaded. Skipping download."
    fi

    if [ -d ${GCC_NAME} ]; then
        # Drop the previously patched extracted dir
        rm -rf "${GCC_NAME}"
        # Also drop the build dir
        rm -rf "$DIR/Build/$ARCH/$GCC_NAME"
    fi

    echo "Extracting gcc..."
    tar -xzf $GCC_PKG
    pushd $GCC_NAME
    patch -p1 < "$PATCHPATH/gcc.patch"
    popd

    if [ "$SYSTEM_NAME" = "Darwin" ]; then
        pushd "gcc-${GCC_VERSION}"
        ./contrib/download_prerequisites
        popd
    fi
popd

# === COMPILE AND INSTALL ===

rm -rf "$PREFIX"
mkdir -p "$PREFIX"

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

pushd "$BUILD"
    unset PKG_CONFIG_LIBDIR # Just in case

    if [ "$ARCH" = "aarch64" ]; then
        rm -rf gdb
        mkdir -p gdb

        pushd gdb
            echo "XXX configure gdb"
            buildstep "gdb/configure" "$DIR"/Tarballs/$GDB_NAME/configure --prefix="$PREFIX" \
                                                     --target="$TARGET" \
                                                     --with-sysroot="$SYSROOT" \
                                                     --enable-shared \
                                                     --disable-nls \
                                                     ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1
            echo "XXX build gdb"
            buildstep "gdb/build" "$MAKE" -j "$MAKEJOBS" || exit 1
            buildstep "gdb/install" "$MAKE" install || exit 1
        popd
    fi

    rm -rf binutils
    mkdir -p binutils

    pushd binutils
        echo "XXX configure binutils"
        buildstep "binutils/configure" "$DIR"/Tarballs/$BINUTILS_NAME/configure --prefix="$PREFIX" \
                                                 --target="$TARGET" \
                                                 --with-sysroot="$SYSROOT" \
                                                 --disable-nls \
												 --disable-werror
                                                 ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1
        if [ "$SYSTEM_NAME" = "Darwin" ]; then
            # under macOS generated makefiles are not resolving the "intl"
            # dependency properly to allow linking its own copy of
            # libintl when building with --enable-shared.
            buildstep "binutils/build" "$MAKE" -j "$MAKEJOBS" || true
            pushd intl
            buildstep "binutils/build" "$MAKE" all-yes
            popd
        fi
        echo "XXX build binutils"
        buildstep "binutils/build" "$MAKE" -j "$MAKEJOBS" || exit 1
        buildstep "binutils/install" "$MAKE" install || exit 1
    popd

    if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
        perl -pi -e 's/-no-pie/-nopie/g' "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/configure"
    fi

    if [ ! -f "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/hydra-userland.h" ]; then
        cp "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/hydra.h" "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/hydra-kernel.h"
    fi

    rm -rf gcc
    mkdir -p gcc

    pushd gcc
        echo "XXX configure gcc and libgcc"
        buildstep "gcc/configure" "$DIR/Tarballs/gcc-$GCC_VERSION/configure" --prefix="$PREFIX" \
                                            --target="$TARGET" \
                                            --with-sysroot="$SYSROOT" \
                                            --disable-nls \
                                            --enable-languages=c,c++ \
                                            ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1

        echo "XXX build gcc and libgcc"
        buildstep "gcc/build" "$MAKE" -j "$MAKEJOBS" all-gcc || exit 1
        if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
            ln -sf liblto_plugin.so.0.0 gcc/liblto_plugin.so
        fi
        buildstep "libgcc/build" "$MAKE" -j "$MAKEJOBS" all-target-libgcc || exit 1
        echo "XXX install gcc and libgcc"
        buildstep "gcc+libgcc/install" "$MAKE" install-gcc install-target-libgcc || exit 1

        #echo "XXX build libstdc++"
        #buildstep "libstdc++/build" "$MAKE" -j "$MAKEJOBS" all-target-libstdc++-v3 || exit 1
        #echo "XXX install libstdc++"
        #buildstep "libstdc++/install" "$MAKE" install-target-libstdc++-v3 || exit 1
    popd

    if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
        cd "$DIR/Local/${ARCH}/libexec/gcc/$TARGET/$GCC_VERSION" && ln -sf liblto_plugin.so.0.0 liblto_plugin.so
    fi
popd


# == SAVE TO CACHE ==

pushd "$DIR"
    if [ "${TRY_USE_LOCAL_TOOLCHAIN}" = "y" ] ; then
        echo "::endgroup::"
        echo "Building cache tar:"

        rm -f "${CACHED_TOOLCHAIN_ARCHIVE}"  # Just in case

        tar czf "${CACHED_TOOLCHAIN_ARCHIVE}" Local/

        echo "Cache (after):"
        ls -l Cache
    fi
popd
