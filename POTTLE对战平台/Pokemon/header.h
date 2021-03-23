#pragma once
#include<WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include<vector>
#include<iostream>
#include<thread>
#include<cstring>
#include<string>
#include<cstdlib>
#include<ctime>
#include<mutex>
#include<easyx.h>
#include<graphics.h>
#include<fstream>
#include<atlstr.h>
#pragma comment(lib,"Ws2_32.lib")


//socket��ز���
#define PORT 6000//�����������˿�
#define BUF_SIZE 1024//��������С
#define RECORD_SIZE (10 * BUF_SIZE)//ս����Ϣ��¼��������С
#define NO_RESPONSE_LIMIT 600000//ʮ��������Ӧ�Զ�����

//�����������ֵ
#define BASIC_ATTACK 60
#define BASIC_DEFENSE 20	
#define BASIC_HP 120
#define BASIC_INTERVAL 12
#define CRITICAL_RATE 32
#define DODGE_RATE 32

//���ྫ���������Ա仯
#define STRENGTH_INC 20
#define HEALTH_INC 60
#define DEFENSE_INC 10
#define INTERVAL_DEC 1

//������������
#define STRENGTH_UP 10
#define HEALTH_UP 10
#define DEFENSE_UP 10
#define INTERVAL_DOWN 1
#define MIN_INTERVAL 5

//���Թ���Ӱ��
#define ATTACK_AFF 10
#define DEFENSE_AFF 10
#define INTERVAL_AFF 3

//����ȼ��������
#define EXP_CAPACITY 100//ÿһ������ֵ
#define MAX_LEVEL 15//����
#define BATTLE_EXP(a,b) (50 + 20 * (b - a))//ս��ȡʤ������ֵ������50 + 20*���ߵȼ��aΪ��ʤ����bΪ��ܷ�

//����ѫ�±�׼
#define GOLDEN_NUM 10
#define SILVER_NUM 5
#define BRONZE_NUM 3
#define LEVEL_STANDARD 15//�ߵȼ�����ȼ�Ҫ��
#define GOLDEN_LEV 5
#define SILVER_LEV 3
#define BRONZE_LEV 1

//ս��ͼ�ν���ģ�����
#define ROUND_INTERVAL 1000//ÿ�ֻ�����ʱ
#define ATTACK_LASTING 2000//������Ч��ʱ
#define BLINK_INTERVAL 100//��Ч��˸���
#define ATTACK_LOC_X 0
#define ATTACK_LOC_Y 100//����������ͼ������
#define DEFENSE_LOC_X 820
#define DEFENSE_LOC_Y 100//����������ͼ������
#define GRAPH_WIDTH 1280
#define GRAPH_HEIGHT 720//ͼ�ν���ߴ�
#define WORD_COLOR  RGB(255, 0, 0)//������ɫ

enum class Poke_type { strength_type = 0, health_type, defense_type, agility_type };//�����ͣ��߹������� �����ͣ���Ѫ������ �����ͣ��߷������� �����ԣ��͹��������
enum class Strength_poke_race { Charmeleon = 0, Charizard };//����� �����
enum class Health_poke_race { Geodude = 0, Graveler };//Сȭʯ ¡¡��
enum class Defense_poke_race { Shellder = 0, Cloyster };//���౴ ���ױ�
enum class Agility_poke_race { Pikachu = 0, Raichu };//Ƥ���� ����


class Pokemon;//�����࣬���и������ԣ����þ���乥���������ɳ�����
class Strength_poke;//�����;��飬���������͹�������
class Health_poke;//����;��飬��������͹�������
class Defense_poke;//�����;��飬���÷����͹�������
class Agility_poke;//�����;��飬���������͹�������
class User;//�û�������û����û������������Ϣ

extern std::vector<User*> users;//�û��������洢ϵͳע����û�
extern std::mutex users_mutex;//�û�������
extern User* server_user;//�������û�

//ͼ�����
extern std::vector<IMAGE*>poke_attack_images;
extern std::vector<IMAGE*>poke_defense_images;
extern std::vector<IMAGE*>poke_attack_images_cover;
extern std::vector<IMAGE*>poke_defense_images_cover;
extern IMAGE background;
extern IMAGE Charmeleon_defense_image;
extern IMAGE Charmeleon_defense_image_cover;
extern IMAGE Charizard_defense_image;
extern IMAGE Charizard_defense_image_cover;
extern IMAGE Geodude_defense_image;
extern IMAGE Geodude_defense_image_cover;
extern IMAGE Graveler_defense_image;
extern IMAGE Graveler_defense_image_cover;
extern IMAGE Shellder_defense_image;
extern IMAGE Shellder_defense_image_cover;
extern IMAGE Cloyster_defense_image;
extern IMAGE Cloyster_defense_image_cover;
extern IMAGE Pikachu_defense_image;
extern IMAGE Pikachu_defense_image_cover;
extern IMAGE Raichu_defense_image;
extern IMAGE Raichu_defense_image_cover;
extern IMAGE background;
extern IMAGE Charmeleon_attack_image;
extern IMAGE Charmeleon_attack_image_cover;
extern IMAGE Charizard_attack_image;
extern IMAGE Charizard_attack_image_cover;
extern IMAGE Geodude_attack_image;
extern IMAGE Geodude_attack_image_cover;
extern IMAGE Graveler_attack_image;
extern IMAGE Graveler_attack_image_cover;
extern IMAGE Shellder_attack_image;
extern IMAGE Shellder_attack_image_cover;
extern IMAGE Cloyster_attack_image;
extern IMAGE Cloyster_attack_image_cover;
extern IMAGE Pikachu_attack_image;
extern IMAGE Pikachu_attack_image_cover;
extern IMAGE Raichu_attack_image;
extern IMAGE Raichu_attack_image_cover;
extern IMAGE lightning_image;
extern IMAGE fire_image;
extern IMAGE rocks_image;
extern IMAGE punch_image;
extern IMAGE lightning_image_cover;
extern IMAGE fire_image_cover;
extern IMAGE rocks_image_cover;
extern IMAGE punch_image_cover;
extern IMAGE critical_image;
extern IMAGE dodged_image;
extern IMAGE critical_image_cover;
extern IMAGE dodged_image_cover;

int InitServer(SOCKET* server_socket);//Server_ini��Ԫ����������WSA�����׽��ֿ�,���������׽��ֲ��󶨶˿�
void init_images();//��ʼ��ͼ����Ϣ
void init_server_user();//��ʼ���������û������ںͷ�������ս

void server_accept_thread();//����˽��ܿͻ������������߳�
void server_handle_thread(SOCKET* connect_socket);//����˴���ͻ���ͨ����Ϣ�߳�
void wait_for_competitor();//ƥ���û�ս������
int battle_thread(User* a, int a_id, User* b, int b_id);//ս���̣߳�aʤ������0��bʤ������1

User* check_username(std::string);//User����Ԫ����������û����Ƿ���ڣ����Ѵ��ڷ����û�ָ�룬���򷵻ؿ�ָ��
int in_lost_select(std::vector<int> v, int id);//User����Ԫ������������Ƿ��ڷ���������ʧ����ѡ�ͳ������б���

//�����ַ���ת������
std::string to_string(Poke_type );
std::string to_string(Strength_poke_race);
std::string to_string(Health_poke_race);
std::string to_string(Defense_poke_race);
std::string to_string(Agility_poke_race);
std::string to_string(bool);