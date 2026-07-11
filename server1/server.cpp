// 第一行！阻止 windows.h 自动引入 winsock.h
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// 再引入新版 socket 头
#include <winsock2.h>
#include <ws2tcpip.h>

#include<iostream>
#include<fstream>
#include<Windows.h>
#include<tchar.h>
#include "../thirdparty/httplib.h"
std::mutex mtx;
std::string g_mutex;
std::string str_text;
int progress_1 = -1;
int progress_2 = -1;

int main(int argc, char** argv){
    std::ifstream file1("text.txt");
    if (!file1.good()) {
        MessageBox(nullptr, _T("无法打开目标文件text.txt"),
            _T("启动失败"), MB_OK | MB_ICONERROR);
        return -1;
    }
    std::stringstream str_stream;
    str_stream << file1.rdbuf();
    str_text = str_stream.str();
    file1.close();
    httplib::Server server;
    server.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(mtx);
        if (progress_1 >= 0 && progress_2 >= 0) {
            res.set_content("-1", "text/plain");
            return;
        }
        res.set_content((progress_1 >= 0 ? "2" : "1"), "text/plain");
        (progress_1 >= 0) ? (progress_2 = 0) : (progress_1 = 0);
        });
    server.Post("/query_text", [&](const httplib::Request& req, httplib::Response& res) {
        //std::lock_guard<std::mutex> lock(mtx);//这里的锁应该是要删掉的，这里先写着看看后面是否会 出错
        res.set_content(str_text, "text/plain");
        });
    server.Post("/update_1", [&](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(mtx);
        progress_1 = std::stoi(req.body);
        res.set_content(std::to_string(progress_2), "text/plain");
        });
    server.Post("/update_2", [&](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(mtx);
        progress_2 = std::stoi(req.body);
        res.set_content(std::to_string(progress_1), "text/plain");
        });
	server.listen("localhost", 25565);
    return 0;
}