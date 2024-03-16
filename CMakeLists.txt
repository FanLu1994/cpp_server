add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
cmake_minimum_required(VERSION 3.25)
project(cpp_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_server main.cpp api/api.cpp db/SQLiteClient.cpp db/SQLiteClient.h util/Singleton.h ws/ws.h ws/ws.cpp api/file.cpp api/file.h)

find_package(Crow CONFIG REQUIRED)
if (Crow_FOUND)
    message("find crow=${Crow_VERSION}")
endif ()
target_link_libraries(cpp_server PRIVATE Crow::Crow)

find_package (SQLite3)
if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries (cpp_server PRIVATE SQLite::SQLite3)
endif (SQLITE3_FOUND)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(cpp_server PRIVATE sqlite_orm::sqlite_orm)
