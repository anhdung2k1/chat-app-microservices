FROM ubuntu:latest

RUN apt-get update
RUN apt install build-essential -y
RUN apt-get install g++ -y
RUN apt-get install nlohmann-json3-dev -y && \
    apt-get install libncurses5-dev libncursesw5-dev -y

WORKDIR /home/socket-client

COPY src/ /home/socket-client

RUN g++ -o client main.cpp controller/ChatController.cpp services/ChatService.cpp view/ChatView.cpp view/StatusBar.cpp model/MessageClient.cpp model/UserClient.cpp model/ChatClient.cpp -pthread -lncursesw

CMD ["./client"]
