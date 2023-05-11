#!/bin/bash

VERSION="0.2.2"
# target:platform
table=(
    "sitar:R6:F20:F20A:C600"
    "violin:C200S:C500"
    "nuclear:R8"
    "astar:R16"
    "octopus:R58"
    "banjo:MR100:V3:V3S:F35:R11:R7"
    "lute:R7S"
    "boobam:R331"
    "flute:R332"
    "bassoon:R333"
    "cello:G102"
    "azalea:R40"
    "tulip:R18"
    "koto:R30"
    "mandolin:R311"
    "organ:MR133"
    "cowbell:R328S2"
    "drum:R328S3"
    "guitar:MR112"
    "harp:MR115"
)

show_help()
{
    echo -ne "Usage: mkpatch-nand.sh [-p <platform>] [-o <outdir>] [-bh] "
    echo -e "\033[32m<Root of TinaSDK>\033[0m"
    echo -e "Options:"
    echo -e "\t-h: show this help message and exit"
    echo -e "\t-o <outdir>: out directory [default current dir]"
    echo -e "\t-p <platform>: the platform to make, default \$TARGET_PLATFORM"
    echo -e "\t-v: just show driver version and exit"
    echo -e ""
    echo "Eg. ./mkpatch-nand.sh -p r16 ~/worksapce/tina : make nand patch for r16"
}

get_kernel_version()
{
    local list="$(find ${ROOT}/target/allwinner -maxdepth 1 -name "$1-*" -type d)"
    local one ver version platform
    for one in ${list}
    do
        grep -q 'common$' <<< "${one}" && continue
        [ -f "${one}/Makefile" ] || continue
        unset ver
        ver="$(awk -F":=" '/KERNEL_PATCHVER/{print $2}' ${one}/Makefile)"
        if [ -z "${version}" ]; then
            platform="${one}"
            version="${ver}"
        else
            [ "${version}" = "${ver}" ] && continue
            echo -en "\033[31mkernel version diff: "
            echo -en "$(basename ${one}) (${ver}) Vs. "
            echo -e "$(basename ${platform}) (${version})\033[0m"
            [ "$(echo "${ver} > ${version}" | bc)" -eq 1 ] && version="${ver}"
        fi
    done
    kernel="linux-${version}"
}

get_uboot_version()
{
    local mk="${ROOT}/target/allwinner/${target%%-*}-common/BoardConfigCommon.mk"
    uboot=$(awk -F= '/TARGET_UBOOT_VERSION/{print $2}' ${mk} 2>/dev/null)
    [ -n "${uboot}" ] && uboot="u-boot-${uboot}"
}

get_chip()
{
    local fconfig
    fconfig="$(find ${ROOT}/device/config/chips/$1 -maxdepth 4 -name "config-$(awk -F'-' '{print $2}' <<< $2)" 2> /dev/null | head -n 1)"
    chip="$(awk -F'[_=]' '/CONFIG_ARCH_SUN.*P.*=y/{print tolower($3)}' "${fconfig}" | head -n 1)"
}

