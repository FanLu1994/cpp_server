//
// Created by fanlu on 2024/2/25.
//

#ifndef CPP_SERVER_SQLITECLIENT_H
#define CPP_SERVER_SQLITECLIENT_H

#include "sqlite3.h"
#include "sqlite_orm/sqlite_orm.h"
#include <iostream>

using namespace sqlite_orm;
using namespace std;



class SQLiteClient{
private:
    SQLiteClient() {
        int result;
        result = sqlite3_open("./data.db", &db);
        if (result) {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // 创建一个TODO表，包含id, content, status, create_time, update_time
        const char* createToDoTableSQL = "CREATE TABLE IF NOT EXISTS ToDo (ID INTEGER PRIMARY KEY, Content TEXT, Status INTEGER, CreateTime TEXT, UpdateTime TEXT);";
        auto errMsg = new char;
        result = sqlite3_exec(db, createToDoTableSQL, nullptr, nullptr, &errMsg);
        if (result != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "ToDo Table created successfully." << std::endl;
        }
    }
    ~SQLiteClient() {
        if(db)
            sqlite3_close(db);
    }
    sqlite3* db;
    static SQLiteClient* instance;
public:

    static SQLiteClient& getInstance() {
        if(!instance)
            instance = new SQLiteClient();
        static SQLiteClient instance;
        return instance;
    }

    sqlite3* getDB() {
        return db;
    }
};


#endif //CPP_SERVER_SQLITECLIENT_H
