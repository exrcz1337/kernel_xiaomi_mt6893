#!/bin/bash

function compile() 
{
    source ~/.bashrc && source ~/.profile
    export LC_ALL=C && export USE_CCACHE=1
    ccache -M 100G
    export ARCH=arm64
    export KBUILD_BUILD_HOST=buildbot
    export KBUILD_BUILD_USER="exerczz1"
    export DEVICE="AGATE"

    mkdir -p out

    make LLVM=-18 O=out ARCH=arm64 LLVM_IAS=1 CROSS_COMPILE="aarch64-linux-gnu-" agate_defconfig
    make -j$(nproc --all) LLVM=-18 O=out ARCH=arm64 LLVM_IAS=1 \
        CROSS_COMPILE="aarch64-linux-gnu-" \
        CROSS_COMPILE_ARM32="arm-linux-gnueabi-" \
        HOSTLDFLAGS="-no-pie" \
        Image.gz CONFIG_NO_ERROR_ON_MISMATCH=y 2>&1 | tee error.log
}

function zupload()
{
    if ! [ -d "AnyKernel" ]; then
        git clone --depth=1 https://github.com/rio004/AnyKernel3.git AnyKernel
    fi
    cp out/arch/arm64/boot/Image.gz AnyKernel/
    cd AnyKernel
    zip -r9 4.19.325-Clang18-KERNEL-$DEVICE.zip * -x .git .github README.md "*.zip"
    cd ..
}

compile
zupload
