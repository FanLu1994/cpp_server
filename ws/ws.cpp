//
// Created by fanlu on 2024/3/15.
//

#include "ws.h"
#include "iostream"
#include <mutex>
#include "crow.h"
#include <unordered_set>

std::mutex mtx;
std::unordered_set<crow::websocket::connection*> users;

void RegisterWs(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/ws").websocket()
            .onopen([&](crow::websocket::connection &conn) {
                CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
                std::lock_guard<std::mutex> _(mtx);
                users.insert(&conn);
            })
            .onclose([&](crow::websocket::connection &conn, const std::string &reason) {
                CROW_LOG_INFO << "websocket connection closed: " << reason;
                std::lock_guard<std::mutex> _(mtx);
                users.erase(&conn);
            })
            .onmessage([&](crow::websocket::connection & /*conn*/, const std::string &data, bool is_binary) {
                std::lock_guard<std::mutex> _(mtx);
                for (auto u: users)
                    if (is_binary)
                        u->send_binary(data);
                    else
                        u->send_text(data);
            });
}