

-- messageText
CREATE TABLE files (
    id_             TEXT            PRIMARY KEY,
    path            TEXT            UNIQUE,
    size            BIGINT          DEFAULT 0,
    date            DATETIME        DEFAULT CURRENT_TIMESTAMP
);

-- messageText
CREATE TABLE messageText (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    text_                                   TEXT,    

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

-- messagePhoto
CREATE TABLE messagePhoto (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    caption_                                TEXT,    
    is_secret_                              BOOLEAN,
    file_id_                                TEXT,
    width                                   INTEGER,
    height                                  INTEGER,

    -- minithumbnail_width                     INTEGER,
    -- minithumbnail_height                    INTEGER,
    -- minithumbnail_data                      BLOB,           -- Image data in jpeg format

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    FOREIGN KEY(file_id_)                   REFERENCES      files(id_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

-- messageVideo
CREATE TABLE messageVideo (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    caption_                                TEXT,    
    is_secret_                              BOOLEAN,
    file_id_                                TEXT,

    duration                                INTEGER,
    width                                   INTEGER,
    height                                  INTEGER,
    mime_type                               TEXT,

    -- minithumbnail_width                     INTEGER,
    -- minithumbnail_height                    INTEGER,
    -- minithumbnail_data                      BLOB,           -- Image data in jpeg format

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    FOREIGN KEY(file_id_)                   REFERENCES      files(id_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

-- messageVoiceNote
CREATE TABLE messageVoiceNote (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    caption_                                TEXT,    
    is_listened                             BOOLEAN,
    file_id_                                TEXT,

    duration                                INTEGER,
    mime_type                               TEXT,

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    FOREIGN KEY(file_id_)                   REFERENCES      files(id_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

-- messageVideoNote
CREATE TABLE messageVideoNote (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    is_secret_                              BOOLEAN,
    is_viewed                               BOOLEAN,
    file_id_                                TEXT,

    duration                                INTEGER,
    length                                  INTEGER,

    -- minithumbnail_width                     INTEGER,
    -- minithumbnail_height                    INTEGER,
    -- minithumbnail_data                      BLOB,           -- Image data in jpeg format

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    FOREIGN KEY(file_id_)                   REFERENCES      files(id_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);

-- messageVideoNote
CREATE TABLE messageDocument (
    id                                      BIGINT,
    chat_id                                 BIGINT,
    version_                                INTEGER,

    caption                                 TEXT,    
    file_id_                                TEXT,
    mime_type                               TEXT,

    FOREIGN KEY(id)                         REFERENCES      modifications(id),
    FOREIGN KEY(chat_id)                    REFERENCES      modifications(chat_id),
    FOREIGN KEY(version_)                   REFERENCES      modifications(version_),
    FOREIGN KEY(file_id_)                   REFERENCES      files(id_),
    PRIMARY KEY(id, chat_id, version_ ASC)
);
