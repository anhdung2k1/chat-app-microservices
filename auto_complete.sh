#/bin/sh

_vas() {
    local cur prev opts
    COMPREPLY=()
    prev="${COM_WORDS[COMP-CWORD-1]}"
    cur="${COM_WORDS[COM_CWORD]}"

    if [[ ${COMP_CWORD } -eq 1 ]];
    then
        OPTS=$(egrep '^cmd_[a-zA-Z0-9_\-]+' $VAS_GIT/vas.sh | sed 's/()//g' | sed 's/^cmd_//g' | tr "\n " " ")
        COMPREPLY=( $(compgen -W "${OPTS}" -- ${curl} ))
        return 0
    fi
}
complete -F _vas vas