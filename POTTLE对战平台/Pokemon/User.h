#pragma once
#include"header.h"

class User
{
private:
	std::string name;//用户名
	std::string password;//密码
	SOCKET* sock;//客户套接字
	std::vector<Pokemon*> pokemons;//用户拥有精灵
	bool online;//是否在线
	bool up_request;//是否请求升级赛
	bool duel_request;//是否请求决斗赛
	bool with_server;//是否是与服务器进行对战
	int select_id;//选择出战精灵编号
	int server_id;//选择服务器出战精灵编号
	int win_num;//获胜场次
	int total_num;//总场次
	/*
	std::vector<Strength_poke>strength_pokes;
	std::vector<Health_poke>health_pokes;
	std::vector<Defense_poke>defense_pokes;
	std::vector<Agility_poke>agility_pokes;*/

public:
	User(std::string name_, SOCKET* sock_);//构造函数
	void set_password(std::string password_);//设置密码
	int match_password(std::string input);//验证密码
	void set_online(bool online_);//登录 下线
	void win_battle(bool win_);//更新胜率
	void set_user_up_request(bool request_);//设置战斗模式
	void set_user_duel_request(bool request_);//设置战斗模式
	void set_with_server(bool with_server_);//设置战斗对象
	void set_select_id(int id_);//设置战斗选择信息
	void set_server_id(int id_);//设置战斗选择信息
	void add_poke(Pokemon* poke_);//添加精灵
	Pokemon* remove_poke(Pokemon* poke_);//移除精灵
	Pokemon* get_random_poke();//获得随机精灵
	std::string show_poke_info()const;//获得所有精灵信息字符串
	Pokemon* get_poke(int id)const;//获得指定编号精灵
	bool get_online()const;//获得在线情况
	std::string get_username()const;//获得用户名
	bool get_user_up_request()const;//获得战斗模式信息
	bool get_user_duel_request()const; //获得战斗模式信息
	SOCKET* get_socket()const;//获得用户通信套接字
	int get_poke_num()const;//获得精灵数量
	int get_select_id()const;//获得战斗选择信息
	int get_server_id()const;//获得战斗选择信息
	bool get_with_server()const;//获得战斗对象信息
	std::string get_user_info()const;//获得用户信息字符串
	int get_level_num()const;//获得高等级精灵数目
	std::vector<int> lost_select_from;//与服务器决斗失败，服务器选择最多三个精灵供用户选择送出，向量存储精灵编号
	friend int in_lost_select(std::vector<int> v, int);//检查编号是否在服务器决斗失败挑选送出精灵列表中
	friend User* check_username(std::string username);//检查用户名是否存在，若已存在返回用户指针，否则返回空指针
};

