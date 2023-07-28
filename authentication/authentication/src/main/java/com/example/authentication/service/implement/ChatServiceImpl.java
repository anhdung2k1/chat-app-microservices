package com.example.authentication.service.implement;

import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

import org.springframework.beans.BeanUtils;
import org.springframework.security.core.userdetails.User;
import org.springframework.stereotype.Service;

import com.example.authentication.entity.ChatEntity;
import com.example.authentication.entity.UserEntity;
import com.example.authentication.exception.ChatRoomNotFoundException;
import com.example.authentication.model.Chat;
import com.example.authentication.repository.ChatRepository;
import com.example.authentication.repository.UserRepository;
import com.example.authentication.service.interfaces.ChatService;
import jakarta.transaction.Transactional;
import lombok.*;

@Service
@Transactional(rollbackOn = Exception.class)
@RequiredArgsConstructor
public class ChatServiceImpl implements ChatService {
    private final ChatRepository chatRepository;
    private final UserRepository userRepository;

    @Override
    public Dictionary<String, Object> createRoomByUser(Chat chat, Long user_id) throws Exception {
        try {
            ChatEntity chatEntity = new ChatEntity();
            UserEntity roomAdmin = userRepository.findById(user_id).isPresent() ? userRepository.findById(user_id).get() : null;
            assert roomAdmin != null;
            chat.setRoomAdmin(roomAdmin);
            chat.setIsPrivate(0L);
            chat.setCreateAt(LocalDateTime.now());
            chat.setUpdateAt(LocalDateTime.now());
            BeanUtils.copyProperties(chat, chatEntity);
            chatRepository.save(chatEntity);
            ChatEntity tempChatEntity = chatRepository.findByRoomName(chat.getRoomName()).isPresent() ? chatRepository.findByRoomName(chat.getRoomName()).get() : null;
            chatRepository.insertUserIntoRoom(tempChatEntity.getRoom_id(), user_id);
            Dictionary <String, Object> output = new Hashtable<>();
            output.put("RoomId", chatEntity.getRoom_id());
            output.put("RoomName", chatEntity.getRoomName());
            return output;
        }
        catch (Exception e) {
            throw new Exception(e.getMessage());
        }
    }
    @Override
    public String createPrivateRoomByUser(Chat chat, Long user_id_created, Long user_id_joined) throws Exception{
        try{
            ChatEntity chatEntity = new ChatEntity();
            UserEntity userCreated = userRepository.findById(user_id_created).isPresent() ? userRepository.findById(user_id_created).get() : null;
            assert userCreated != null;
            chat.setRoomAdmin(userCreated);
            chat.setIsPrivate(1L);
            chat.setCreateAt(LocalDateTime.now());
            chat.setUpdateAt(LocalDateTime.now());
            BeanUtils.copyProperties(chat, chatEntity);
            chatRepository.save(chatEntity);
            ChatEntity tempChatEntity = chatRepository.findByRoomName(chat.getRoomName()).isPresent() ? chatRepository.findByRoomName(chat.getRoomName()).get() : null;
            chatRepository.insertUserIntoRoom(tempChatEntity.getRoom_id(), user_id_created);
            chatRepository.insertUserIntoRoom(tempChatEntity.getRoom_id(), user_id_joined);
            return "Private Room has been created";
        }catch(Exception e){
            throw new Exception(e.getMessage());
        }
    }

    @Override
    public List<Dictionary<String, Object>> getAllRooms(Long isPrivate) {
        List<ChatEntity> chatEntities = chatRepository.findAll().stream().filter(
            chatEntity -> chatEntity.getIsPrivate() == isPrivate
        ).collect(Collectors.toList());
        List<Dictionary<String, Object>> listRooms = new ArrayList<Dictionary<String, Object>>();
        for (ChatEntity chatEntity: chatEntities)
        {
            Dictionary<String,Object> room = new Hashtable<>();
            room.put("roomId", chatEntity.getRoom_id());
            room.put("roomName", chatEntity.getRoomName());
            listRooms.add(room);
        }
        return listRooms;
    }
    @Override
    public List<Dictionary<String, Object>> getAllPrivateRoomJoined(Long user_id) {
       List<UserEntity> userEntities = userRepository.findAllPrivateRoomUserJoined(user_id).get();
       List<Dictionary<String,Object>> listRooms = new ArrayList<Dictionary<String,Object>>();
       for(UserEntity userEntity : userEntities)
       {
            ChatEntity privateRoom = chatRepository.findPrivateRoomByRoomAdmin(user_id, userEntity.getUser_id(), 1L).get();
            Long countUnreadMessage = chatRepository.countAllUnreadMessage(user_id, privateRoom.getRoom_id()).get();
            Dictionary<String, Object> user = new Hashtable<>();
            user.put("roomId", userEntity.getUser_id());
            user.put("roomName", userEntity.getUserName());
            user.put("messageCount", countUnreadMessage);
            listRooms.add(user);
       }
       return listRooms;
    }

