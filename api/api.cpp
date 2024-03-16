//
// Created by fanlu on 2024/2/21.
//
#include "crow.h"
#include "api.h"
#include "ctime"
#include <iostream>
#include <sqlite3.h>
#include "sqlite_orm/sqlite_orm.h"

using namespace sqlite_orm;
using namespace std;


auto storage = make_storage("./data.sqlite",
                            make_table("ToDo",
                                       make_column("ID", &ToDo::id, autoincrement(), primary_key()),
                                       make_column("Content", &ToDo::content),
                                       make_column("Status", &ToDo::status),
                                       make_column("CreateTime", &ToDo::createTime),
                                       make_column("UpdateTime", &ToDo::updateTime),
                                        make_column("IsDeleted", &ToDo::isDeleted)
                            ));

void RegisterApi(crow::SimpleApp& app) {
    jsonAPi(app);
    // 增删改查接口
    storage.sync_schema();     // 创建表
//    storage.remove_all<ToDo>(); // 清空表

    addToDo(app);
    deleteToDo(app);
    updateToDo(app);
    queryToDo(app);

    // 聊天相关接口

    // 文件相关接口

    // 耗时任务模拟接口
}

// 获取当前时区时间的函数
std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
}

// 统一封装，json返回
template <typename T>
crow::json::wvalue success(int code,T data, string message){
    crow::json::wvalue x({{"message", message}});
    x["code"] = code;
    x["data"] = data;
    return x;
}

// 泛型特化
crow::json::wvalue success(int code,std::vector<ToDo> todoList, string message){
    crow::json::wvalue x({{"message", message}});
    x["code"] = code;
    crow::json::wvalue::list resArray;
    for(size_t i=0;i<todoList.size();i++){
        crow::json::wvalue item;
        item["id"] = todoList[i].id;
        item["content"] = todoList[i].content;
        item["status"] = todoList[i].status;
        item["createTime"] = todoList[i].createTime;
        item["updateTime"] = todoList[i].updateTime;
        item["isDeleted"] = todoList[i].isDeleted;
        resArray.push_back(item);
    }
    x["data"] = std::move(resArray);
    return x;
}

crow::json::wvalue error(int code, string message){
    crow::json::wvalue x({{"message", message}});
    x["code"] = code;
    return x;
}


void jsonAPi(crow::SimpleApp& app){
    CROW_ROUTE(app, "/json")
            ([]{
                crow::json::wvalue x({{"message", "Hello, World!"}});
                x["message2"] = "Hello, World.. Again!";
                return x;
            });
}

void addToDo(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/todo_add")
            ([](const crow::request& req){
                // 检查请求参数
                auto content = req.url_params.get("content");
                if(content== nullptr || content == ""){
                    return error(400, "content is empty");
                }

                ToDo todo{-1, content, int(Status::Pending), getCurrentTime(), getCurrentTime()};
                auto id = storage.insert(todo);
                return success(200, id,"新建TODO成功");
            });
}

void deleteToDo(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/todo_delete")
            ([](const crow::request& req){
                // 取出get请求参数 id
                auto id = req.url_params.get("id");
                cout << "id: " << id << endl;
                if(id == nullptr || id == ""){
                    return error(400, "id is empty");
                }
                // 判断id是否存在，并且更新isDeleted字段
                try{
                    auto found_item = storage.get<ToDo>(stoi(id));  // 会抛出not found异常
                    if(found_item.id == stoi(id)){
                        found_item.isDeleted = true;
                        storage.update(found_item);
                        return success(200, id, "删除TODO成功");
                    }else{
                        return error(400, "id not found");
                    }
                }catch (const std::exception& e){
                    cout << e.what() << endl;
                    return error(400, "id not found");
                }
            });
}

void updateToDo(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/todo_update").methods("POST"_method)
            ([](const crow::request& req){
//                crow::json::wvalue x({{"message", "Hello, World!"}});
//                x["message2"] = "Hello, World.. Again!";
                auto x = crow::json::load(req.body);
                if (!x)
                    return error(400, "json parse error");
                // 检查是否存在id参数
                if(x.has("id") == false)
                    return error(400, "id is empty");

                auto id = x["id"].i();

                try {
                    auto found_item = storage.get<ToDo>(id);
                    if(x.has("content") && x["content"]!=""){
                        found_item.content = x["content"].s();
                        found_item.updateTime = getCurrentTime();
                    }
                    if(x.has("status")){
                        found_item.status = x["status"].b();
                        found_item.updateTime = getCurrentTime();
                    }

                    storage.update(found_item);
                    return success(200, id, "更新TODO成功");
                }catch (const std::exception& e){
                    cout << e.what() << endl;
                    return error(400, "id not found");
                }
            });
}

void queryToDo(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/todo_query")
            ([](const crow::request& req){
                crow::json::wvalue x({{"message", "Hello, World!"}});
                x["message2"] = "Hello, World.. Again!";
                auto isFinish = req.url_params.get("is_finish");
                if(isFinish == nullptr || isFinish == ""){
                    auto all = storage.get_all<ToDo>();

                    return success(200, all, "查询TODO成功");
                }else{
                    auto all = storage.get_all<ToDo>(where(c(&ToDo::status) == (isFinish == "true" ? 1 : 0)));
                    return success(200, all, "查询TODO成功");
                }
            });
}
