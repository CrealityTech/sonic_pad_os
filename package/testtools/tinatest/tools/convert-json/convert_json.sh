#!/bin/bash

source $( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/common.sh

main()
{
    # check usage
    [ "$#" -ne 1 ] && echo "I Need Only One Argumemt" && exit 1
    [ ! -f "$1" ] && echo "Not Found $1" && exit 1
    eval $(grep -vi '="default"' $1)

    top="$(get_top)"
    treelist=($(get_treelist))
    convert_json 0 ${treelist[@]/#\//@}
}

main $@