    @Override
    public Dictionary<String, Object> getPrivateRoomByUser(Long user_id_created, Long user_id_joined, Long isPrivate) throws ChatRoomNotFoundException{
        try{
            ChatEntity chatEntity = chatRepository.findPrivateRoomByRoomAdmin(user_id_created,user_id_joined, isPrivate).get();
            Chat chat = new Chat();
            assert chatEntity != null && chatEntity.getIsPrivate() == 1L;
            BeanUtils.copyProperties(chatEntity, chat);
            Dictionary<String, Object> output = new Hashtable<>();
            output.put("roomId", chatEntity.getRoom_id());
            output.put("roomName", chatEntity.getRoomName());
            output.put("roomAdmin", chatEntity.getRoomAdmin().getUserName());
            return output;
        }catch(NoSuchElementException e){
            throw new ChatRoomNotFoundException("Chat room is not found within user_id:%d" + user_id_created.toString());
        }
    }

    @Override
    public Dictionary<String, Object> getRoomById(Long id) throws ChatRoomNotFoundException {
        try {
            ChatEntity chatEntity = chatRepository.findById(id).isPresent() ? chatRepository.findById(id).get() : null;
            Chat chat = new Chat();
            assert chatEntity != null;
            BeanUtils.copyProperties(chatEntity, chat);
            Dictionary<String, Object> output = new Hashtable<>();
            output.put("roomId", chatEntity.getRoom_id());
            output.put("roomName", chatEntity.getRoomName());
            output.put("roomAdmin", chatEntity.getRoomAdmin().getUserName());
            return output;
        }
        catch (NoSuchElementException e){
            throw new ChatRoomNotFoundException("Chat room is not found :%d" + id.toString());
        }
    }

    @Override
    public List<Dictionary<String, Object>> getUsersInRoom(Long room_id) throws ChatRoomNotFoundException {
        try {
            Integer i = 0;
            ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get() : null;
            assert chatEntity != null;
            Set<UserEntity> membersList = chatEntity.getMembers();
            List<Dictionary<String, Object>> output = new ArrayList<Dictionary<String, Object>>();
            for (UserEntity member: membersList)
            {
                Dictionary<String, Object> memberInfo = new Hashtable<>();
                memberInfo.put("memberId", member.getUser_id());
                memberInfo.put("memberName", member.getUserName());
                output.add(memberInfo);
            }
            return output;
        }
        catch (NoSuchElementException e) {
            throw new ChatRoomNotFoundException("Chat room is not found :%d" + room_id.toString());
        }
    }

    @Override
    public Map<String, Object> findUserInRoom(Long room_id, String userName) throws ChatRoomNotFoundException {
        try {
            ChatEntity chatEntity = chatRepository.findById(room_id).isPresent() ? chatRepository.findById(room_id).get() : null;
            assert chatEntity != null;
            Set<UserEntity> membersList = chatEntity.getMembers();
            Map<String, Object> output = new Hashtable<>();
            for (UserEntity member: membersList)
            {
                if (userRepository.findByUserName(userName).isPresent())
                {
                    if(userRepository.findByUserName(userName).get().getUserName().equals(member.getUserName())){
                        output.put("userName", member.getUserName());
                        // output.put("birthDay", member.getBirth_day());
                        // output.put("gender", member.getGender());
                        output.put("createAt", member.getCreateAt());
                    }
                }
            }
            return output;
        }
        catch (NoSuchElementException e) {
            throw new ChatRoomNotFoundException("Chat room is not found :%d" + room_id.toString());
        }
    }

    @Override
    public Chat updateRoom(Long id, Chat chat, Long user_id) throws ChatRoomNotFoundException {
        try {
            ChatEntity chatEntity = chatRepository.findById(id).isPresent() ? chatRepository.findById(id).get(): null;
            assert chatEntity != null;
            if (user_id == chatEntity.getRoomAdmin().getUser_id())
            {
                chatEntity.setRoomName(chat.getRoomName());
                chatEntity.setUpdateAt(LocalDateTime.now());
                chatRepository.save(chatEntity);
            }
            return chat;
        }
        catch (NoSuchElementException e) {
            throw new ChatRoomNotFoundException("Chat room is not found :%d" + id.toString());
        }
    }

    @Override
    public boolean deleteRoom(Long id, Long user_id) throws ChatRoomNotFoundException {
       try{
            ChatEntity chatEntity = chatRepository.findById(id).isPresent() ? chatRepository.findById(id).get() : null;
            assert chatEntity != null;
            if (user_id == chatEntity.getRoomAdmin().getUser_id()) {
                chatRepository.deleteRoomUser(id, user_id);
                chatRepository.delete(chatEntity);
            }
            return true; 
       }catch(NoSuchElementException e){
            throw new ChatRoomNotFoundException("Chat room is not found :%d" + id.toString());
       }
    }
    
}
