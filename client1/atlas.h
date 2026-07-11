
#ifndef ATLAS_H
#define ATLAS_H
#include <vector>
#include <graphics.h>
#include <iostream>
#include <string>
#include <Windows.h>
class Atlas {
public:
	Atlas() = default;
	~Atlas() = default;
	void clear() {
		if (image_list.empty())return;
		for (IMAGE* img : image_list) {
			delete img;
		}
		image_list.clear();
	}
	void add_image(IMAGE* img) {
		image_list.push_back(img);
	}
    void load_image(std::string path, int num) {
        image_list.clear();

        for (size_t i = 0; i < (size_t)num; i++) {
            char full_path[512] = {0};
            // path contains a format specifier like "%d"
            int needed = _snprintf_s(full_path, sizeof(full_path), _TRUNCATE, path.c_str(), static_cast<int>(i + 1));
            if (needed < 0) {
                // path was truncated or formatting failed
                image_list.clear();
                throw std::runtime_error(std::string("atlas path formatting failed for: ") + path);
            }

            IMAGE* img = new IMAGE();

            // Convert to wide string when UNICODE is enabled
#ifdef UNICODE
            auto widen = [](const std::string& s) -> std::wstring {
                if (s.empty()) return {};
                int required = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
                if (required == 0) {
                    // fall back to ANSI code page
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

            std::string full_path_str(full_path);
            std::wstring wfull = widen(full_path_str);
            // check file exists (wide)
            DWORD fa = GetFileAttributesW(wfull.c_str());
            if (fa == INVALID_FILE_ATTRIBUTES) {
                delete img;
                image_list.clear();
                throw std::runtime_error(std::string("atlas file not found: ") + full_path_str);
            }
            loadimage(img, wfull.c_str());
#else

            // check file exists (ansi)
            std::string full_path_str(full_path);
            DWORD fa = GetFileAttributesA(full_path_str.c_str());
            if (fa == INVALID_FILE_ATTRIBUTES) {
                delete img;
                image_list.clear();
                throw std::runtime_error(std::string("atlas file not found: ") + full_path_str);
            }
            loadimage(img, full_path_str.c_str());
#endif

            // validate image
            if (!GetImageBuffer(img)) {
                delete img;
                image_list.clear();
                throw std::runtime_error(std::string("load image failed: ") + full_path_str);
            }

            image_list.push_back(img);
        }

    }
	int get_size() {
		if (image_list.empty())return 0;
		return image_list.size();
	}
	IMAGE* get_image(int idx) {
		if (idx < 0 || idx >= image_list.size())return NULL;
		return image_list[idx];
	}
	friend class Animation;
private:

	std::vector<IMAGE*>image_list;//这里用指针也有要防止空指针


};

#endif
