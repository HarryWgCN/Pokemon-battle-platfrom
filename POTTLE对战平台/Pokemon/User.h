#pragma once
#include"header.h"

class User
{
private:
	std::string name;//�û���
	std::string password;//����
	SOCKET* sock;//�ͻ��׽���
	std::vector<Pokemon*> pokemons;//�û�ӵ�о���
	bool online;//�Ƿ�����
	bool up_request;//�Ƿ�����������
	bool duel_request;//�Ƿ����������
	bool with_server;//�Ƿ�������������ж�ս
	int select_id;//ѡ���ս������
	int server_id;//ѡ���������ս������
	int win_num;//��ʤ����
	int total_num;//�ܳ���
	/*
	std::vector<Strength_poke>strength_pokes;
	std::vector<Health_poke>health_pokes;
	std::vector<Defense_poke>defense_pokes;
	std::vector<Agility_poke>agility_pokes;*/

public:
	User(std::string name_, SOCKET* sock_);//���캯��
	void set_password(std::string password_);//��������
	int match_password(std::string input);//��֤����
	void set_online(bool online_);//��¼ ����
	void win_battle(bool win_);//����ʤ��
	void set_user_up_request(bool request_);//����ս��ģʽ
	void set_user_duel_request(bool request_);//����ս��ģʽ
	void set_with_server(bool with_server_);//����ս������
	void set_select_id(int id_);//����ս��ѡ����Ϣ
	void set_server_id(int id_);//����ս��ѡ����Ϣ
	void add_poke(Pokemon* poke_);//��Ӿ���
	Pokemon* remove_poke(Pokemon* poke_);//�Ƴ�����
	Pokemon* get_random_poke();//����������
	std::string show_poke_info()const;//������о�����Ϣ�ַ���
	Pokemon* get_poke(int id)const;//���ָ����ž���
	bool get_online()const;//����������
	std::string get_username()const;//����û���
	bool get_user_up_request()const;//���ս��ģʽ��Ϣ
	bool get_user_duel_request()const; //���ս��ģʽ��Ϣ
	SOCKET* get_socket()const;//����û�ͨ���׽���
	int get_poke_num()const;//��þ�������
	int get_select_id()const;//���ս��ѡ����Ϣ
	int get_server_id()const;//���ս��ѡ����Ϣ
	bool get_with_server()const;//���ս��������Ϣ
	std::string get_user_info()const;//����û���Ϣ�ַ���
	int get_level_num()const;//��øߵȼ�������Ŀ
	std::vector<int> lost_select_from;//�����������ʧ�ܣ�������ѡ������������鹩�û�ѡ���ͳ��������洢������
	friend int in_lost_select(std::vector<int> v, int);//������Ƿ��ڷ���������ʧ����ѡ�ͳ������б���
	friend User* check_username(std::string username);//����û����Ƿ���ڣ����Ѵ��ڷ����û�ָ�룬���򷵻ؿ�ָ��
};

