#!/bin/bash

git_root=$(git rev-parse --show-toplevel)
commit_files=()
mapfile -t commit_files <  <(git show --pretty="format:" --name-only)


RED="\033[0;31m"
YELLOW="\033[0;33m"
NC='\033[0m' # No Color

log(){
    if [[ $1 == "critical" ]]; then
        echo -e "${RED} $2${NC}\n"
        exit 1
    elif [[ $1 == "warning" ]]; then
        echo -e "${YELLOW}$2${NC}\n"
    else
        echo "$@"
    fi
}

for file in "${commit_files[@]}"; do
    if [[ "${file##*.}" == "sh" ]]; then
        path="${git_root%/}/${file}"
        if [[ -f "${path}" ]] && ! shellcheck "${path}"; then
             log warning "========== WARNING ========="
             log critical "Invalid coding style: ${path}"
        fi
    fi
done

