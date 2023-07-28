package com.example.authentication.entity;
import java.io.Serializable;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Set;

import com.fasterxml.jackson.annotation.JsonFormat;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.JoinTable;
import jakarta.persistence.ManyToMany;
import jakarta.persistence.OneToMany;
import jakarta.persistence.OneToOne;
import jakarta.persistence.Table;
import jakarta.persistence.TableGenerator;
import jakarta.transaction.Transactional;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

@Entity
@Data
@Table(name = "Room")
@Transactional(rollbackOn = Exception.class)
public class ChatEntity implements Serializable{
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "ROOM_ID", nullable = false, unique = true)
    @TableGenerator(name = "ROOM_GEN",
                    table = "SEQUENCER",
                    pkColumnName = "SEQ_NAME",
                    valueColumnName = "SEQ_COUNT",
                    pkColumnValue = "ROOM_SEQ_NEXT_VAL",
                    allocationSize = 1)
    private Long room_id;

    @Column(name = "ROOM_NAME", nullable = false, length = 255)
    @NotBlank(message = "Room name must not be blank")
    private String roomName;

    @OneToOne
    @JoinColumn(name = "USER_ID")
    private UserEntity roomAdmin;

    @OneToMany(mappedBy = "room", cascade = CascadeType.ALL)
    @EqualsAndHashCode.Exclude
    @ToString.Exclude
    private List<MessageEntity> messages;

    @ManyToMany(cascade = CascadeType.ALL)
    @JoinTable(name = "room_user",
    joinColumns = @JoinColumn(name = "room_id"),
    inverseJoinColumns = @JoinColumn(name = "user_id"))
    private Set<UserEntity> members;

    @Column(name = "IS_PRIVATE", nullable = false)
    private Long isPrivate;

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @Column(name = "CREATE_AT")
    private LocalDateTime createAt;
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @Column(name = "UPDATE_AT")
    private LocalDateTime updateAt;
}
