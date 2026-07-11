#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H
#include "atlas.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "util.h"
class ResourcesManager {
public:
	
	static ResourcesManager* instance();//获取单例实例的静态成员函数，返回指向唯一实例的指针,如果实例不存在则创建一个新的实例
	void load();
	Atlas* find_atlas(const std::string& id);
	IMAGE* find_image(const std::string& id);
private:
	std::unordered_map<std::string, Atlas*>atlas_pool;
	std::unordered_map<std::string, IMAGE*>image_pool;
	void flip_image(IMAGE* src_img, IMAGE* dst_img, int num_h = 1);
	void flip_image(const std::string& src_id, const std::string& dst_id, int num_h);
	void flip_atlas(const std::string& src_id, const std::string& dst_id);
	static ResourcesManager* manager;//单例模式的静态成员变量，指向唯一实例的指针
    bool loading = false; // guard against reentrant load calls
	//构造函数和析构函数私有化，禁止外部创建和销毁实例
	//但是和碰撞箱相比的话有不同了，碰撞箱只用给管理器一个接口就行了



	ResourcesManager();
	~ResourcesManager();
};

#endif