check_platform()
{
    up_platform="$(sed 's/[a-z]/\u&/g' <<< "${platform}")"
    target=${platform}
    get_kernel_version ${target}
    get_uboot_version
    get_chip ${target} ${kernel}
    # kernel
    case ${kernel} in
        "linux-3.4"|"linux-3.10")
            kbase="drivers/block/nand"
            kdriver="lichee/${kernel}/${kbase}/${chip}"
            ;;
        *)
            kbase="modules/nand"
            kdriver="lichee/${kernel}/${kbase}/${chip}"
            ;;
    esac
    kversion=$(grep '#define *NAND_DRV_VERSION_.*' ${ROOT}/${kdriver}/nfd/nand_osal_for_linux.c \
        | awk '{print $3}' \
        | sed 's/0x0*//;N;s/\n//;s/0x/./')
    if [ "${chip}" == "sun8iw15p1" ]; then
        local subkver=$(grep '#define *NAND_DRV_VERSION_.*' \
            ${ROOT}/${kdriver}-small/nfd/nand_osal_for_linux.c \
            | awk '{print $3}' \
            | sed 's/0x0*//;N;s/\n//;s/0x/./')
        kversion=(${kversion} ${subkver})
    fi
    # uboot
    case ${uboot} in
        *2011*)
            uroot="lichee/brandy/${uboot}"
            udriver="${uroot}/nand_sunxi/${chip%p*}"
            ;;
        *2014*)
            uroot="lichee/brandy/${uboot}"
            udriver="${uroot}/nand_sunxi/${chip}"
            ;;
        *2018*)
            uroot="lichee/brandy-2.0/${uboot}"
            udriver="${uroot}/drivers/sunxi_flash/nand/${chip}"
            ;;
        *)
            echo "invalid uboot: ${uboot}"
            exit 1
            ;;
    esac

    ubase="${ROOT}/${udriver}/osal"
    if [ ! -f "${ubase}/nand_osal_uboot.c" ]; then
        if [ -f "${ubase}/Makefile" ]; then
            #有时候为了多方案共用osal，并不会单独维护一份osal，而是在common中共用
            uboot_common=$(grep -oE 'common[^/]*' ${ubase}/Makefile | head -n 1)
            ubase="${ROOT}/${udriver}/../${uboot_common}/osal"
        else
            # uboot-2018,不再有osal，而是集中到commonX中
            ubase="$(sed 's/[a-z]/\u&/g' <<< ${chip})"
            ubase="$(sed -n '/^config/{h; :loop; n;
                /'${ubase%P*}'/{g; p; b }; /^$/b; b loop }' ${ROOT}/${udriver%/*}/Kconfig)"
            ubase="$(sed -r 's/.*_V([0-9]+)/\1/' <<< "${ubase}")"
            [ -z "${ubase}" ] && ubase=0
            uboot_common="common${ubase}"
            ubase="${ROOT}/${udriver%/*}/${uboot_common}"
        fi
    fi
    uversion=$(grep '#define *NAND_DRV_VERSION_.*' ${ubase}/nand_osal_uboot.c 2>/dev/null \
        | awk '{print $3}' \
        | sed 's/0x0*//;N;s/\n//;s/0x/./')
    if [ -n "$(grep ' ' <<< ${uversion})" ]; then
        local chipnum
        chipnum=$(sed 's/p1//' <<<${chip} | sed 's/[a-z]/\u&/g')
        if [ -n "$(grep ${chipnum} ${ubase}/nand_osal_uboot.c)" ]; then
            uversion=$(sed '/\${chipnum}/{n;s/0x0//;n;s/0x/./}' ${ubase}/nand_osal_uboot.c \
                | grep "define  NAND_DRV_VERSION" | grep -v 0x | awk '{print $3}' | sed 'N;s/\n//')
        else
            uversion=$(awk -F' ' '{print $NF}' <<< $uversion)
        fi
    fi
    if [ "${chip}" == "sun8iw15p1" ]; then
        subuver=$(grep '#define *NAND_DRV_VERSION_.*' \
            ${ubase}/nand_osal_uboot.c 2>/dev/null \
            | awk '{print $3}' \
            | sed 's/0x0*//;N;s/\n//;s/0x/./')
        uversion=(${uversion} ${subuver})
    fi
    echo "===================================================================="
    echo "platform: ${platform}"
    echo "kernel: ${kernel}"
    echo "uboot: ${uboot}"
    echo "chip: ${chip}"
    echo "target: ${target}"
    echo "kernel driver: ${kdriver}"
    echo "kernel driver version: ${kversion[@]}"
    echo "uboot driver: ${udriver}"
    echo "uboot driver version: ${uversion[@]}"
    echo "===================================================================="
    [ -z "${platform}" -o -z "${kernel}" -o -z "${uboot}" -o -z "${chip}" -o -z "${target}" \
        -o -z "${kversion}" -o -z "${uversion}" ] && return 1 || return 0
}

mk_README()
{
cat > ${tmpdir}/README.txt << EOF
注意：
=============================================================
此补丁只适用于更新全志 TinaSDK 或 AndroidSDK 的 nand 驱动
=============================================================

补丁信息：
=============================================================
nand驱动: ${kversion[@]}
适用方案: ${platform} (${kernel} + ${uboot})
补丁提取日期: $(date +%Y-%m-%d\ %H:%M:%S)
=============================================================

文件内容：
=============================================================
patch.tar.gz    : nand 补丁
bin.tar.gz      : Tina nand bin 补丁
README.txt      : 说明文档
md5sum.txt      : 补丁包的md5码
install.sh      : 快速更新脚本
=============================================================

更新步骤：
=============================================================
1. 安装补丁
   ./install.sh <SDK根目录>
       Android SDK: 打补丁及重新编译uboot
       Tina SDK: 打补丁及替换uboot/boot0的bin
   Eg. install.sh ~/workspace/tina : 打补丁+替换uboot/boot0的bin
   Eg. install.sh ~/workspace/android : 打补丁+编译uboot

   Note.
   Android SDK根目录 指全志AndroidSDK中，包含lichee与Android源码
   的目录。目录结构如下：
       android/ (SDK根目录)
       ├── android
       └── lichee

2. 对TinaSDK，如果有定制uboot，需要手动编译uboot
   ./install -u <SDK根目录>
       Android SDK: 步骤1，默认编译uboot，不用执行步骤2
       Tina SDK: 重新编译uboot
   Eg. install.sh -u ~/workspace/tina : 重新编译Tina的uboot

3. 重新编译&打包
   若编译失败，表示补丁更新失败，请提供编译log以进一步分析
=============================================================
EOF
}

