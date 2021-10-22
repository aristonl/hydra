#!/usr/bin/env bash
set -eo pipefail
# This file will need to be run in bash, for now.

echo "Echo Cross-Compiler/Toolchain Build Script"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "$DIR"

ARCH=${ARCH:-"x86_64"}
TARGET="$ARCH-pc-echo"
PREFIX="$DIR/Local/$ARCH"
BUILD="../Build/$ARCH"
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

BINUTILS_VERSION="2.33.1"
BINUTILS_MD5SUM="1e55743d73c100b7a0d67ffb32398cdb"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils"

GDB_VERSION="10.2"
GDB_MD5SUM="7aeb896762924ae9a2ec59525088bada"
GDB_NAME="gdb-$GDB_VERSION"
GDB_PKG="${GDB_NAME}.tar.gz"
GDB_BASE_URL="https://ftp.gnu.org/gnu/gdb"

GCC_VERSION="10.3.0"
GCC_MD5SUM="dc6886bd44bb49e2d3d662aed9729278"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="https://ftp.gnu.org/gnu/gcc"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

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

mkdir "$DIR/Tarballs"
pushd "$DIR/Tarballs"
    # Build aarch64-gdb for cross-debugging support on x86 systems
    if [ "$ARCH" = "aarch64" ]; then
        md5=""
        if [ -e "$GDB_PKG" ]; then
            md5="$($MD5SUM $GDB_PKG | cut -f1 -d' ')"
            echo "bu md5='$md5'"
        fi
        if [ "$md5" != ${GDB_MD5SUM} ] ; then
            rm -f $GDB_PKG
            curl -LO "$GDB_BASE_URL/$GDB_PKG"
        else
            echo "Skipped downloading gdb"
        fi
    fi

    md5=""
    if [ -e "$BINUTILS_PKG" ]; then
        md5="$($MD5SUM $BINUTILS_PKG | cut -f1 -d' ')"
        echo "bu md5='$md5'"
    fi
    if [ "$md5" != ${BINUTILS_MD5SUM} ] ; then
        rm -f $BINUTILS_PKG
        curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
    else
        echo "Skipped downloading binutils"
    fi

    md5=""
    if [ -e "$GCC_PKG" ]; then
        md5="$($MD5SUM ${GCC_PKG} | cut -f1 -d' ')"
        echo "gc md5='$md5'"
    fi
    if [ "$md5" != ${GCC_MD5SUM} ] ; then
        rm -f $GCC_PKG
        curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"
    else
        echo "Skipped downloading gcc"
    fi

    if [ "$ARCH" = "aarch64" ]; then
        if [ -d ${GDB_NAME} ]; then
            rm -rf "${GDB_NAME}"
            rm -rf "$DIR/Build/$ARCH/$GDB_NAME"
        fi
        echo "Extracting GDB..."
        tar -xzf ${GDB_PKG}

        pushd ${GDB_NAME}

        popd
    fi

    if [ -d ${BINUTILS_NAME} ]; then
        rm -rf "${BINUTILS_NAME}"
        rm -rf "$DIR/Build/$ARCH/$BINUTILS_NAME"
    fi
    echo "Extracting binutils..."
    tar -xzf ${BINUTILS_PKG}

    pushd ${BINUTILS_NAME}
    echo "Patching binutils..."
    patch -p1 < "$PATCHPATH/binutils.patch"
    popd

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

mkdir -p "$DIR/Build/$ARCH"

pushd "$DIR/Build/$ARCH"
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
                                                 --enable-shared \
                                                 --disable-nls \
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

    echo "XXX echo libc, libm and libpthread headers"
    mkdir -p "$BUILD"
    pushd "$BUILD"
        mkdir -p Root/usr/include/
        SRC_ROOT=$($REALPATH "$DIR"/..)
        FILES=$(find "$SRC_ROOT"/Libraries/LibC "$SRC_ROOT"/Libraries/LibM "$SRC_ROOT"/Libraries/LibPthread -name '*.h' -print)
        for header in $FILES; do
            target=$(echo "$header" | sed -e "s@$SRC_ROOT/Libraries/LibC@@" -e "s@$SRC_ROOT/Libraries/LibM@@" -e "s@$SRC_ROOT/Libraries/LibPthread@@")
            buildstep "system_headers" $INSTALL -D "$header" "Root/usr/include/$target"
        done
        unset SRC_ROOT
    popd

    if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
        perl -pi -e 's/-no-pie/-nopie/g' "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/configure"
    fi

    if [ ! -f "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/echo-userland.h" ]; then
        cp "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/echo.h" "$DIR/Tarballs/gcc-$GCC_VERSION/gcc/config/echo-kernel.h"
    fi

    rm -rf gcc
    mkdir -p gcc

    pushd gcc
        echo "XXX configure gcc and libgcc"
        buildstep "gcc/configure" "$DIR/Tarballs/gcc-$GCC_VERSION/configure" --prefix="$PREFIX" \
                                            --target="$TARGET" \
                                            --with-sysroot="$SYSROOT" \
                                            --disable-nls \
                                            --with-newlib \
                                            --enable-shared \
                                            --enable-languages=c,c++ \
                                            --enable-default-pie \
                                            --enable-lto \
                                            --enable-threads=posix \
                                            ${TRY_USE_LOCAL_TOOLCHAIN:+"--quiet"} || exit 1

        echo "XXX build gcc and libgcc"
        buildstep "gcc/build" "$MAKE" -j "$MAKEJOBS" all-gcc || exit 1
        if [ "$SYSTEM_NAME" = "OpenBSD" ]; then
            ln -sf liblto_plugin.so.0.0 gcc/liblto_plugin.so
        fi
        buildstep "libgcc/build" "$MAKE" -j "$MAKEJOBS" all-target-libgcc || exit 1
        echo "XXX install gcc and libgcc"
        buildstep "gcc+libgcc/install" "$MAKE" install-gcc install-target-libgcc || exit 1

        echo "XXX build libstdc++"
        buildstep "libstdc++/build" "$MAKE" -j "$MAKEJOBS" all-target-libstdc++-v3 || exit 1
        echo "XXX install libstdc++"
        buildstep "libstdc++/install" "$MAKE" install-target-libstdc++-v3 || exit 1
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
