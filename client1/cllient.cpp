#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING//流到utf_8字符相关
// 第一行！阻止 windows.h 自动引入 winsock.h
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// 再引入新版 socket 头
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../thirdparty/httplib.h"
#include<iostream>
#include<graphics.h>
#include<ctime>
#include"camera.h"
#include<chrono>
#include<stdlib.h>
#include"util.h"
#include"resources_manager.h"
#include<tchar.h>
#include<thread>
#include "player.h"
#include"path.h"
#include<locale>
#include<codecvt>
#pragma comment(lib,"Imm32.lib")
enum class Stage {
	Waiting,
	Ready,
	Racing
};
int val_countdown = 4;
Stage stage = Stage::Waiting;
int id_player = 0;
std::atomic<int> progress_1 = -1;
std::atomic<int> progress_2 = -1;
int num_total_char = 0;
static void draw_background(camera cam) {
	static IMAGE* img_background = ResourcesManager::instance()->find_image("background");
	static Rect rect_dst = {
	0,0,
		img_background->getwidth(),
		img_background->getheight()
	};
	putimage_ex(cam, img_background, &rect_dst);
}
Path path = Path({
	{842,842},
	{1332,842},
	{1322,442},
	{2762,442},
	{2762,842},
	{3162,842},
	{3162,1722},
	 {2122,1722},
	{2122,1562},
	{842,1562},
	{842,842}
	});
int  idx_line = 0;
int idx_char = 0;
std::string str_text;
std::vector<std::string>str_line_list;
camera* Camera = nullptr;
Player* player1 = nullptr;
Player* player2 = nullptr;

std::string str_address;//服务器地址
httplib::Client* client = nullptr;//客户端对象
void client_load() {
	std::fstream file("config.cfg");

	if (!file.good()) {
		std::cerr << "无法打开配置文件" << std::endl;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
    str_address = str_stream.str();
    // trim whitespace/newline from config (avoid '\n' or '\r')
    auto is_space = [](unsigned char c){ return std::isspace(c); };
    // trim left
    size_t start = 0;
    while (start < str_address.size() && is_space((unsigned char)str_address[start])) start++;
    size_t end = str_address.size();
    while (end > start && is_space((unsigned char)str_address[end-1])) end--;
    str_address = str_address.substr(start, end - start);
}
void login_to_server() {
	client = new httplib::Client(str_address);
	client->set_keep_alive(true);
	httplib::Result res = client->Post("/login");
	HWND hwnd = GetHWnd();
	if (!res || res->status != 200) {
		MessageBox(hwnd, _T("无法连接服务器"), _T("启动失败"), MB_OK | MB_ICONERROR);
		exit(-1);
	}
	id_player = std::stoi(res->body);
	if (id_player <= 0) {
		MessageBox(hwnd, _T("比赛已经开始"), _T("拒绝加入哦"), MB_OK | MB_ICONERROR);
		exit(-1);
	}
	(id_player == 1) ? (progress_1 = 0) : (progress_2 = 0);
	str_text = client->Post("/query_text")->body;
	std::stringstream str_stream(str_text);
	std::string str_line;
	while (std::getline(str_stream, str_line)) {
		str_line_list.push_back(str_line);
		num_total_char += (int)str_line.length();
	}
	std::thread([&] (){
		while (true) {
			std::string route = (id_player == 1) ? "/update_1" : "/update_2";
			std::string body = std::to_string((id_player == 1) ? progress_1 : progress_2);
			httplib::Result result = client->Post(route, body, "text/plain");
			if (result && result->status == 200) {
				int progress = std::stoi(result->body);
				(id_player == 1) ? (progress_2 = progress) : (progress_1 = progress);
			}
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / 10));
		}

		}).detach();
}

