cmake_minimum_required(VERSION 3.0)
project(phlawd_db_maker)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/libsqlitewrapped.h
        src/main.cpp
        src/SQLiteDBController.cpp
        src/SQLiteDBController.h
        src/GenBankReader.cpp
        src/GenBankReader.h
        src/utils.cpp
        src/utils.h)

#find_library(SQLITEW_LIB sqlitewrapped)
#find_library(SQLITE_LIB sqlite3)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    link_directories(deps/mac)
ELSE()
    link_directories(deps/linux)
    link_directories(/usr/lib/x86_64-linux-gnu)
ENDIF()

add_executable(phlawd_db_maker ${SOURCE_FILES})

target_link_libraries(phlawd_db_maker sqlitewrapped sqlite3)
