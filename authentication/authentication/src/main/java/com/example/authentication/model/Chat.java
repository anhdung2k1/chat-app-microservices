package com.example.authentication.model;
import lombok.*;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Set;

import com.example.authentication.entity.MessageEntity;
import com.example.authentication.entity.UserEntity;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Chat {
    private Long room_id;
    private String roomName;
    private UserEntity roomAdmin;
    private List<MessageEntity> messages;
    private Set<UserEntity> members;
    private Long isPrivate;
    private LocalDateTime createAt;
    private LocalDateTime updateAt;
}
