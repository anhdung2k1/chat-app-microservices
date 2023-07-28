package com.example.authentication.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(code = HttpStatus.NOT_FOUND, reason = "Chat room is not found")
public class ChatRoomNotFoundException  extends Exception{
    public ChatRoomNotFoundException(String message){
        super(message);
    }
}