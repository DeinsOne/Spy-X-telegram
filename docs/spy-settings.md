# Spy settings via SpySettingsController

| Name | Type | Default value | Values list | Description |
|------|------|---------------|-------------|----|
| Include chats | array\<chats\> | all | all/chats | List of chats to be processed on spy lifetime |
| Exclude chats | array\<chats\> | none | none/all/chats | List of chats to be excluded from spy processing |
| Force exclude channels | bool | false | true/false | Regulates whether spy should skip channels from spy processing |
| Save deleted media | bool | true | true/false | Responsible for saving photos, videos, voice and video notes(messages). Spy will download all matching content since activating this param  |
| Saved media size limit | int | 50mb | any | Maximum media file size in megabytes to be downloaded |
| Save secret content | bool | true | true/false | Param to enable downloading secret media or media with ttl(time to live) |
