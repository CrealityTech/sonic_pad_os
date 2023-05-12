#!/bin/bash

source "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/common.sh"

fix_lang()
{
    local json_file=$1
    [ x"${CONFIG_TINATEST_SYS_GLOBAL_INFO_LANG}" = x"y" ] || return 0;

    if [ x"${CONFIG_TINATEST_SYS_GLOBAL_INFO_LANG_zh}" = x"y" ]; then
        local lang="zh"
    elif [ x"${CONFIG_TINATEST_SYS_GLOBAL_INFO_LANG_en}" = x"y" ]; then
        local lang="en"
    fi

    for testcase in $(get_valid_testcases)
    do
        # pconf: path of private.conf
        local pconf="${top}/testcase${testcase}/private.conf"
        [ ! -f "${pconf}" ] && echo "No Found ${pconf}" && continue

        local name_disp=$(egrep "^name_$lang *=.*$" "${pconf}" | awk -F '=' '{print $2}' | awk '$1=$1')
        [ x"${name_disp}" = x"" ] && continue

        sed -i "s/\(.*\)\"$(basename "${testcase}")\" : {/\0\n\1    \"display_name\" : ${name_disp},/g" "${json_file}"
    done
}

main()
{
    [ ! -f "$1" ] && echo "Not Found $1" && exit 1
    eval "$(grep -vi '="default"' "$1")"
    top="$(get_top)"
    fix_lang "$2"
}
main $@
