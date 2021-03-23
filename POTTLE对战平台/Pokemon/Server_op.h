#pragma once
#include"header.h"

class Server_op
{
private:
	SOCKET server_socket;//�����׽���
public:
	Server_op();
	SOCKET* get_socket();
	friend int InitServer(SOCKET* server_socket);//����WSA�����׽��ֿ�,���������׽��ֲ��󶨶˿�
	friend void init_images();//��ʼ��ͼ����Ϣ
	friend void init_server_user();//��ʼ���������û������ںͷ�������ս

	friend void server_accept_thread();//����˽��ܿͻ������������߳�
	friend void server_handle_thread(SOCKET* connect_socket);//����˴���ͻ���ͨ����Ϣ�߳�
	friend void wait_for_competitor();//ƥ���û�ս������
	friend int battle_thread(User* a, int a_id, User* b, int b_id);//ս���̣߳�aʤ������0��bʤ������1

	friend std::string to_string(Poke_type);
	friend std::string to_string(Strength_poke_race);
	friend std::string to_string(Health_poke_race);
	friend std::string to_string(Defense_poke_race);
	friend std::string to_string(Agility_poke_race);
	friend std::string to_string(bool);
};

