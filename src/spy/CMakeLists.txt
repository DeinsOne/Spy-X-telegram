
find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(oatpp-sqlite   1.3.0 REQUIRED)
find_package(cxxopts REQUIRED)
find_package(Threads REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(tgspy
  TelegramSpy.cpp
  auth/ConfigAuth.cpp
  auth/SpyAuth.cpp
  db/MessagesDatabase/MessagesDatabase.cpp
  service/SpyService.cpp
  service/functions/DownloadFile/DownloadFile.cpp
  service/functions/SendMessage/SendMessage.cpp
  utils/StringTools.cpp
  utils/Logger/Logger.cpp
  controller/ErrorHandler.cpp
  service/controller/ChatsController/ChatsController.cpp
  service/controller/SettingsController/SpySettingsController.cpp
  service/controller/NewContentController/NewContentController.cpp
  service/controller/DeletedContentController/DeletedContentController.cpp
  service/controller/DeletedContentChatController/DeletedContentChatController.cpp
  service/controller/DeletedContentChatController/Command/CommandHandler.cpp
  service/controller/DeletedContentChatController/Command/HelpCommand/HelpCommand.cpp
  service/controller/DeletedContentChatController/Command/SetChatGroupTypeCommand/SetChatGroupTypeCommand.cpp
  service/controller/DeletedContentChatController/Command/SetExcludeChannelsCommand/SetExcludeChannelsCommand.cpp
  service/controller/DeletedContentChatController/Command/SaveDeletedCommand/SaveDeletedCommand.cpp
  service/controller/DeletedContentChatController/Command/SaveSecretCommand/SaveSecretCommand.cpp
  service/controller/DeletedContentChatController/Command/SaveSizeCommand/SaveSizeCommand.cpp
  service/contentWorker/ContentWorker.cpp
)
target_link_libraries(tgspy
  PUBLIC tdlpp
  PUBLIC Threads::Threads
  PRIVATE oatpp::oatpp
  PRIVATE oatpp::oatpp-swagger
  PRIVATE oatpp::oatpp-sqlite
  PRIVATE cxxopts::cxxopts
  PRIVATE stdc++fs
  PRIVATE jsoncpp_static
)
target_include_directories(tgspy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)

target_compile_definitions(tgspy
  ## define path to swagger-ui static resources folder
  PRIVATE -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

  ## Path to database migration scripts
  PRIVATE -DDATABASE_MIGRATIONS="resources/migrations/"

  ## Folder to store spy's data
  PRIVATE -DSPY_DATA_FOLDER="spydata/"
)

if (${SPY_ENABLE_OPTIMIZATION})
  target_compile_options(tgspy PRIVATE -O3)
endif()


install(TARGETS tgspy DESTINATION ${CMAKE_INSTALL_PREFIX})
if (${SPY_PACKAGED_BUILD})
  set_target_properties(tgspy PROPERTIES INSTALL_RPATH bin)
endif()
