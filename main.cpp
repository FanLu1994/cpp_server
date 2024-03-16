#include <iostream>
#include <crow.h>
#include "api/api.h"
//#include "db/SQLiteClient.h"
#include "sqlite3.h"
#include "ws/ws.h"
#include "api/file.h"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    // 初始化数据库
    crow::SimpleApp app;
//    SQLiteClient &sqLiteClient = SQLiteClient::getInstance();

    CROW_ROUTE(app, "/")
    ([]() {
        return "Hello, World!";
    });

    RegisterApi(app);
    RegisterFile(app);
    RegisterWs(app);

    app.loglevel(crow::LogLevel::Info);
    app.port(9000).multithreaded().run();

    return 0;
}
