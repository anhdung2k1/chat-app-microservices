package com.example.authentication.repository;


import java.util.List;
import java.util.Optional;
import java.util.Set;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import com.example.authentication.entity.ChatEntity;
import jakarta.transaction.Transactional;

@Repository
public interface ChatRepository extends JpaRepository<ChatEntity, Long> {
    @Modifying(clearAutomatically = true)
    @Transactional
    @Query(value = "insert into room_user(room_id, user_id)"
        + "values (:roomId, :userId)"
        ,nativeQuery = true)
    void insertUserIntoRoom(Long roomId,Long userId);
    @Query(value = "delete from room_user where room_id =:roomId and user_id =:userId"
        ,nativeQuery = true)
    void deleteRoomUser(Long roomId,Long userId);
    Optional<ChatEntity> findByRoomName(String roomName);

    @Query(value = "select rm.*"
     + " from room rm"
     + " where rm.is_private =:isPrivate and"
     + " rm.room_id in ( "
     + " select rm1.room_id from room_user rm1"
     + " where rm1.user_id =:user_id_created "
     + " intersect select rm2.room_id from room_user rm2"
     + " where rm2.user_id =:user_id_joined)"
     + " LIMIT 1",
      nativeQuery = true)
    Optional<ChatEntity> findPrivateRoomByRoomAdmin(Long user_id_created,Long user_id_joined, Long isPrivate);

    @Query(value = "select count(ms.mess_id) as count_mess from message ms where ms.user_id=:user_id and ms.room_id=:room_id and ms.is_seen=0", nativeQuery = true)
    Optional<Long> countAllUnreadMessage(Long user_id, Long room_id);
}