mk_md5sum()
{
    ! [ -f "${tmpdir}/patch.tar.gz" ] \
        && echo -e "\033[31mNot Found ${tmpdir}/patch.tar.gz\033[0m" \
        && return 1
    cd ${tmpdir} >/dev/null
    md5sum bin.tar.gz >> ${tmpdir}/md5sum.txt
    md5sum patch.tar.gz > ${tmpdir}/md5sum.txt
}

mk_install()
{
    touch ${tmpdir}/install.sh
    chmod +x ${tmpdir}/install.sh

cat > ${tmpdir}/install.sh << EOF
#!/bin/bash
show_help()
{
    echo -e "Usage: install.sh [-ahpu] \\033[32m<Root of TinaSDK or AndroidSDK>\\033[0m"
    echo -e "Options:"
    echo -e "\\t-a: patch and build uboot [default]"
    echo -e "\\t-h: show this help message and exit"
    echo -e "\\t-p: just patch"
    echo -e "\\t-u: just build uboot"
    echo -e ""
    echo "Eg. ./install.sh ~/worksapce/tina : just patch"
    echo "Eg. ./install.sh -u ~/worksapce/android: just build uboot"
}

check_top()
{
    OBJ_ROOT="\$1"

    [ -f "\${OBJ_ROOT}/android/build/envsetup.sh" -a -d "\${OBJ_ROOT}/lichee" ] \\
        && SDK=android && return 0

    [ -f "\${OBJ_ROOT}/build/envsetup.sh" -a -d "\${OBJ_ROOT}/../lichee" ] \\
        && SDK=android && OBJ_ROOT="\${OBJ_ROOT}/.." && return 0

    [ -f "\${OBJ_ROOT}/build/envsetup.sh" -a -d "\${OBJ_ROOT}/target/allwinner" ] \\
        && SDK=tina && return 0

    echo -e "\\033[31mInvalid Root of TinaSDK or AndroidSDK\\033[0m\n"
    return 1
}

check_md5()
{
    ! which md5sum &>/dev/null \\
        && echo -e "\\033[31mPlease Install md5sum\\033[0m" \\
        && return 1

    ! which tar &>/dev/null \\
        && echo -e "\\033[31mPlease Install tar\\033[0m" \\
        && return 1

    [ ! -f \${cur}/patch.tar.gz ] \\
        && echo -e "\\033[31mNot Found Tina Patch In Current Path\\033[0m" \\
        && return 1

    [ ! -f \${cur}/md5sum.txt ] \\
        && echo -e "\\033[31mNot Found md5sum.txt In Current Path\\033[0m" \\
        && return 1

    ! md5sum -c md5sum.txt &>/dev/null \\
        && echo -e "\\033[31mCheck md5sum Failed\\033[0m" \\
        && return 1

    return 0
}

check()
{
    [ \$# -ne 1 -o ! -d "\$1" ] && show_help && exit 1
    ! check_top "\$1" && show_help && exit 1
    ! check_md5 && echo -e "\\n\\033[31mBad Patch\\033[0m" && exit 1
    return 0
}

uboot()
{
    echo -e "\\033[33mCheck Env\\033[0m"
    if [ ! -d \${OBJ_ROOT}/device/config/chips ]; then
        export TARGET_PLATFORM="\${PLAT}"
    else
        export TARGET_PLATFORM="${target}"
    fi
    chip="${chip}"
    echo -e "\\033[32mCheck Env ... OK\\033[0m"

    # build uboot
    echo -e "\\n\\033[33mBuild Uboot\\033[0m"
    cd \${OBJ_ROOT}/${uroot}
    make distclean
    make \${chip}_config
    make -j4

    # check build
    [ "\$?" -ne 0 ] \\
        && echo -e "\\n\\033[31mBuild Uboot Failed\\033[0m" \\
        && exit 1

    echo -e "\\033[32mBuild Uboot ... OK\\033[0m"
    return 0
}

patch()
{
    ! tar -zmxf \${cur}/patch.tar.gz -C \${OBJ_ROOT} \\
        && echo -e "\\033[31mPatch Failed\\033[0m" \\
        && exit 1
    if [ ! -d \${OBJ_ROOT}/device/config/chips ]; then
        ! tar -zmxf \${cur}/bin.tar.gz -C \${cur} \\
            && echo -e "\\033[31mBackup Failed\\033[0m" \\
            && exit 1
        if [ "\${SDK}" = "tina" -a -d "\${cur}/device" ]; then
            mv \${cur}/device/config/chips/${target}/bin/* \\
                \${OBJ_ROOT}/target/allwinner/\${PLAT}-common/bin
            if [[ \$? == 0 ]]; then
                echo -e "\\033[32mPatch Bin for Tina SDK ... OK\\033[0m"
            else
                if [ "\${SDK}" == "tina" ]; then
                    echo -ne "\\033[31mPatch Bin files Failed. Don't worry. "
                    echo -e "It can be fixed by calling 'muboot' to rebuild uboot!\\033[0m"
                else
                    echo -ne "\\033[31mPatch Bin files Failed. Don't worry. "
                    echo -e "It can be fixed by rebuilding uboot!\\033[0m"
                fi
            fi
        else
            if [ "\${SDK}" == "tina" ]; then
                echo -ne "\\033[31mPatch Bin files Failed. Don't worry. "
                echo -e "It can be fixed by calling 'muboot' to rebuild uboot!\\033[0m"
            else
                echo -ne "\\033[31mPatch Bin files Failed. Don't worry. "
                echo -e "It can be fixed by rebuilding uboot!\\033[0m"
            fi
        fi
    else
        if [ "\${SDK}" = "tina" -a -f "\${cur}/bin.tar.gz" ]; then \\
           ! tar -zmxf \${cur}/bin.tar.gz -C \${OBJ_ROOT} device \\
           && echo -e "\\033[31mPatch Bin files Failed\\033[0m" \\
           && exit 1
           echo -e "\\033[32mPatch Bin for Tina SDK ... OK\\033[0m"
        fi
    fi
    if [ -d \${cur}/device ]; then
        rm -rf \${cur}/device
    fi
$(
if [ "${chip}" == "sun8iw15p1" ]; then
echo "    # fix Makefile for sun8iw15p1"
echo "    sed -i '/PLATFORM_LIBS.*lib-nand/{"
echo "i\\"
echo "ifeq (\$(SOC), sun8iw15p1)\\"
echo "\\"
echo "ifeq (\$(wildcard nand_sunxi/\$(SOC)-small/lib-nand/Makefile),)\\"
echo "PLATFORM_LIBS += nand_sunxi/\$(SOC)-small/libnand-\$(SOC)\\"
echo "endif\\"
echo "\\"
echo "else\\"
echo "\\"
echo "ifeq (\$(wildcard nand_sunxi/\$(SOC)/lib-nand/Makefile),)\\"
echo "PLATFORM_LIBS += nand_sunxi/\$(SOC)/libnand-\$(SOC)\\"
echo "endif\\"
echo "\\"
echo "endif"
echo "d}' \${OBJ_ROOT}/${uroot}/Makefile"
echo ""
fi
)
    echo -e "\\033[32mPatch ... OK\\033[0m"
    return 0
}

clean()
{
    # clean uboot
    rm -rf \${OBJ_ROOT}/${udriver}/lib-nand
    # clean kernel
    rm -rf \${OBJ_ROOT}/${kdriver}/lib
    make -C \${OBJ_ROOT}/lichee/${kernel}/${kbase} clean &>/dev/null
$(if [ "${kernel}" = "linux-3.4" -o "${kernel}" = "linux-3.10" ]; then
    echo "    rm -rf \${OBJ_ROOT}/lichee/${kernel}/modules/nand/${chip}/lib"
    echo "    make -C \${OBJ_ROOT}/lichee/${kernel}/modules/nand clean &>/dev/null"
fi)
}

# update
update()
{
    PLAT="$(sed 's/ /\n/g' <<< "${table[@]}" | grep -w "${up_platform}" | head -n 1 | awk -F: "/${up_platform}/{print \$1}")"
    [ -z "\${PLAT}" ] && PLAT=${target}
    cur=\$(dirname \$0)
    opts=\$(getopt -o 'ahpu' -- \$@) || return 1
    eval set -- "\${opts}"
    while true
    do
        case "\$1" in
            -a)
                actor="clean patch uboot"
                shift
                ;;
            -h)
                show_help
                exit 0
                ;;
            -p)
                actor="clean patch"
                shift
                ;;
            -u)
                actor="clean uboot"
                shift
                ;;
            --)
                shift
                break
        esac
    done

    check \$@
    if [ -z "\${actor}" ]; then
        actor="clean patch"
        [ "\${SDK}" = "android" ] && actor="\${actor} uboot"
    fi

    eval "\$(sed 's/ /\\n/g' <<< \${actor})"
    echo -e "\\033[32m--- END ---\\033[0m"
}

update \$@
EOF
}

mk_patch()
{
    [ ! -d "${ROOT}/${kdriver}" -o ! -d "${ROOT}/${udriver}" ] \
        && echo -e "\033[31mMiss Driver Codes\033[0m" && return 1

    # clean all
    echo -ne "\033[32mCleaning kernel ... \033[0m"
    make -C ${ROOT}/lichee/${kernel} M=${kbase} clean &>/dev/null
    echo -e "\033[32mOK\033[0m"
    echo -ne "\033[32mCleaning uboot ... \033[0m"
    if [ ${uboot} != 'u-boot-2018' ]; then
        make -C ${uroot} M=nand_sunxi clean &>/dev/null
    else
        make -C ${uroot} M=nand_sunxi clean &>/dev/null
    fi
    echo -e "\033[32mOK\033[0m"
    echo

    # kernel
    mkdir -p ${tmpdir}/${kdriver%/*}
    cp -r ${ROOT}/${kdriver} ${tmpdir}/${kdriver}
    if grep "block" <<< ${kdriver} &>/dev/null; then
        # fix for android
        mkdir -p ${tmpdir}/lichee/${kernel}/modules/nand
        cp -r ${ROOT}/${kdriver} ${tmpdir}/lichee/${kernel}/modules/nand
        sed -i '1s/\(obj-\).*\( += nand.o\)/\1m\2/' \
            ${tmpdir}/lichee/${kernel}/modules/nand/${chip}/Makefile
    fi
    #有可能是软连接，因此拷贝的可能只是软链接
    if [ -L "${ROOT}/${kdriver}" ]; then
        kdriver="${kdriver%/*}/$(readlink ${ROOT}/${kdriver})"
        cp -r ${ROOT}/${kdriver} ${tmpdir}/${kdriver}
        if grep "block" <<< ${kdriver} &>/dev/null; then
            # fix for android
            mkdir -p ${tmpdir}/lichee/${kernel}/modules/nand
            cp -r ${ROOT}/${kdriver} ${tmpdir}/lichee/${kernel}/modules/nand
            sed -i '1s/\(obj-\).*\( += nand.o\)/\1m\2/' \
                ${tmpdir}/lichee/${kernel}/modules/nand/${chip}/Makefile
        fi
    fi
    if [ "${chip}" == "sun8iw15p1" ]; then
        cp -r ${ROOT}/${kdriver}-small ${tmpdir}/${kdriver}-small
        cp -r ${ROOT}/${kdriver}-small/../Makefile ${tmpdir}/${kdriver}-small/..
    fi

    # uboot
    mkdir -p ${tmpdir}/${udriver%/*}
    cp -r ${ROOT}/${udriver} ${tmpdir}/${udriver}
    [ -f "${ROOT}/${udriver}/../Makefile" ] &&
        cp -r ${ROOT}/${udriver}/../Makefile ${tmpdir}/${udriver}/..
    [ -f "${ROOT}/${udriver}/../Kconfig" ] &&
        cp -r ${ROOT}/${udriver}/../Kconfig ${tmpdir}/${udriver}/.. 2>/dev/null
    if [ "${chip}" == "sun8iw15p1" ]; then
        mkdir -p ${tmpdir}/${udriver%/*}
        cp -r ${ROOT}/${udriver}-small ${tmpdir}/${udriver}-small
        cp -r ${ROOT}/${udriver}-small/../Makefile ${tmpdir}/${udriver}-small/..
        cp -r ${ROOT}/${udriver}-small/../Kconfig ${tmpdir}/${udriver}-small/.. 2>/dev/null
    fi
    [ -n "${uboot_common}" ] \
        && cp -r ${ROOT}/${udriver}/../${uboot_common} ${tmpdir}/${udriver}/..
    if [[ ${uboot} == "*2014*" ]]; then
        cp -r ${ROOT}/${uroot}/nand_sunxi/Makefile ${tmpdir}/${udriver}/.. \
        && cp -r ${ROOT}/${uroot}/nand_sunxi/nand_interface ${tmpdir}/${udriver}/..
    fi

    # bin
    mkdir -p ${tmpdir}/device/config/chips/${target}/bin
    cp -r ${ROOT}/device/config/chips/${target}/bin/{boot0_nand_sun*.bin,u-boot-sun*.bin} \
        ${tmpdir}/device/config/chips/${target}/bin
    # clean src
    find ${tmpdir} -name "lib-nand" -type d -exec rm -rf {} \; &>/dev/null
    find ${tmpdir} -name "lib" -type d -exec rm -rf {} \; &>/dev/null
    find ${tmpdir} -name "tags" -exec rm -f {} \;
    find ${tmpdir} -name ".*" -exec rm -rf {} \; &>/dev/null

    # pack
    cd ${tmpdir}
    echo -e "\033[33mPatch List\033[0m"
    tar -zcvf bin.tar.gz device
    tar -zcvf patch.tar.gz lichee
}

do_patch()
{
    base="${up_platform}_Update_Nand_To_$(echo ${kversion[@]} | sed 's/ /-/g')_$(date +%Y-%m-%d)"
    tmpdir="${outdir}/${base}"
    outfile="${base}.tar.gz"

    rm -rf ${tmpdir}
    rm -f ${outdir}/${outfile}
    mkdir -p ${tmpdir}

    mk_README && mk_install && mk_patch && mk_md5sum
    [ "$?" -ne 0 ] && exit 1

    echo ""
    rm -rf lichee target device
    cd ${tmpdir}/..
    tar -zcvf ${outdir}/${outfile} ${base} > /dev/null
    ! [ -f "${outdir}/${outfile}" ] \
        && echo -e "\033[32mMake Patch Failed\033[0m" \
        && exit 1

    echo -e "\033[32mMake Patch Finish\033[0m"
    echo -e "Patch at:\n\033[33m  ${outdir}/${outfile}\033[0m"
}

check_top()
{
    ROOT="$(sed 's#/$##' <<< $1)"
    ROOT="${ROOT:-${TINA_BUILD_TOP}}"

    [ -z "${ROOT}" ] \
        && echo -e "\033[31mMiss Root Of SDK\033[0m" \
        && show_help && return 1

    [ ! -f "${ROOT}/build/envsetup.sh" ] \
        && echo -e "\033[31mInvalid Root of Tina SDK\033[0m\n" \
        && return 1
    return 0
}

check()
{
    ! check_top "$1" && return 1

    [ -z "${platform}" ] \
        && echo -e "\033[31mPlease Tell Me The Platform by -p <platform>\033[0m" \
        && return 1
    [ ! -d "${ROOT}/device/config/chips/${platform}" ] \
        && echo -e "\033[31mPlease Enter The Correct Platform\033[0m" \
        && return 1
    ! check_platform \
        && echo -e "\033[31mInvalid Platform: ${platform}\033[0m" \
        && return 1

    echo ""
    return 0
}

show_version()
{
    echo "VERSION for this script: ${VERSION}"
}

mkpatch()
{
    opts=$(getopt -o 'ho:p:v' -- $@) || return 1
    eval set -- "${opts}"
    while true
    do
        case "$1" in
            -h)
                show_help
                exit 0
                ;;
            -p)
                shift
                platform="$1"
                shift
                ;;
            -o)
                shift
                outdir="$(cd $1 && pwd)"
                shift
                ;;
            -v)
                shift
                do_and_exit="show_version"
                ;;
            --)
                shift
                break
        esac
    done
    [ -z "${outdir}" -o "${outdir}" = '.' ] && outdir="${PWD}"
    platform="${platform:-${TARGET_PLATFORM}}"
    check $@ || exit 1

    [ -n "${do_and_exit}" ] && {
        $(sed 's/ /\n/g' <<< ${do_and_exit})
        exit 0
    }

    do_patch

    echo -e "\033[32m--- END ---\033[0m"
}

mkpatch $@
