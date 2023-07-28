package com.example.authentication.controller;

// function in Springframework
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.Dictionary;
import java.util.List;
import java.util.Map;
// import file from authentication folder
import com.example.authentication.exception.ChatRoomNotFoundException;
import com.example.authentication.model.Chat;
import com.example.authentication.service.interfaces.ChatService;

@RestController
@RequestMapping("/api")

public class ChatController {
    @Autowired
    private final ChatService chatService;

    public ChatController(ChatService chatService) {
        this.chatService = chatService;
    }

    // Create room and the user who create it will be the room admin
    @PostMapping(value = "/user/{user_id}/room")
    public ResponseEntity<Dictionary<String, Object>> createRoomByUser(@RequestBody Chat chat, @PathVariable("user_id") Long user_id) throws Exception {
        return ResponseEntity.ok(chatService.createRoomByUser(chat, user_id));
    }
    @PostMapping(value = "/useradmin/{user_id_created}/user/{user_id_joined}/room")
    public ResponseEntity<String> createPrivateRoomByUser(@RequestBody Chat chat, @PathVariable("user_id_created") Long user_id_created, @PathVariable("user_id_joined") Long user_id_joined) throws Exception{
        return ResponseEntity.ok(chatService.createPrivateRoomByUser(chat, user_id_created, user_id_joined));
    }
    @GetMapping(value = "/useradmin/{user_id_created}/user/{user_id_joined}/room")
    public ResponseEntity<Dictionary<String,Object>> getPrivateRoomByUser(@PathVariable("user_id_created") Long user_id_created, @PathVariable("user_id_joined") Long user_id_joined, @RequestParam("isPrivate") Long isPrivate) throws ChatRoomNotFoundException{
        return ResponseEntity.ok(chatService.getPrivateRoomByUser(user_id_created, user_id_joined, isPrivate));
    }
    @GetMapping(value = "/room")
    public ResponseEntity<List<Dictionary<String, Object>>> getAllRooms(@RequestParam("isPrivate") Long isPrivate) {
        return ResponseEntity.ok(chatService.getAllRooms(isPrivate));
    }
    @GetMapping(value = "/user/{user_id}/room")
    public ResponseEntity<List<Dictionary<String,Object>>> getAllPrivateRoomJoined(@PathVariable("user_id") Long user_id){
        return ResponseEntity.ok(chatService.getAllPrivateRoomJoined(user_id));
    }

    @GetMapping(value = "/room/{room_id}")
    public ResponseEntity<Dictionary<String, Object>> getRoomById(@PathVariable("room_id") Long room_id) throws ChatRoomNotFoundException {
        return ResponseEntity.ok(chatService.getRoomById(room_id));
    }

    @GetMapping(value = "/room/{room_id}/members")
    public ResponseEntity<List<Dictionary<String, Object>>> getUsersInRoom(@PathVariable("room_id") Long room_id) throws ChatRoomNotFoundException {
        return ResponseEntity.ok(chatService.getUsersInRoom(room_id));
    }

    @GetMapping(value = "/room/{room_id}/members/search")
    public ResponseEntity<Map<String, Object>> findUserInRoom(@PathVariable("room_id") Long room_id, @RequestParam("userName") String userName) throws ChatRoomNotFoundException {
        return ResponseEntity.ok(chatService.findUserInRoom(room_id, userName));
    }

    @PutMapping(value = "user/{user_id}/room/{room_id}/update")
    public ResponseEntity<Chat> updateRoom(@PathVariable("room_id") Long room_id, @RequestBody Chat chat, @PathVariable("user_id") Long user_id) throws ChatRoomNotFoundException {
        return ResponseEntity.ok(chatService.updateRoom(room_id, chat, user_id));
    }

    @DeleteMapping(value = "user/{user_id}/room/{room_id}")
    public ResponseEntity<Boolean> deleteRoom (@PathVariable("room_id") Long room_id, @PathVariable("user_id") Long user_id) throws ChatRoomNotFoundException {
        return ResponseEntity.ok(chatService.deleteRoom(room_id, user_id));
    }
}