//
//  Created by Reasone
//  Date: 2024.06.12
//

#include "rocket.h"


using namespace std;


rocket::rocket(const string& host)
{
    // 初始化WSA
    constexpr WORD socketVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    WSAStartup(socketVersion, &wsaData);
    // 初始化套接字
    socket_standard_message.sin_family = AF_INET;
    socket_standard_message.sin_addr.s_addr = inet_addr(host.c_str());
}
rocket::~rocket()
{
    if (!closed)
    {
        close();
        shutdown_type = false;
    }
}

void rocket::close()
{
    connected = false;
    if (activate)
    {
        activate = false;
        closesocket(listen);
    }
    closesocket(talk);
    if (listening.joinable())
        listening.join();
    if (tick.joinable())
        tick.join();
    WSACleanup();
    closed = true;
    shutdown_type = true;
}


void rocket::link_to(int channel)
{
    talk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socket_standard_message.sin_port = htons(channel);
    constexpr int max_attempts = 3; // 最大尝试次数
    const std::chrono::milliseconds retry_delay(256); // 每次尝试之间的延时，例如256毫秒

    for (int attempt = 0; attempt < max_attempts && !connected; ++attempt)
    {
        // 尝试连接
        int connect_status = connect(talk,
                                     (struct sockaddr*)&socket_standard_message,
                                             sizeof(socket_standard_message));

        if (connect_status == 0)
        {
            // 连接成功
            connected = true;
        }
        else
        {
            // 连接失败，获取错误码并打印
            int err = WSAGetLastError();
            cerr << "Connect attempt " << attempt + 1 << " failed with error: " << err << endl;

            // 等待一段时间后再次尝试
            this_thread::sleep_for(retry_delay);
        }
    }

    if (!connected)
    {
        cerr << "Failed to connect after " << max_attempts << " attempts." << endl;
        // 清理工作，如关闭套接字等
        closesocket(talk);
        WSACleanup();
        return;
    }
    tick = thread(&rocket::loop, this);
}

void rocket::sever_create(int channel)
{
    listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socket_standard_message.sin_port = htons(channel);
    if (bind(listen, (struct sockaddr*)&socket_standard_message,
        sizeof(socket_standard_message)))
    {
        std::cerr << "Error: Unable to bind socket." << std::endl;
        closesocket(listen);
        WSACleanup();
        return;
    }
    activate = true;
    listening = thread(&rocket::linker_waiter, this);
}

void rocket::linker_waiter()
{
    while (activate)
    {
        if (!connected)
        {
            talk = accept(listen, nullptr, nullptr);
            connected = true;
            tick = thread(&rocket::loop, this);
        }
        this_thread::sleep_for (chrono::milliseconds(512));
    }
    closesocket(listen);
}

void rocket::loop()
{
    fd_set read_fds{};
    struct timeval timeout{};
    // 将socket添加到集合中
    FD_SET(talk, &read_fds);
    // 设置超时时间
    timeout.tv_sec = 128;
    timeout.tv_usec = 128;
    mutex connect_mutex;
    while (connected)
    {
        // 使用select函数监听socket的可读状态，避免了不必要的轮询
        int status = select(static_cast<int>(talk) + 1, &read_fds,
                        nullptr, nullptr, &timeout);
        if (status != SOCKET_ERROR && status != 0)
        {
            char c_cross[64] {};
            int read_stt = recv (talk, c_cross, 64, 0);
            if (read_stt == SOCKET_ERROR)
            {
                // 处理错误
                continue;
            }
            istringstream read_mess (c_cross);
            int num {};
            mess data {};
            while (read_mess >> num)
            {
                data.push_back (num);
            }
            unique_lock<mutex> loop_lock(pool_mutex);
            pool.push (data);
            loop_lock.unlock();
        }
    }
}


void rocket::write(const mess& data) const
{
    // 信息载体
    stringstream write_mess;
    for (const auto& elem: data)
    {
        write_mess << elem;
        write_mess << " ";
    }
    send (talk, write_mess.str().c_str(),
        static_cast<int> (write_mess.str().length()), 0);
}

bool rocket::read(mess& data)
{
    unique_lock<mutex> read_lock(pool_mutex); // 确保线程安全
    if (pool.empty())
    {
        return false;
    }
    else
    {
        data = pool.front();
        pool.pop();
        read_lock.unlock();
        return true;
    }

}

bool rocket::feed_status() const
{
    return shutdown_type;
}


