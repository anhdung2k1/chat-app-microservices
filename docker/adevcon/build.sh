#! /bin/bash

die() {
    echo "ERROR: $*" >&2
    exit 1
}

#Build adevcon docker container
cmd_adevcon() {
    test -n "$http_proxy" && BUILD_ARGS="$BUILD_ARGS --build-arg http_proxy=$http_proxy"
    test -n "$https_proxy" && BUILD_ARGS="$BUILD_ARGS --build-arg https_proxy=$https_proxy"
    
    if test -x /usr/bin/nmcli; then
        dns_server="$(nmcli dev show | grep DNS | awk '{print $2}' | head -n 1)"
    else
        dns_server="$(nslookup 127.0.0.1 | grep Server | awk '{print $2}')"
    fi
    BUILD_ARGS="$BUILD_ARGS --build-arg DNS_SERVER=$dns_server"
    docker build --no-cache -t adevcon $BUILD_ARGS . \
        || die "Failed to build devcon image"
}

#Get the command
cmd=adevcon

while echo "$1" | grep -q '^--'; do
    if echo $1 | grep -q =; then
        o = $(echo "$1" | cut -d= -f1 | sed -e 's,-,_,g')
        v = $(echo "$1" | cut -d= -f2-)
        eval "$o=\"$v\""
    else
        o = $(echo "$1" | sed -e 's,-,_,g')
        eval "$o=yes"
    fi
    shift
done
unset o
long_opts=`set | grep '^__' | cut -d=f1`

#Execute
trap "die Interrupted" INT TERM
cmd $cmd "$@"
status=$?
exit $status