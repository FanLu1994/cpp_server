//
// Created by fanlu on 2024/3/16.
//

#ifndef CPP_SERVER_FILE_H
#define CPP_SERVER_FILE_H

#include "crow.h"

void RegisterFile(crow::SimpleApp& app);
void uploadApi(crow::SimpleApp& app);
void staticApi(crow::SimpleApp& app);

#endif //CPP_SERVER_FILE_H
