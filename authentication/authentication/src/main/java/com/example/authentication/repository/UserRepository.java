package com.example.authentication.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import com.example.authentication.entity.UserEntity;

@Repository
public interface UserRepository extends JpaRepository<UserEntity, Long>{
    List<UserEntity> findByUserNameContains(String userName);
    
    @Query(value = "Select u.* from users u where u.user_name =:userName",nativeQuery=true)
    Optional<UserEntity> findByUserName(String userName);

    @Query(value = "select us.*"
    + " from users us where us.user_id in"
    + " (select distinct rm.user_id from room_user rm"
    + " where rm.room_id in (select distinct rm1.room_id from room rm1" 
    + " where rm1.is_private=1 intersect select rm2.room_id from room_user rm2"
    + " where rm2.user_id =:user_id and rm2.user_id != us.user_id))"
    ,nativeQuery = true)
    Optional<List<UserEntity>> findAllPrivateRoomUserJoined(Long user_id);
}

