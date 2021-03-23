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


//socket相关参数
#define PORT 6000//服务器监听端口
#define BUF_SIZE 1024//缓冲区大小
#define RECORD_SIZE (10 * BUF_SIZE)//战斗信息记录缓冲区大小
#define NO_RESPONSE_LIMIT 600000//十分钟无响应自动下线

//精灵基础属性值
#define BASIC_ATTACK 60
#define BASIC_DEFENSE 20	
#define BASIC_HP 120
#define BASIC_INTERVAL 12
#define CRITICAL_RATE 32
#define DODGE_RATE 32

//种类精灵特征属性变化
#define STRENGTH_INC 20
#define HEALTH_INC 60
#define DEFENSE_INC 10
#define INTERVAL_DEC 1

//升级属性增加
#define STRENGTH_UP 10
#define HEALTH_UP 10
#define DEFENSE_UP 10
#define INTERVAL_DOWN 1
#define MIN_INTERVAL 5

//特性攻击影响
#define ATTACK_AFF 10
#define DEFENSE_AFF 10
#define INTERVAL_AFF 3

//精灵等级相关属性
#define EXP_CAPACITY 100//每一级经验值
#define MAX_LEVEL 15//满级
#define BATTLE_EXP(a,b) (50 + 20 * (b - a))//战斗取胜方经验值奖励，50 + 20*二者等级差，a为获胜方，b为落败方

//荣誉勋章标准
#define GOLDEN_NUM 10
#define SILVER_NUM 5
#define BRONZE_NUM 3
#define LEVEL_STANDARD 15//高等级精灵等级要求
#define GOLDEN_LEV 5
#define SILVER_LEV 3
#define BRONZE_LEV 1

//战斗图形界面模拟参数
#define ROUND_INTERVAL 1000//每轮基础延时
#define ATTACK_LASTING 2000//攻击特效延时
#define BLINK_INTERVAL 100//特效闪烁间隔
#define ATTACK_LOC_X 0
#define ATTACK_LOC_Y 100//攻击方精灵图像坐标
#define DEFENSE_LOC_X 820
#define DEFENSE_LOC_Y 100//防御方精灵图像坐标
#define GRAPH_WIDTH 1280
#define GRAPH_HEIGHT 720//图形界面尺寸
#define WORD_COLOR  RGB(255, 0, 0)//文字颜色

enum class Poke_type { strength_type = 0, health_type, defense_type, agility_type };//力量型（高攻击）， 生命型（高血量）， 防守型（高防御）， 敏捷性（低攻击间隔）
enum class Strength_poke_race { Charmeleon = 0, Charizard };//火恐龙 喷火龙
enum class Health_poke_race { Geodude = 0, Graveler };//小拳石 隆隆岩
enum class Defense_poke_race { Shellder = 0, Cloyster };//大舌贝 铁甲贝
enum class Agility_poke_race { Pikachu = 0, Raichu };//皮卡丘 雷丘


class Pokemon;//精灵类，具有各种属性，内置精灵间攻击函数，成长函数
class Strength_poke;//力量型精灵，内置力量型攻击函数
class Health_poke;//肉盾型精灵，内置肉盾型攻击函数
class Defense_poke;//防御型精灵，内置防御型攻击函数
class Agility_poke;//敏捷型精灵，内置敏捷型攻击函数
class User;//用户类包括用户的用户名、精灵等信息

extern std::vector<User*> users;//用户向量，存储系统注册的用户
extern std::mutex users_mutex;//用户向量锁
extern User* server_user;//服务器用户

//图像对象
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

int InitServer(SOCKET* server_socket);//Server_ini友元函数，启动WSA加载套接字库,创建服务套接字并绑定端口
void init_images();//初始化图像信息
void init_server_user();//初始化服务器用户，用于和服务器对战

void server_accept_thread();//服务端接受客户端连接请求线程
void server_handle_thread(SOCKET* connect_socket);//服务端处理客户端通信信息线程
void wait_for_competitor();//匹配用户战斗请求
int battle_thread(User* a, int a_id, User* b, int b_id);//战斗线程，a胜利返回0，b胜利返回1

User* check_username(std::string);//User类友元函数，检查用户名是否存在，若已存在返回用户指针，否则返回空指针
int in_lost_select(std::vector<int> v, int id);//User类友元函数，检查编号是否在服务器决斗失败挑选送出精灵列表中

//类型字符串转化函数
std::string to_string(Poke_type );
std::string to_string(Strength_poke_race);
std::string to_string(Health_poke_race);
std::string to_string(Defense_poke_race);
std::string to_string(Agility_poke_race);
std::string to_string(bool);