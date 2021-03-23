#pragma once
#include"header.h"

class Server_op
{
private:
	SOCKET server_socket;//监听套接字
public:
	Server_op();
	SOCKET* get_socket();
	friend int InitServer(SOCKET* server_socket);//启动WSA加载套接字库,创建服务套接字并绑定端口
	friend void init_images();//初始化图像信息
	friend void init_server_user();//初始化服务器用户，用于和服务器对战

	friend void server_accept_thread();//服务端接受客户端连接请求线程
	friend void server_handle_thread(SOCKET* connect_socket);//服务端处理客户端通信信息线程
	friend void wait_for_competitor();//匹配用户战斗请求
	friend int battle_thread(User* a, int a_id, User* b, int b_id);//战斗线程，a胜利返回0，b胜利返回1

	friend std::string to_string(Poke_type);
	friend std::string to_string(Strength_poke_race);
	friend std::string to_string(Health_poke_race);
	friend std::string to_string(Defense_poke_race);
	friend std::string to_string(Agility_poke_race);
	friend std::string to_string(bool);
};

