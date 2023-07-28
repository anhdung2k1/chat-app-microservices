package com.example.authentication.entity;

import java.io.Serializable;
import java.time.LocalDateTime;

import com.fasterxml.jackson.annotation.JsonFormat;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
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
@Table(name = "MESSAGE")
@Transactional(rollbackOn = Exception.class)
public class MessageEntity implements Serializable{
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "MESS_ID", nullable = false, unique = true)
    @TableGenerator(name = "MESSAGE_GEN",
                    table = "SEQUENCER",
                    pkColumnName = "SEQ_NAME",
                    valueColumnName = "SEQ_COUNT",
                    pkColumnValue = "MESSAGE_SEQ_NEXT_VAL",
                    allocationSize = 1)
    private Long id;
    
    @Column(name = "CONTENT", nullable = false, length = 255)
    @NotBlank(message = "Message content must not be blank")
    private String text;
    
    @OneToOne
    @JoinColumn(name = "USER_ID")
    private UserEntity users;

    @ManyToOne
    @JoinColumn(name = "room_id")
    @EqualsAndHashCode.Exclude
    @ToString.Exclude
    private ChatEntity room;

    @Column(name = "IS_SEEN", nullable = false, columnDefinition = "TINYINT", length=1)
    private Boolean isSeen;

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @Column(name = "CREATE_AT")
    private LocalDateTime createAt;
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @Column(name = "UPDATE_AT")
    private LocalDateTime updateAt;
}
