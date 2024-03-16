//
// Created by fanlu on 2024/2/27.
//

#ifndef CPP_SERVER_SINGLETON_H
#define CPP_SERVER_SINGLETON_H
#pragma once

class Singleton {
public:
    // 公有静态成员函数，用于获取单例对象
    static Singleton& getInstance() {
        // 如果实例不存在，则创建一个新实例
        // 该实例在程序生命周期内只会被创建一次
        static Singleton instance;
        return instance;
    }

    // 以下函数防止复制实例和赋值实例
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // 将构造函数和析构函数私有化，防止外部创建实例和删除实例
    Singleton() {}
    ~Singleton() {}
};


#endif //CPP_SERVER_SINGLETON_H
