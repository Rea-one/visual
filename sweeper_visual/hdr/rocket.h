//
//  Created by Reasone
//  Date: 2024.06.12
//

#ifndef SWEEPER_CONNECTOR_H
#define SWEEPER_CONNECTOR_H


#include "iostream"
#include "cstdint"
#include "vector"
#include "queue"
#include "winsock2.h"
#include "ws2tcpip.h"
#include "thread"
#include "mutex"
#include "deque"
#include "chrono"
#include "string"
#include "condition_variable"

#pragma comment(lib, "Ws2_32.lib")


typedef std::vector<int> mess;
typedef std::queue<mess> messes;


class rocket
{
protected:
    // 数据池
    SOCKET talk{};
    SOCKET listen{};
    messes pool{};
    std::thread tick{};
    std::thread listening{};

    sockaddr_in socket_standard_message{};
    // 信号控制
    bool connected = false;
    bool activate = false;
    bool closed = false;

    // 方法
    void linker_waiter ();
    void loop ();


    // 辅助型
    std::mutex pool_mutex{};
    bool shutdown_type = false;


public:
    explicit rocket (const std::string& host="127.0.0.1");
    ~rocket ();
    void close ();
    void link_to (int channel=24524);
    void sever_create (int channel=24524);
    void write (const mess& data) const;
    bool read (mess& data);
    bool feed_status () const;
};



#endif //SWEEPER_CONNECTOR_H
