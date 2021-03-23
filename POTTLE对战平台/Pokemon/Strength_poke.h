#pragma once
#include "Pokemon.h"
class Strength_poke :
	public Pokemon
{
private:
	Strength_poke_race race;//种族
public:
	Strength_poke(Strength_poke_race race_);//初始化种类特征属性，增加攻击力
	std::string attack_to(Pokemon* a) override;//降低防御力的攻击
	void level_inc()override;//等级上升
	std::string get_race() override;//获取种族信息
	std::string show_info();//返回精灵信息字符串
	std::string show_temp_info()const override;//返回战斗中信息
};

