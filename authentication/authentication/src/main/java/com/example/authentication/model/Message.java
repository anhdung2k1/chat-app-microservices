package com.example.authentication.model;
import
 java.time.LocalDateTime;

import com.example.authentication.entity.ChatEntity;
import com.example.authentication.entity.UserEntity;

import lombok.*;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Message {
    private Long id;
    private String text;
    private UserEntity users;
    private ChatEntity room;
    private Boolean isSeen;
    private LocalDateTime createAt;
    private LocalDateTime updateAt;
}
