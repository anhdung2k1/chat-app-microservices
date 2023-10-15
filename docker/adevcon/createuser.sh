#!/bin/bash

user=$1
uid=$2
gid=$3

addgroup -g $gid chatapp
adduser -s /bin/bash -g $gid -u $uid $user
adduser $user wheel
adduser $user docker
echo $user:$user | chpasswd