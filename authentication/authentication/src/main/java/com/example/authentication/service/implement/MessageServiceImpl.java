package com.example.authentication.service.implement;

import java.time.LocalDateTime;
import java.util.NoSuchElementException;
import java.util.Dictionary;
import java.util.List;
import java.util.*;

import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import com.example.authentication.entity.ChatEntity;
import com.example.authentication.entity.MessageEntity;
import com.example.authentication.entity.UserEntity;
import com.example.authentication.exception.MessageNotFoundException;
import com.example.authentication.model.Message;
import com.example.authentication.repository.ChatRepository;
import com.example.authentication.repository.MessageRepository;
import com.example.authentication.repository.UserRepository;
import com.example.authentication.service.interfaces.MessageService;

import jakarta.transaction.Transactional;
import lombok.RequiredArgsConstructor;

@Service
@Transactional(rollbackOn = Exception.class)
@RequiredArgsConstructor
public class MessageServiceImpl implements MessageService{
    private final MessageRepository messageRepository;
    private final ChatRepository chatRepository;
    private final UserRepository userRepository;

    @Override
    public String createMessage(Message message, Long room_id, Long user_id) throws Exception{
        try {
            MessageEntity messageEntity = new MessageEntity();
            ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get(): null;
            assert chatEntity != null;
            UserEntity userEntity = userRepository.findById(user_id).isPresent() ? userRepository.findById(user_id).get(): null;
            assert userEntity != null;
            message.setRoom(chatEntity);
            message.setUsers(userEntity);
            message.setIsSeen(false);
            message.setUpdateAt(LocalDateTime.now());
            message.setCreateAt(LocalDateTime.now());
            BeanUtils.copyProperties(message, messageEntity);
            messageRepository.save(messageEntity);
            return "Message has been created";
        }
        catch(Exception e) {
            throw new Exception(e.getMessage());
        }
    }

    @Override
    public Dictionary<String, String> getMessageById(Long id, Long room_id) throws MessageNotFoundException {
        try {
            ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get(): null;
            assert chatEntity != null;
            MessageEntity messageEntity = messageRepository.findById(id).isPresent() ? messageRepository.findById(id).get() : null;
            assert messageEntity != null;
            // BeanUtils.copyProperties(messageEntity, message);
            Dictionary<String, String> output = new Hashtable<>();
            output.put("content", messageEntity.getText());
            output.put("send_user", messageEntity.getUsers().getUser_id().toString());
            output.put("room", messageEntity.getRoom().getRoom_id().toString());
            output.put("is_seen", messageEntity.getIsSeen().toString());
            output.put("Create_at", messageEntity.getCreateAt().toString());
            output.put("Update_at", messageEntity.getUpdateAt().toString());
            return output;
        }
        catch (NoSuchElementException e) {
            throw new MessageNotFoundException("Message is not found :%d" + id.toString());
        }
    }

    @Override
    public List<Dictionary<String, Object>> getAllMessages(Long room_id) {
        ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get(): null;
        assert chatEntity != null;
        List<MessageEntity> messageEntities = chatEntity.getMessages();

        // return list of message which just contain: content, user_id, room_id, create_at, update_at (Just for testing)
        List<Dictionary<String, Object>> response = new ArrayList<Dictionary<String, Object>>();
        for (MessageEntity messageEntity: messageEntities) {
            Dictionary<String, Object> message = new Hashtable<>();
            message.put("id", messageEntity.getId());
            message.put("content", messageEntity.getText());
            message.put("userName", messageEntity.getUsers().getUserName());
            message.put("is_seen", messageEntity.getIsSeen().toString());
            response.add(message);
        }

        return response;

    }

    @Override
    public String updateMessage(Long user_id, Long room_id) throws MessageNotFoundException {
        try {
            List<MessageEntity> messageEntities = messageRepository.findAllMessagesByRoomIdAndUserId(room_id,user_id).isPresent() ? messageRepository.findAllMessagesByRoomIdAndUserId(room_id,user_id).get() : null;
            assert messageEntities != null;
            for(MessageEntity messageEntity : messageEntities){
                messageEntity.setIsSeen(true);
                messageEntity.setUpdateAt(LocalDateTime.now());
            }
            return "Message has been updated";
        }
        catch (NoSuchElementException e) {
            throw new MessageNotFoundException("Message is not found in this room:%d" + room_id.toString());
        }
    }

    @Override
    public String deleteMessage(Long user_id, Long room_id, Long message_id) throws MessageNotFoundException {
        try {
            MessageEntity messageEntity = messageRepository.findById(message_id).isPresent() ? messageRepository.findById(message_id).get() : null;
            assert messageEntity != null;
            UserEntity userEntity = userRepository.findById(user_id).isPresent() ? userRepository.findById(user_id).get(): null;
            assert userEntity != null;
            ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get(): null;
            assert chatEntity != null;
            if (user_id == messageEntity.getUsers().getUser_id() && room_id == messageEntity.getRoom().getRoom_id()) {
                messageRepository.delete(messageEntity);
                return "Message has been delete!";
            }
            else {
                if (user_id != messageEntity.getUsers().getUser_id() && room_id == messageEntity.getRoom().getRoom_id()) return "Wrong user id!";
                else if (user_id == messageEntity.getUsers().getUser_id() && room_id != messageEntity.getRoom().getRoom_id()) return "Wrong room id!";
                else return "Wrong user id and room id!";
            }
        }
        catch(NoSuchElementException e) {
            throw new MessageNotFoundException("Message is not found :%d" + message_id.toString());
        }
}
}
