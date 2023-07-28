package com.example.authentication.repository;

import java.util.Optional;
import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import com.example.authentication.entity.MessageEntity;

@Repository
public interface MessageRepository extends JpaRepository<MessageEntity, Long> {
    @Query(value="select ms.* from message ms where ms.room_id=:roomId and ms.user_id=:userId",nativeQuery = true)
    Optional<List<MessageEntity>>findAllMessagesByRoomIdAndUserId(Long roomId, Long userId);
}