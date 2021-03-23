#include "Defense_poke.h"

Defense_poke::Defense_poke(Defense_poke_race race_) :Pokemon()//构造函数
{
	this->type = Poke_type::defense_type;//精灵种类
	this->race = race_;//精灵种族
	this->defense += DEFENSE_INC;//特征属性
}

std::string Defense_poke::attack_to(Pokemon* a)//攻击函数
{
	//图形化界面
	for (int i = 0; i < (ATTACK_LASTING / BLINK_INTERVAL) / 2; i++)
	{
		//背景及对战精灵
		putimage(0, 0, &background);//背景地图
		IMAGE* attacker_cover = poke_attack_images_cover[this->image_id];
		IMAGE* attacker = poke_attack_images[this->image_id];
		int loca_x = ATTACK_LOC_X, loca_y = ATTACK_LOC_Y;
		putimage(loca_x, loca_y, attacker_cover, SRCPAINT);
		putimage(loca_x, loca_y, attacker, SRCAND);//攻击方
		IMAGE* defenser_cover = poke_defense_images_cover[a->get_image_id()];
		IMAGE* defenser = poke_defense_images[a->get_image_id()];
		loca_x = DEFENSE_LOC_X;
		loca_y = DEFENSE_LOC_Y;
		putimage(loca_x, loca_y, defenser_cover, SRCPAINT);
		putimage(loca_x, loca_y, defenser, SRCAND);//防御方
		//攻击方血量图形化界面显示
		settextcolor(WORD_COLOR);
		settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//设定字体
		std::string text_string;
		text_string += "HP : " + std::to_string(this->hp_temp);
		outtextxy(420, 480, text_string.c_str());//攻击方血量


		//停顿片刻
		Sleep(BLINK_INTERVAL);

		//攻击动作
		putimage(400, 300, &punch_image_cover, SRCPAINT);
		putimage(400, 300, &punch_image, SRCAND);

		//停顿片刻
		Sleep(BLINK_INTERVAL);
	}

	//攻击方血量图形化界面显示
	settextcolor(WORD_COLOR);
	settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//设定字体
	std::string text_string;
	text_string += "HP : " + std::to_string(this->hp_temp);
	outtextxy(420, 480, text_string.c_str());//攻击方血量

	//信息构造
	std::string send_string;
	send_string += get_race() + " attacking with hard shell to  ";
	std::cout << get_race() << " attacking with hard shell to ";
	send_string += a->attack_from(this->attack, 1, 0, 0);//特性攻击
	return send_string;
}

void Defense_poke::level_inc()//等级上升
{
	this->level++;
	if (level > MAX_LEVEL)//满级
		this->level = MAX_LEVEL;
	this->exp -= EXP_CAPACITY;

	this->attack += STRENGTH_INC;
	this->defense += DEFENSE_INC;
	this->hp += HEALTH_INC;
	this->attack_interval -= INTERVAL_DOWN;
	this->defense += DEFENSE_INC;//特性属性增加幅度大
	if (this->attack_interval <= MIN_INTERVAL)//攻击速度有上限
		this->attack_interval = MIN_INTERVAL;
}

std::string Defense_poke::show_info()//返回精灵信息
{
	std::string ret;
	ret += get_race();
	ret += "  Defense type";
	ret += " Level : ";
	ret += std::to_string(this->get_level());
	ret += " EXP : ";
	ret += std::to_string(this->get_exp());
	ret += "  attack : ";
	ret += std::to_string(this->get_attack());
	ret += "  defense : ";
	ret += std::to_string(this->get_defense());
	ret += "  health : ";
	ret += std::to_string(this->get_hp());
	ret += "  attack interval : ";
	ret += std::to_string(this->get_attack_interval());
	ret += "\n";//构造信息字符串
	return ret;
}

std::string Defense_poke::get_race()//获得种族
{
	return to_string(this->race);
}

std::string Defense_poke::show_temp_info()const//返回精灵战斗临时信息
{
	std::string ret;
	ret += to_string(this->race);
	std::cout << to_string(this->race);
	ret += "  hp : " + std::to_string(this->hp_temp);
	ret += "  attack : " + std::to_string(this->attack_temp);
	ret += "  defense : " + std::to_string(this->defense_temp);
	ret += "  attack_interval : " + std::to_string(this->attack_interval_temp);
	ret += "\n";
	std::cout << ret;
	return ret;
}