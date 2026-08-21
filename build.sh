#!/bin/bash


function compile() 
{

source ~/.bashrc && source ~/.profile
export LC_ALL=C && export USE_CCACHE=1
ccache -M 100G
export ARCH=arm64
export KBUILD_BUILD_HOST=android-mtk
export KBUILD_BUILD_USER="AbzRaider"
export DEVICE="AGATE"
git clone --depth=1 https://github.com/techyminati/android_prebuilts_clang_host_linux-x86_clang-6443078  clang
git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_aarch64_aarch64-linux-android-4.9 los-4.9-64
git clone --depth=1 https://github.com/LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9 los-4.9-32

if ! [ -d "out" ]; then
	echo "Kernel OUT Directory Not Found . Making Again"
mkdir out

else

	
	sleep 5
	echo "out directory already exists , Making Dirty Build !! "
	echo "If you want to clean Build , just rm -rf out"
	
fi

make O=out ARCH=arm64 agate_defconfig

PATH="${PWD}/clang/bin:${PATH}:${PWD}/clang/bin:${PATH}:${PWD}/clang/bin:${PATH}" \
PATH="${PWD}/clang/bin:${PATH}:${PWD}/los-4.9-32/bin:${PATH}:${PWD}/los-4.9-64/bin:${PATH}" \
make -j$(nproc --all) CC=clang O=out ARCH=arm64 LLVM=1 LLVM_IAS=1 LD=ld.lld AS=llvm-as AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump READELF=llvm-readelf STRIP=llvm-strip CROSS_COMPILE="aarch64-linux-gnu" CROSS_COMPILE_ARM32="arm-linux-gnueabi-"  Image.gz CONFIG_NO_ERROR_ON_MISMATCH=y 2>&1 | tee error.log 

}

function zupload()
{
    rm -rf AnyKernel    
    git clone --depth=1 https://github.com/rio004/AnyKernel3.git AnyKernel
    cp out/arch/arm64/boot/Image.gz AnyKernel
    cd AnyKernel
    zip -r9 4.19.325-Test-OSS-KERNEL-$DEVICE-VIC.zip *
    cd ..
}

compile
zupload
