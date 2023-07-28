package com.example.authentication.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Dictionary;
import java.util.List;

import com.example.authentication.exception.MessageNotFoundException;
import com.example.authentication.model.Message;
import com.example.authentication.service.interfaces.MessageService;

@RestController 
@RequestMapping("/api")
public class MessageController {
    @Autowired
    private final MessageService messageService;

    public MessageController(MessageService messageService){
        this.messageService = messageService;
    }

    @PostMapping(value = "user/{user_id}/room/{room_id}/message")
    public ResponseEntity<String> createMessage(@RequestBody Message message, @PathVariable Long room_id, @PathVariable Long user_id) throws Exception{
        return ResponseEntity.ok(messageService.createMessage(message, room_id, user_id));
    }

    @GetMapping(value = "/room/{room_id}/message")
    public ResponseEntity<List<Dictionary<String, Object>>> getAllMessages(@PathVariable("room_id") Long room_id){
        return ResponseEntity.ok(messageService.getAllMessages(room_id));
    }

    @GetMapping(value = "/room/{room_id}/message/{message_id}")
    public ResponseEntity<Dictionary<String, String>> getMessageById(@PathVariable("message_id") Long message_id, @PathVariable("room_id") Long room_id) throws MessageNotFoundException{
        return ResponseEntity.ok(messageService.getMessageById(message_id, room_id));
    }

    @PutMapping(value = "user/{user_id}/room/{room_id}/message")
    public ResponseEntity<String> updateMessage(@PathVariable("user_id") Long user_id, @PathVariable("room_id") Long room_id) throws MessageNotFoundException{
       return ResponseEntity.ok(messageService.updateMessage(user_id, room_id));
    }

    @DeleteMapping(value = "user/{user_id}/room/{room_id}/message/{message_id}")
    public ResponseEntity<String> deleteMessage(@PathVariable("user_id") Long user_id, @PathVariable("room_id") Long room_id, @PathVariable("message_id") Long message_id) throws MessageNotFoundException{
        return ResponseEntity.ok(messageService.deleteMessage(user_id, room_id, message_id));
    }
}
