#!/usr/bin/env bash
#Install script for git-hooks repo
readonly PROGNAME="${0##*/}"

readonly SCRIPT=$(readlink -f $0)
readonly SCRIPTPATH=`dirname $SCRIPT`

set -o errexit
set -o nounset

error() {
    [ -n "$@" ] && echo "error: $@" >&2
    exit 1
}

install_script() {
    from="$1"
    to="$2"

    if [ -e "${to}" ] && [ $force_install == 0]; then
        while true; do
            echo -n "${to} already exists, overwrite [y/N]: "
                
}