package com.example.authentication.service.interfaces;

import com.example.authentication.exception.MessageNotFoundException;
import com.example.authentication.model.Message;

import java.util.Dictionary;
import java.util.List;

public interface MessageService {
    String createMessage(Message message, Long room_id, Long user_id) throws Exception;
    List<Dictionary<String, Object>> getAllMessages(Long room_id);
    Dictionary<String, String> getMessageById(Long id, Long room_id) throws MessageNotFoundException;
    // The one who send the message should be the only one can edit
    String updateMessage(Long user_id, Long room_id) throws MessageNotFoundException;
    // The one who write that message should be the only one can delete it
    String deleteMessage(Long user_id, Long room_id, Long message_id) throws MessageNotFoundException;
}