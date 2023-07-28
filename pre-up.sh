#! /bin/bash

export root=$(git rev-parse --show-toplevel)
echo $root
export mysqlIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mysql_container)
echo $mysqlIp
exec $SHELL -i
