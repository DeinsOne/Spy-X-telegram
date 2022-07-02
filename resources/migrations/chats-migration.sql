
-- Chats table
CREATE TABLE chats (
    id                              BIGINT,
    title                           TEXT,
    type                            TEXT,
    lastMessageId                   BIGINT,
    fetchSince                      DATETIME        DEFAULT CURRENT_TIMESTAMP,
    fetched                         DATETIME        DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY(id)
);
