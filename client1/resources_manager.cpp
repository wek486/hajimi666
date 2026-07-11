#include "resources_manager.h"
#include<Windows.h>
#include<graphics.h>
#pragma comment(lib,"gdi32.lib")
ResourcesManager* ResourcesManager::manager = nullptr;
ResourcesManager* ResourcesManager::instance() {
	if (!manager) {
		manager = new ResourcesManager();
	}
	return manager;
}
ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() {//必须在析构函数中主动清空两个连接池，释放内存。
	// 释放所有图集
	for (auto& pair : atlas_pool) {
		if (pair.second) {
			pair.second->clear(); // 释放 Atlas 内部的 IMAGE*
			delete pair.second;   // 释放 Atlas 自身
		}
	}
	atlas_pool.clear();

	// 释放所有单张图片
	for (auto& pair : image_pool) {
		if (pair.second) {
			delete pair.second;
		}
	}
	image_pool.clear();
}//必须在析构函数中主动清空两个连接池，释放内存。
struct ImageInfo {
	std::string id;
	std::string path;
};
struct AtlasInfo {
	std::string id;
	std::string path;
	int num_frame = 0;
};
static const std::vector<ImageInfo>image_info_list = {
	{"background",R"(resources/background.png)"},
	{"ui_textbox",R"(resources/ui_textbox.png)"},
	{"ui_fight",R"(resources/ui_fight.png)"}
};
static const  std::vector<AtlasInfo>atlas_info_list = {
	{"hajimi_idle_back",R"(resources/hajimi_idle_back_%d.png)",4},
	{"hajimi_idle_up",R"(resources/hajimi_idle_front_%d.png)",4},
	{"hajimi_idle_left",R"(resources/hajimi_idle_left_%d.png)",4},
	{"hajimi_run_back",R"(resources/hajimi_run_back_%d.png)",4},
    {"hajimi_run_up",R"(resources/hajimi_run_front_%d.png)",4},
    {"hajimi_run_left",R"(resources/hajimi_run_left_%d.png)",4},
    {"manbo_idle_back",R"(resources/manbo_idle_back_%d.png)",4},
    {"manbo_idle_up",R"(resources/manbo_idle_front_%d.png)",4},
    {"manbo_idle_left",R"(resources/manbo_idle_left_%d.png)",4},
    {"manbo_run_back",R"(resources/manbo_run_back_%d.png)",4},
    {"manbo_run_up",R"(resources/manbo_run_front_%d.png)",4},
    {"manbo_run_left",R"(resources/manbo_run_left_%d.png)",4},
    {"ui",R"(resources/ui_%d.png)",3}//路径写错问题,加载资源会错误，重要，检查好再写
};
static inline bool check_image_valid(IMAGE* image) {
	return GetImageBuffer(image);//获取不了像素缓冲区自然就是没有成功获取图片
}

