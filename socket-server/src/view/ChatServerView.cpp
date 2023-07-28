#include"ChatServerView.h"

using std::endl;
using std::cout;

void ChatServerView::displayMessage(Message message) {
   cout << "Sender: " << message.getSender().getName() << " Message: " << message.getText() << endl;
}
void ChatServerView::displayChat(Chat chat) {
   vector<Message> messages = chat.getMessage();
   for(auto& message : messages) {
      displayMessage(message);
   }
}
