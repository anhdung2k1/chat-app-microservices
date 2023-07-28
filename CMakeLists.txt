cmake_minimum_required(VERSION 3.10)

project(CBA VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(client_exec
    socket-client/src/main.cpp
    socket-client/src/model/Chat.cpp
    socket-client/src/model/Message.cpp
    socket-client/src/model/User.cpp
    socket-client/src/controller/ChatController.cpp
    socket-client/src/services/ChatService.cpp
    socket-client/src/view/ChatView.cpp
)

target_link_libraries(client_exec 
    pthread
)

add_executable(server_exec
    socket-server/src/main.cpp
    socket-server/src/model/Chat.cpp
    socket-server/src/model/Message.cpp
    socket-server/src/model/User.cpp
    socket-server/src/controller/ChatServerController.cpp
    socket-server/src/services/ChatServerService.cpp
    socket-server/src/view/ChatView.cpp
    socket-server/src/repository/ChatRepository.cpp
    socket-server/src/utils/CurlUtils.cpp
)

target_link_libraries(server_exec
    pthread
    mysqlclient
)

# add_executable(client_test_exec
#     unittest/unittest_GtestClient.cpp 
# )

# add_executable(server_test_exec
#     unittest/unittest_GtestServer.cpp
# )
