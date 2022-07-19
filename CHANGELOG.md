Version History
---------------

### Changes in v0.2.1:

 - Added built in swagger documentation
 - Added new endpoint controllers for messages and spy settings
 - Extendad brand stickerpack with Stella and Tonitrus(https://t.me/addstickers/tgspy)
 - Fixed msvc windows multiple definition error while compilation 
 - Fixed supergroup commands to contain brief description and and options


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
