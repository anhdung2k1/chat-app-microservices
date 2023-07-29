#!/bin/bash
set -x
rm *log
docker ps -a -q --filter ancestor=chat-app-microservices-server | xargs docker rm -f
cd $root
docker compose build server
docker compose run server | tee -a server.log
