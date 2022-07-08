Version History
---------------

### Changes in v0.1.21:

 - Added media controller supergroup handling user's commands.
    - `/setchatgrouptype` - Regulates whether to process all chats and exclude particular, or exlude all and process particular chats only
    - `/setexcludechannels` - Is responsible for setting channels force excluding from parsing
    - `/setexcluded` - Set parsing excluded chats
    - `/setincluded` - Set parsing included chats
    - `/savedeleted` - Is responsible for setting download deleted content parameter
    - `/savesecret` - Is responsible for setting download secret content parameter
    - `/savesize` - Is useful to set maximum media content size such as photos, videos, voice and video notes(messages)
    - `/cancel` - Command to cancel preceding one
    - `/help` - Displays help infroramtion togeter with commands list
 - Added basic spy x telegram services to parse messages, and download media content
 - Created brand spy stickers, which are send to media controller supergroup on creating
 - Added cmake superbuild scripts controlling `oatpp` modules, zlib, openssl, jsoncpp, gperf and td libraries building and installing