void ResourcesManager::load() {
    // prevent reentrant calls to load()
    if (this->loading) return;
    this->loading = true;
    auto widen = [](const std::string& s) -> std::wstring {
        if (s.empty()) return {};
        int required = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
        if (required == 0) {
            required = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
            if (required == 0) return std::wstring();
            std::vector<wchar_t> buf(required);
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, buf.data(), required);
            return std::wstring(buf.data());
        }
        std::vector<wchar_t> buf(required);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buf.data(), required);
        return std::wstring(buf.data());
    };

    for (const auto& info : image_info_list) {
        IMAGE* image = new IMAGE();

        // check file existence first
#ifdef UNICODE
        std::wstring wpath = widen(info.path);
        if (GetFileAttributesW(wpath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            delete image;
            throw std::runtime_error(std::string("image file not found: ") + info.path);
        }
        loadimage(image, wpath.c_str());
#else
        if (GetFileAttributesA(info.path.c_str()) == INVALID_FILE_ATTRIBUTES) {
            delete image;
            throw std::runtime_error(std::string("image file not found: ") + info.path);
        }
        loadimage(image, info.path.c_str());
#endif
        if (!check_image_valid(image)){
            delete image;
            throw std::runtime_error(std::string("failed to load image: ") + info.path);
        }
        image_pool[info.id] = image;
    }
    for (const auto& info : atlas_info_list) {
        Atlas* atlas = new Atlas();
        try {
            atlas->load_image(info.path, info.num_frame);
        }
        catch (const std::exception& ex) {
            delete atlas;
            throw std::runtime_error(std::string("atlas load failed for ") + info.id + ": " + ex.what());
        }
        for (int i = 0; i < atlas->get_size(); i++) {
            IMAGE* image = atlas->get_image(i);
            if (!check_image_valid(image)){
                delete atlas;
                throw std::runtime_error(std::string("atlas contains invalid image: ") + info.path);
            }
        }
        atlas_pool[info.id] = atlas;
    }

	IMAGE* ui_fight = image_pool["ui_fight"];
	atlas_pool["ui"]->add_image(ui_fight);
	flip_atlas("manbo_idle_left", "manbo_idle_right");
	flip_atlas("manbo_run_left", "manbo_run_right");
	flip_atlas("hajimi_idle_left", "hajimi_idle_right");
	flip_atlas("hajimi_run_left", "hajimi_run_right");

    // create aliases so code using "down"/"right" naming finds the atlases
    atlas_pool["hajimi_idle_down"] = atlas_pool["hajimi_idle_back"];
    atlas_pool["hajimi_run_down"] = atlas_pool["hajimi_run_back"];
    atlas_pool["manbo_idle_down"] = atlas_pool["manbo_idle_back"];
    atlas_pool["manbo_run_down"] = atlas_pool["manbo_run_back"];
    // ensure right-facing aliases exist (flip_atlas should have created them, but set explicit aliases as fallback)
    if (atlas_pool.find("hajimi_idle_right") == atlas_pool.end()) atlas_pool["hajimi_idle_right"] = atlas_pool["hajimi_idle_left"];
    if (atlas_pool.find("hajimi_run_right") == atlas_pool.end()) atlas_pool["hajimi_run_right"] = atlas_pool["hajimi_run_left"];
    if (atlas_pool.find("manbo_idle_right") == atlas_pool.end()) atlas_pool["manbo_idle_right"] = atlas_pool["manbo_idle_left"];
    if (atlas_pool.find("manbo_run_right") == atlas_pool.end()) atlas_pool["manbo_run_right"] = atlas_pool["manbo_run_left"];
	load_audio(R"(resources\bgm.mp3)", "bgm");
	load_audio(R"(resources\1p_win.mp3)","1p_win");
	load_audio(R"(resources\2p_win.mp3)", "2p_win");
	load_audio(R"(resources\click_1.mp3)", "click_1");//后面有时间可以自定义新的点击映音效
	load_audio(R"(resources\click_2.mp3)", "click_2");
	load_audio(R"(resources\click_3.mp3)", "click_3");
	load_audio(R"(resources\click_4.mp3)", "click_4");
	load_audio(R"(resources\ui_1.mp3)", "ui_1");
	load_audio(R"(resources\ui_2.mp3)", "ui_2");
	load_audio(R"(resources\ui_3.mp3)", "ui_3");
	load_audio(R"(resources\ui_fight.mp3)", "ui_fight");
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);
    this->loading = false;

    // Debug: list loaded atlas ids and their sizes (after flips/aliases)
    for (const auto& p : atlas_pool) {
        const std::string& id = p.first;
        Atlas* a = p.second;
        int sz = a ? a->get_size() : 0;
        std::cout << "[ResourcesManager] atlas loaded: " << id << " size=" << sz << std::endl;
    }
}
Atlas* ResourcesManager::find_atlas(const std::string& id) {
	const auto& itor = atlas_pool.find(id);
	if (itor == atlas_pool.end())return NULL;
	return itor->second;
}
IMAGE* ResourcesManager::find_image(const std::string& id) {
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())return NULL;
	return itor->second;
}
void ResourcesManager::flip_image(IMAGE* src_img, IMAGE* dst_img, int num_h) {
	if (!src_img || !dst_img) return;
	if (num_h <= 0) num_h = 1;
	int w = src_img->getwidth();
	int h = src_img->getheight();
	int w_frame = w / num_h;
	Resize(dst_img, w, h);//一定在下一步之前有，不然错
	DWORD* src_buffer = GetImageBuffer(src_img);
	DWORD* dst_buffer = GetImageBuffer(dst_img);//先分配空间，图片的大小，即宽高，不然就是空指针了，危险

	for (size_t i = 0; i < num_h; i++) {
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++) {
			//用了int x = x_left; x < x_right，那么就是x不是从0开始不用减1，对齐
			for (int x = x_left; x < x_right; x++) {
				int idx_src = y * w + x;
				// mirror horizontally within the frame [x_left, x_right)
				int idx_dst_x = x_right - 1 - (x - x_left);
				int idx_dst = y * w + idx_dst_x;
				if (idx_dst >= 0 && idx_dst < w * h)
					dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}
}
void ResourcesManager::flip_image(const std::string& src_id, const std::string& dst_id, int num_h) {


	IMAGE* src_image = image_pool[src_id];
	if (src_image == NULL)return;
	IMAGE* dst_image = new IMAGE();
	flip_image(src_image, dst_image, num_h);
	image_pool[dst_id] = dst_image;


}
void ResourcesManager::flip_atlas(const std::string& src_id, const std::string& dst_id) {
	Atlas* src_atlas = atlas_pool[src_id];
	if (src_atlas == NULL)return;
	Atlas* dst_atlas = new Atlas();
	for (int i = 0; i < src_atlas->get_size(); i++) {
		IMAGE* image = new IMAGE();
		flip_image(src_atlas->get_image(i), image);
		dst_atlas->add_image(image);
	}
	atlas_pool[dst_id] = dst_atlas;

   
}