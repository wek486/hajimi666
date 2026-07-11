#pragma once
#include "atlas.h"
#include "vector2.h"
#include "camera.h"
#include<string>

// 新增这一行，mciSendString 所需头文件
#include <mmsystem.h>

#pragma comment(lib,"WINMM.lib")
#pragma comment (lib,"MSIMG32.LIB")//mciSendStringA还是可能要头文件
typedef struct {
	int x, y;
	int w, h;
}Rect;

inline void putimage_ex(const camera& cam, IMAGE* img, const Rect* rect_dst, const Rect* rect_src = NULL) {
    int w = img->getwidth();
    int h = img->getheight();
    // apply camera position (view origin) plus any shake offset stored in camera.camera_position
    Vector2 cam_offset = cam.get_position();
    cam_offset += cam.camera_position;
    AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x - (int)cam_offset.x, rect_dst->y - (int)cam_offset.y, rect_dst->w, rect_dst->h, GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0, rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
inline void load_audio(std::string path, std::string id) {
	std::string str_cmd = "open " + path + " alias " + id;
	mciSendStringA(str_cmd.c_str(), NULL, 0, NULL);
}
inline void play_audio(std::string id, bool is_loop) {
	std::string str_cmd = "play " + id + (is_loop ? " repeat" : " from 0");//一定加上from 0在 mciSendString 中，如果你给音频起了一个固定的 alias（比如 player_attack_1），而这个音频还没播放完你就再次触发播放指令，MCI 往往会忽略第二次请求。
	mciSendStringA(str_cmd.c_str(), NULL, 0, NULL);
}
inline void stop_audio(std::string id) {
	std::string str_cmd = "stop " + id;
	mciSendStringA(str_cmd.c_str(), NULL, 0, NULL);
}

					

