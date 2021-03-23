#pragma once
#include "Pokemon.h"
class Defense_poke :
	public Pokemon
{
private:
	Defense_poke_race race;//种族
public:
	Defense_poke(Defense_poke_race race_);//初始化种类特征属性，增加防御力
	std::string attack_to(Pokemon* a) override;//降低攻击力的攻击
	void level_inc()override;//等级上升
	std::string get_race() override;//获取种族信息
	std::string show_info();//返回精灵信息字符串
	std::string show_temp_info()const override;//返回战斗中信息
};

