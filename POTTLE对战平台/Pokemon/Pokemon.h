#pragma once
#include"header.h"

class Pokemon
{
protected:
	Poke_type type;//精灵种类
	int level;//等级
	int exp;//经验值
	int attack;//攻击力
	int attack_temp;//战斗中临时攻击力
	int defense;//防御力
	int defense_temp;//战斗中临时防御力
	int hp;//生命值
	int hp_temp;//战斗中临时生命值
	int attack_interval;//攻击间隔
	int attack_interval_temp;//战斗中临时攻击间隔
	int rest_time_to_attack;//战斗中距离下一次攻击的剩余时间
	bool attack_aff;//攻击已经受到了影响，不再受影响
	bool defense_aff;//防御力已经受到了影响，不再受影响
	bool interval_aff;//攻击速度已经受到了影响，不再受影响
	int image_id;//图像存储位置
public:
	Pokemon();//构造函数
	virtual std::string attack_to(Pokemon* a) = 0; //攻击方法，各种类子类各自重写
	std::string attack_from(int attack_volumn, int attack_dec, int defense_dec, int interval_inc);//受到攻击方法，参数为攻击值和攻击影响
	void exp_inc(int exp_volumn);//成长经验增加
	virtual void level_inc() = 0;//等级增加，每个种类精灵增加属性不同，由子类重写
	void battle_init();//战斗准备,初始化战斗中攻击力、防御力、生命值、攻击间隔、剩余等待攻击时间
	void set_image_id(int id);//设置图像存储位置
	//获取属性
	virtual std::string get_race() = 0;//获得种族信息，由子类重写
	int get_level()const;//获得等级
	int get_exp()const;//获得经验值
	int get_hp()const;//获得生命值
	int get_hp_temp()const;//获得战斗临时生命值
	int get_attack()const;//获得攻击力
	int get_attack_temp()const;//获得战斗临时攻击力
	int get_defense()const;//获得防御力
	int get_defense_temp()const;//获得战斗临时防御力
	int get_attack_interval()const;//获得攻击间隔
	int get_attack_interval_temp()const;//获得战斗临时攻击间隔
	int get_rtta()const;//获得距离下次攻击间隔
	int get_image_id()const;//获得图像存储位置	
	int rtta_dec();//攻击倒计时更新，返回战斗中距离下一次攻击的剩余时间，若为0则开始进行攻击
	virtual std::string show_temp_info()const = 0;//获得精灵战斗临时信息字符串
	virtual std::string show_info() = 0;//返回精灵信息字符串
};