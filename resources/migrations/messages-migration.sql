
-- Base messages table
CREATE TABLE messages (
    id                              BIGINT,
    sender_id                       BIGINT,
    chat_id                         BIGINT,
    media_album_id                  BIGINT      DEFAULT 0,
    can_be_deleted_for_all          BOOLEAN,
    PRIMARY KEY(id, chat_id)
);


-- Messges modifications table
CREATE TABLE modifications (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,
    message_content                         TEXT,
    date_                                   DATETIME        DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(id)                         REFERENCES      messages(id),
    FOREIGN KEY(chat_id)                    REFERENCES      messages(chat_id),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

