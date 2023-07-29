#! /bin/bash
# Local
myLocal() {
    export root=$(git rev-parse --show-toplevel)
    echo $root
    #If mysql container is not running --> Create ones and get the IP
    if [[ -n $(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mysql_container) ]] ; then
        export mysqlIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mysql_container)
    else 
        docker compose up mysql-db -d #Enable mysql container
        export mysqlIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mysql_container)
    fi
    echo "MySQL Container IP: $mysqlIp"
    #Check the authentication is running or not
    if [[ -n $(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' authentication) ]]; then
        export authenIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' authentication)
    else 
        docker compose up authentication -d
        export authenIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' authentication)
    fi
    echo "Authentication IP: $authenIp"
    #Check the Server Container is running or not
    serverContainer=$(docker ps -a --format "{{.Names}}" | grep server -m 1)
    if [[ -n $(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' authentication) ]]; then
        export serverIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $serverContainer)
    else 
        docker compose up authentication -d
        export serverIp=$(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $serverContainer)
    fi
    echo "Server IP: $serverIp"
}

myLocal

exec $SHELL -i
