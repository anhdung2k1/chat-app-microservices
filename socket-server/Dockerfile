FROM ubuntu:latest

RUN apt-get update && \
    apt install build-essential -y
RUN apt-get install g++ -y && \ 
    apt install libmysqlclient-dev -y && \
    apt-get install nlohmann-json3-dev -y

RUN apt-get install libcurl4-gnutls-dev -y &&\
    apt-get install libcurl4-openssl-dev -y && \
    apt-get install libcurl4-nss-dev -y

RUN apt-get install libboost-all-dev -y


WORKDIR /home/socket-server

COPY src/ /home/socket-server

RUN g++ main.cpp -o server controller/ChatServerController.cpp services/ChatServerService.cpp model/Message.cpp model/User.cpp model/Chat.cpp repository/ChatRepository.cpp utils/CurlUtils.cpp -lcurl -lmysqlclient -lpthread

CMD ["./server"]
