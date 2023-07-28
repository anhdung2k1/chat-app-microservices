#include "controller/ChatServerController.h"
#include "utils/CurlUtils.h"
using namespace std;

int main(int argc, char const* argv[])
{
    ChatServerController::connectToClient();
    return 0;
}