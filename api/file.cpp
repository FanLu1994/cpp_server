//
// Created by fanlu on 2024/3/16.
//

#include "file.h"
#include "crow.h"
#include "iostream"
#include "api.h"

using namespace std;

void RegisterFile(crow::SimpleApp& app) {
    uploadApi(app);
    staticApi(app);
}


void uploadApi(crow::SimpleApp& app){
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::Post)
            ([](const crow::request& req) {
                crow::multipart::message file_message(req);
                for(const auto& part:file_message.part_map)  // 依次读取表单数据
                {
                    const auto& file_name = part.first;
                    const auto& file_data = part.second;

                    if(file_name=="file")  // 如果是文件字段
                    {
                        // 获取文件名
                        auto headers_it = file_data.headers.find("Content-Disposition");
                        if (headers_it == file_data.headers.end())
                        {
                            CROW_LOG_ERROR << "No Content-Disposition found";
                            return error(400,"no Content-Disposition");
                        }
                        auto params_it = headers_it->second.params.find("filename");
                        if (params_it == headers_it->second.params.end())
                        {
                            CROW_LOG_ERROR << "Part with name \"InputFile\" should have a file";
                            return error(400,"no file name");
                        }
                        const std::string file_name = params_it->second;

                        std::ofstream out_file(file_name, std::ios::binary);
                        out_file << file_data.body;
                        out_file.close();
                    }
                    cout << "file_name: " << file_name << endl;
                    cout << "file_data: " << file_data.body.size() << endl;
                }

                return success(200,0,"上传文件成功");
            });
}

void staticApi(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/static/<string>")
            ([](const crow::request &req, crow::response &res, std::string filename) {
                res.set_static_file_info("static/" + filename);
                res.end();
            });
}