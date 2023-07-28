package com.example.authentication.service.interfaces;

import com.example.authentication.model.Chat;
import com.example.authentication.exception.ChatRoomNotFoundException;

import java.util.Dictionary;
import java.util.List;
import java.util.Map;


public interface ChatService {
    Dictionary<String, Object> createRoomByUser(Chat chat, Long user_id) throws Exception;
    String createPrivateRoomByUser(Chat chat, Long user_id_created, Long user_id_joined) throws Exception;
    List<Dictionary<String, Object>> getAllRooms(Long isPrivate);
    List<Dictionary<String, Object>> getAllPrivateRoomJoined(Long user_id);
    Dictionary<String, Object> getPrivateRoomByUser(Long user_id_created, Long user_id_joined, Long isPrivate) throws ChatRoomNotFoundException;
    Dictionary<String, Object> getRoomById(Long id) throws ChatRoomNotFoundException;
    List<Dictionary<String, Object>> getUsersInRoom(Long room_id) throws ChatRoomNotFoundException;
    Map<String, Object> findUserInRoom(Long room_id, String userName) throws ChatRoomNotFoundException;
    Chat updateRoom(Long id, Chat chat, Long user_id) throws ChatRoomNotFoundException;
    boolean deleteRoom(Long id, Long user_id) throws ChatRoomNotFoundException;
}