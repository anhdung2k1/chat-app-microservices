DOCKERFLAGS="--platform=linux/amd64"

test -n "$VAS_GIT" || export VAS_GIT=$(pwd -P)
test -n "$BUILD_DIR" || export BUILD_DIR="$VAS_GIT/build"

prg=$(basename $0) # vas.sh filename
dir=$(dirname $0); dir=$(cd $dir; pwd) #Get root dir
me=$dir/$prg #Get absolutely path vas.sh
vas=$me

git_commit=$(git --git-dir="$VAS_DIR/.git" rev-parse --short=7 HEAD)
change_id=$(git show $git_commit | grep '^\ Change-Id' | awk '{print $2}')
release=$git_commit

auto_clean() {
    if [ "$AUTO_CLEAN_MODE" = "true" ]; then
        cd "$BUILD_DIR" && rm -rf "$1"
    fi
}

die() {
    echo "ERROR: $*" >&2
    exit 1
}

help() {
    grep '^##' $0 | cut -c3-
    exit 0
}

test -n "$1" || help
echo "$1" | grep -qi "^help\|-h" && help

cmd_make() {
    test -n "$__command" || __command="/bin/sh"
    test -n "$__devcon" || __devcon="adevcon"
    test -n "$__user" || __user=$USER
    test -n "$__uid" || __uid=$(id -u $__user)
    test -n "$__gid" || __gid=$(id -g $__user)
    test -n "$__tz" || __tz="Asia/Ho_Chi_Minh"
    test -n "$__volume" || __volume=$(pwd)
    PARAMS="-v $__volume:/devel"
    cp $VAS_GIT/etc_passwd/$BUILD_DIR
    echo "$__user:x:$__gid:" >> $BUILD_DIR/etc_group
    PARAMS="$PARAMS -v $BUILD_DIR/etc_group:/etc/group"
    PARAMS="$PARAMS -e TZ=$__tz"
    PARAMS="$PARAMS -e VAS_GIT=/devel"
    PARAMS="$PARAMS -u $__uid:$__gid "
    PARAMS="$PARAMS -w /devel"
    test -r ~/.docker && PARAMS="$PARAMS -v $HOME/.docker:/.docker"
    docker run $DOCKERFLAGS --rm $PARAMS $__devcon $__command \
    || die "Devcon emulation failed at command: $__command"
}

cmd_fire() {
    test -n "$DEVCON_DIR" || export DEVCON_DIR="$VAS_GIT/devcon" #Path devcon
    source $VAS/devcon/Envsettings #Source changes .rc
    PARAMS=""
    test -n "$__user" && PARAMS="$PARAMS --user=$__user"
    test -n "$__uid" && PARAMS="$PARAMS --uid=$__uid"
    test -n "$__gid" && PARAMS="$PARAMS --gid=$__gid"
    test -n "$__repo" && PARAMS="$PARAMS --repo=$__repo"
    test -n "$__mem" && __mem=6
    test -n "$__mem" && PARAMS="$PARAMS --mem=$__mem"
    test -n "$__volume" && PARAMS="$PARAMS --volume=$__volume"
    test -n "$__kubeconfig" && PARAMS="$PARAMS --kubeconfig=$__kubeconfig"
    test -n "$__name" && PARAMS="$PARAMS --name $__name"
    devcon fire $PARAMS $@ \
        || die "Failed to start devcon container"
}

cmd_devcon() {
    test -n "$DEVCON_DIR" || export DEVCON_DIR="$VAS_GIT/devcon"
    source $VAS_GIT/devcon/Envsettings
    devcon build $@ \
      || die "Failed to start devcon container"
}
