package com.example.authentication.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(code = HttpStatus.NOT_FOUND, reason = "Message is not found")
public class MessageNotFoundException  extends Exception{
    public MessageNotFoundException(String message){
        super(message);
    }
}