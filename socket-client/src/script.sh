#!/bin/bash
set -x
rm *log
docker ps -a -q --filter ancestor=cba-brainstormers_server | xargs docker rm -f
docker-compose build server
docker-compose run server | tee -a server.log