int main(void) {
	srand(time(NULL));
	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);//后续可以关闭的控制台
	HIMC himc = ImmGetContext(hwnd);
	if (himc) {
		ImmAssociateContext(hwnd, NULL); // 立即解除关联，这样游戏过程中按 A/D 就不会跳输入法了
		ImmReleaseContext(hwnd, himc);   // 释放获取到的上下文句柄
	}
	
	try {
		ResourcesManager::instance()->load();
	}
	catch (const std::runtime_error& err) { //  正确捕获标准运行时异常
		std::string err_msg = err.what();   // 获取具体的错误路径和原因

		// 简易将 std::string 转换为 std::wstring 供 MessageBoxW 使用
		std::wstring werr(err_msg.begin(), err_msg.end());

		MessageBox(hwnd, werr.c_str(), _T("资源加载失败"), (MB_OK | MB_ICONERROR));
		return -1;
	}
	SetWindowText(hwnd, _T("哈基米大冒险"));
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
    // create players after resources are loaded so their animations can find atlases
    player1 = new Player();
    player2 = new Player();
	ExMessage msg;
	std::chrono::nanoseconds frame_duration(1000000000/144);
	bool is_quit = false;
	BeginBatchDraw();
	std::chrono::steady_clock::time_point last_frame = std::chrono::steady_clock::now();

	Timer timer_countdown;
	camera camera_ui, camera_scene;
	camera_ui.set_size({ 1280,720 });
	camera_scene.set_size({ 1280,720 });
	player1->set_position({ 842,842 });
	player2->set_position({ 842,842 });//玩家位置初始化
	client_load();
	login_to_server();
	Animation* ui_ready = new Animation();
	ui_ready->set_loop(false);

	ui_ready->add_frame(ResourcesManager::instance()->find_atlas("ui"));
	ui_ready->set_position(Vector2(getwidth() / 2, getheight() / 2));//ui位置初始化
	timer_countdown.set_one_shot(false);
	timer_countdown.set_wait_time(1.0f);
	timer_countdown.set_callback([&]() {
		val_countdown--;
		if (val_countdown >= 0 && val_countdown <= 3) {
			// 因为 Animation 声明了 friend class 或者是可以通过特定方式，
			// 如果你的 Animation 没有提供 set_frame_index，可以直接给内部变量赋值（它是类的成员，由于在main中可能需要写个小接口）
			// 建议在 Animation 类里加一个公有函数：void set_frame_index(size_t idx) { idx_frame = idx; }
			ui_ready->set_frame_index(3 - val_countdown);
		}
		switch (val_countdown) {
		case 3:play_audio("ui_1", false); break;
		case 2:play_audio("ui_2", false); break;
		case 1:play_audio("ui_3", false); break;
		case 0:play_audio("ui_fight", false); break;
		case -1:stage = Stage::Racing;
			timer_countdown.pause();
			stop_audio("ui_fight");
			stop_audio("ui_1");
			stop_audio("ui_2");
			stop_audio("ui_3");
			play_audio("bgm", true);
			break;
		}
		});

	while (!is_quit) {
		ImmAssociateContext(hwnd, NULL);
		while (peekmessage(&msg)) {
		 // Only handle WM_CHAR for gameplay typing when in Racing stage.
		 if (msg.message == WM_CHAR && stage == Stage::Racing && idx_line < str_line_list.size()) {
			 const std::string& str_line = str_line_list[idx_line];
			 // bounds-check idx_char before indexing
			 if (idx_char < (int)str_line.length() && str_line[idx_char] == msg.ch) {
					 switch (rand() % 4) {
					 case 0:play_audio("click_0", false); break;
					 case 1:play_audio("click_1", false); break;
					 case 2:play_audio("click_2", false); break;
					 case 3:play_audio("click_3", false); break;
					 }
					 (id_player == 1) ? (progress_1++) : (progress_2++);
					 idx_char++;//这里是现加了之后再判断的，所以是>=符号
				 if (idx_char >= (int)str_line.length()) {
						 idx_char = 0;
						 idx_line++;
					 }
				 }
		 }
		}
		std::chrono::steady_clock::time_point current_frame = std::chrono::steady_clock::now();
		std::chrono::duration<float> delta = std::chrono::duration<float>(current_frame - last_frame);//把纳秒换成小数的时间间隔，由chrono提供

		

		//更新

		if (stage == Stage::Waiting) {
			if (progress_1 >= 0 && progress_2 >= 0) {
				stage = Stage::Ready;
				
					

					//  核心修复：在此处手动激活并开启定时器！
					val_countdown = 4;
					timer_countdown.restart(); // 确保 Timer 内部状态重置并开始激活动画时钟

					// 初始第一帧对齐
					ui_ready->set_frame_index(0);
				
			}
		}
		else {
			if (stage == Stage::Ready) {
				timer_countdown.on_update(delta.count());
			

			}
			if ((id_player == 1 && progress_1 >= num_total_char) || (id_player == 2 && progress_2 >= num_total_char)) {
				play_audio((id_player == 1 ? "1p_win" : "2p_win"), false);
				MessageBox(hwnd, _T("恭喜你赢了"), _T("游戏结束了哦"), MB_OK | MB_ICONINFORMATION);
				is_quit = true;
			}
			else if ((id_player == 1 && progress_2 >= num_total_char) ||
				(id_player == 2 && progress_1 >= num_total_char)) {
				MessageBox(hwnd, _T("你输了哦，拜托加点油吧"), _T("游戏结束了哦"), MB_OK | MB_ICONINFORMATION);
				is_quit = true;
			}
			// ✨ 核心保护：只有当进度 >= 0 时才计算路径位置，防止 -1 产生非法运算
			float prog1 = (progress_1 < 0) ? 0.0f : (float)progress_1 / num_total_char;
			float prog2 = (progress_2 < 0) ? 0.0f : (float)progress_2 / num_total_char;

			player1->set_target(path.get_position_at_progress(prog1));
			player2->set_target(path.get_position_at_progress(prog2));

			player1->on_update(delta.count());
			player2->on_update(delta.count());
			camera_scene.look_at((id_player == 1) ? player1->get_position() : player2->get_position());

		}
		setbkcolor(RGB(0, 0, 0));
		cleardevice();
		//绘图
		if (stage == Stage::Waiting) {
			settextcolor(RGB(195, 195, 195));
			outtextxy(15, 675, _T("比赛即将开始，等待其他玩家加入中。。。。"));
		}
		else {
			draw_background(camera_scene);
			// ✨ 修改这里：只要 val_countdown >= 0，不管在什么阶段，都把倒计时 UI 画出来！
			if (val_countdown >= 0
				) {
				ui_ready->on_render(camera_ui);
			}
			
			if (player1->get_position().y > player2->get_position().y) {
				player2->on_render(camera_scene);
				player1->on_render(camera_scene);
			}
			else {
				player1->on_render(camera_scene);
				player2->on_render(camera_scene);
			}
		}
		
		//player1->on_update(delta.count());
		//player2->on_update(delta.count());

		
		
		if (stage == Stage::Racing) {
			static const Rect rect_textbox = {
				0,720 - (ResourcesManager::instance()->find_image("ui_textbox"))->getheight(),
				(ResourcesManager::instance()->find_image("ui_textbox"))->getwidth(),(ResourcesManager::instance()->find_image("ui_textbox"))->getheight()
			};
			static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>convert;//心功能：UTF-8 字符串 ↔ wchar_t 宽字符串 互相转换
			std::wstring wstr_line = convert.from_bytes(str_line_list[idx_line]);
			std::wstring wstr_completed = convert.from_bytes(str_line_list[idx_line].substr(0,idx_char));
			putimage_ex(camera_ui,ResourcesManager::instance()->find_image("ui_textbox"), &rect_textbox);
			settextcolor(RGB(125, 125, 125));
			outtextxy(185 + 2, rect_textbox.y + 65 + 2, wstr_line.c_str());
			settextcolor(RGB(25, 25, 25));
			outtextxy(185 , rect_textbox.y + 65 , wstr_line.c_str());
			settextcolor(RGB(0, 149, 217));
			outtextxy(185, rect_textbox.y + 65, wstr_completed.c_str());
		}
		FlushBatchDraw();
		last_frame = current_frame;
		std::chrono::nanoseconds sleep_duration = frame_duration - (std::chrono::steady_clock::now() - current_frame);
		if (sleep_duration > std::chrono::nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);//休眠

	}
	stop_audio("bgm");
	EndBatchDraw();
	closegraph();
	//ImmAssociateContext(f, PrevImc);
	return 0;
}