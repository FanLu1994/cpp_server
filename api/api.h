//
// Created by fanlu on 2024/2/21.
//

#ifndef CPP_SERVER_API_H
#define CPP_SERVER_API_H

#include "iostream"

using namespace std;


enum class Status {
    Pending = 0,
    Done = 1
};

struct ToDo {
    int id;
    string content;
    bool status;
    string createTime;
    string updateTime;
    bool isDeleted;
};

void RegisterApi(crow::SimpleApp& app);
void jsonAPi(crow::SimpleApp& app);
// 四个接口：增删改查
void addToDo(crow::SimpleApp& app);
void deleteToDo(crow::SimpleApp& app);
void updateToDo(crow::SimpleApp& app);
void queryToDo(crow::SimpleApp& app);

crow::json::wvalue error(int code, string message);

template <typename T>
crow::json::wvalue success(int code,T data, string message);

crow::json::wvalue success(int code,std::vector<ToDo> todoList, string message);

std::string getCurrentTime();


#endif //CPP_SERVER_API_H
