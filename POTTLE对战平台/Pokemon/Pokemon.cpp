#include "Pokemon.h"

Pokemon::Pokemon()//构造函数
{
	this->image_id = 0;//为消除警告初始化
	this->type = Poke_type::agility_type;//为消除警告初始化
	this->level = 1;
	this->exp = 0;
	this->attack = BASIC_ATTACK;
	this->attack_temp = this->attack;
	this->defense = BASIC_DEFENSE;
	this->defense_temp = this->defense;
	this->hp = BASIC_HP;
	this->hp_temp = this->hp;
	this->attack_interval = BASIC_INTERVAL;
	this->attack_interval_temp = this->attack_interval;
	this->rest_time_to_attack = this->attack_interval;
	this->attack_aff = FALSE;
	this->defense_aff = FALSE;
	this->interval_aff = FALSE;//初始化属性
}

std::string Pokemon::attack_from(int attack_volumn, int attack_dec, int defense_dec, int interval_inc)//受到攻击方法，参数为攻击值和攻击影响
{
	std::string ret;//返回战斗信息
	ret += get_race() + ".\n";
	std::cout << get_race() << std::endl;

	srand((int)time(0));// 产生随机种子
	int critical, dodge;
	critical = rand() % 100;//暴击
	dodge = rand() % 100;//闪避

	if (dodge <= DODGE_RATE)//若闪避，图形化界面显示闪避
	{
		putimage(440, 100, &dodged_image_cover, SRCPAINT);
		putimage(440, 100, &dodged_image, SRCAND);
	}

	if (critical <= CRITICAL_RATE)//若暴击，图形化界面显示暴击
	{
		ret += "Critical attack!!!!!!!!!  Double attack volumn.\n";
		std::cout << "Critical attack!!!!!!!!!  Double attack volumn.\n";
		attack_volumn = attack_volumn * 2;//双倍攻击
		
		if (dodge > DODGE_RATE)//若闪避，则不会将暴击显示在图形化界面上
		{
			//图形化界面
			putimage(440, 100, &critical_image_cover, SRCPAINT);
			putimage(440, 100, &critical_image, SRCAND);
		}
	}

	ret += get_race();
	std::cout << get_race();

	//闪避
	if (dodge <= DODGE_RATE)
	{
		ret += " dodged attack!!!!!!!!!.\n";
		std::cout << " dodged attack!!!!!!!!!.\n";
		//防御方血量图形化界面显示
		settextcolor(WORD_COLOR);
		settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//设定字体
		std::string text;
		text += "HP : " + std::to_string(this->hp_temp);
		outtextxy(800, 480, text.c_str());
		return ret;//攻击被回避
	}


	if (attack_volumn > this->defense_temp)//受到伤害
	{
		//受到的伤害 = 攻击值-防御值
		this->hp_temp -= attack_volumn - this->defense_temp;
		ret = ret + " is damaged by " + std::to_string(attack_volumn - this->defense_temp) + " now HP : " + std::to_string(this->hp_temp) + " of " + std::to_string(this->hp) + ".\n";
	}
	else//没有收到伤害
	{
		ret += " is not dameged.\n";
	}

	//添加受到特性攻击造成影响
	if (attack_dec && !this->attack_aff)//攻击力下降
	{
		this->attack_aff = true;
		this->attack_temp -= ATTACK_AFF * attack_dec;
		ret += this->get_race();
		ret += "'s attack is decreased to " + this->attack_temp;
		ret += "\n";
	}
	if (defense_dec && !this->defense_aff)//防御力下降
	{
		this->defense_aff = true;
		this->defense_temp -= DEFENSE_AFF * defense_dec;
		ret += this->get_race();
		ret += "'s defense is decreased to " + this->defense_temp;
		ret += "\n";
	}
	if (interval_inc && !this->interval_aff)//攻击速度变慢
	{
		this->interval_aff = true;
		this->attack_interval_temp += INTERVAL_AFF * interval_inc;
		ret += this->get_race();
		ret += "'s attack_interval is increase to " + this->attack_interval_temp;
		ret += "\n";
	}

	//防御方血量图形化界面显示
	settextcolor(WORD_COLOR);
	settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//设定字体
	std::string text;
	text += "HP : " + std::to_string(this->hp_temp);
	outtextxy(800, 480, text.c_str());

	std::cout << ret;
	return ret;
}

void Pokemon::exp_inc(int exp_volumn)//成长经验增加
{
	if (exp_volumn < 0)
		return;
	this->exp += exp_volumn;
	while (true)
	{
		if (this->exp >= EXP_CAPACITY)
			this->level_inc();
		else
			break;
	}
	return;
}



void Pokemon::battle_init()//战斗准备,初始化战斗中攻击力、防御力、生命值、攻击间隔、剩余等待攻击时间
{
	//初始化战斗中临时属性
	this->attack_temp = this->attack;
	this->attack_aff = false;
	this->defense_temp = this->defense;
	this->defense_aff = false;
	this->hp_temp = this->hp;
	this->attack_interval_temp = this->attack_interval;
	this->interval_aff = false;
	this->rest_time_to_attack = this->attack_interval_temp;//受到攻击造成的攻击间隔影响会在下一个等待攻击周期生效
}

void Pokemon::set_image_id(int id)//设置图像存储位置
{
	this->image_id = id;
}


int Pokemon::get_level()const//获得等级
{
	return this->level;
}

int Pokemon::get_exp()const//获得经验值
{
	return this->exp;
}

int Pokemon::get_hp()const//获得生命值
{
	return this->hp;
}

int Pokemon::get_hp_temp()const//获得战斗临时生命值
{
	return this->hp_temp;
}

int Pokemon::get_attack()const//获得攻击力
{
	return this->attack;
}

int Pokemon::get_attack_temp()const//获得战斗临时攻击力
{
	return this->attack_temp;
}

int Pokemon::get_defense()const//获得防御力
{
	return this->defense;
}

int Pokemon::get_defense_temp()const//获得战斗临时防御力
{
	return this->defense_temp;
}

int Pokemon::get_attack_interval()const//获得攻击间隔
{
	return this->attack_interval;
}

int Pokemon::get_attack_interval_temp()const//获得战斗临时攻击间隔
{
	return this->attack_interval_temp;
}

int Pokemon::get_rtta()const//获得距离下次攻击间隔
{
	return this->rest_time_to_attack;
}

int Pokemon::get_image_id()const//获得图像存储位置	
{
	return this->image_id;
}

int Pokemon::rtta_dec()//倒计时更新，返回战斗中距离下一次攻击的剩余时间，若为0则开始进行攻击
{
	//rtta从attack_interval_temp-1到0共经历attack_interval_temp个回合，最后一回合进行一次攻击
	this->rest_time_to_attack = (--this->rest_time_to_attack + this->attack_interval_temp) % this->attack_interval_temp;
	return this->rest_time_to_attack;//返回剩余时间
}
