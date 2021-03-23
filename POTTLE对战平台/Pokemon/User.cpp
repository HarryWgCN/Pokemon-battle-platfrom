#include "User.h"
#include"Pokemon.h"
#include"Strength_poke.h"
#include"Health_poke.h"
#include"Defense_poke.h"
#include"Agility_poke.h"

User::User(std::string name_, SOCKET* sock_)//构造函数
{
	this->name = name_;
	this->sock = sock_;
	this->duel_request = false;
	this->up_request = false;
	this->with_server = false;
	this->win_num = 0;
	this->total_num = 0;//初始化各种信息
	srand((int)time(0));// 产生随机种子
	this->add_poke(this->get_random_poke());
	this->add_poke(this->get_random_poke());
	this->add_poke(this->get_random_poke());//初始获得随机分配的三只精灵
}

void User::set_password(std::string password_)//设置密码
{
	this->password = password_;
}

int User::match_password(std::string input)//验证密码正确性
{
	if (this->password == input)
		return 0;
	return 1;
}

void User::set_online(bool online_)//登录下线操作
{
	this->online = online_;
}

void User::win_battle(bool win_)//记录胜率信息
{
	this->total_num++;
	if (win_ == true)
		this->win_num++;
}

void User::set_user_up_request(bool request_)//战斗模式请求
{
	this->up_request = request_;
}

void User::set_user_duel_request(bool request_)//战斗模式请求
{
	this->duel_request = request_;

}

void User::set_select_id(int id_)//设置战斗选择信息
{
	this->select_id = id_;
}

void User::set_server_id(int id_)//设置战斗选择信息
{
	this->server_id = id_;
}

void User::add_poke(Pokemon* poke_)//增加精灵
{
	this->pokemons.push_back(poke_);
}

Pokemon* User::remove_poke(Pokemon* poke_)//移出精灵
{
	for (auto p = this->pokemons.begin(); p != this->pokemons.end();p++)
	{
		if ((*p) == poke_)
		{
			this->pokemons.erase(p);
			return poke_;
		}
	}
	if (this->pokemons.size() == 0)//没有精灵，随机分配一只
	{
		srand((int)time(0));// 产生随机种子
		this->add_poke(this->get_random_poke());
	}
	return NULL;
}

void User::set_with_server(bool with_server_)//战斗对象选择
{
	this->with_server = with_server_;
}

Pokemon* User::get_random_poke()//随机产生一只精灵
{
	int result;
	result = rand() % 4;
	Pokemon* poke = NULL;
	switch (result)//四种类型精灵 每种两个种族
	{
	case 0:
		poke = (Pokemon*)new Strength_poke(static_cast<Strength_poke_race>(rand() % 2));
		poke->get_race() == "Charmeleon" ? poke->set_image_id(0) : poke->set_image_id(1);
		return poke; break;
	case 1:
		poke = (Pokemon*)new Health_poke(static_cast<Health_poke_race>(rand() % 2));
		poke->get_race() == "Geodude" ? poke->set_image_id(2) : poke->set_image_id(3);
		return poke; break;
	case 2:
		poke = (Pokemon*)new Defense_poke(static_cast<Defense_poke_race>(rand() % 2));
		poke->get_race() == "Shellder" ? poke->set_image_id(4) : poke->set_image_id(5);
		return poke; break;
	case 3:
		poke = (Pokemon*)new Agility_poke(static_cast<Agility_poke_race>(rand() % 2));
		poke->get_race() == "Pikachu" ? poke->set_image_id(6) : poke->set_image_id(7);
		return poke; break;
	}
	return NULL;
}

std::string User::show_poke_info()const//展示所有精灵信息
{
    std::string ret;
	int j = 0;
    if (pokemons.empty())
        return "Failed to get pokemons";
	for (auto i = this->pokemons.begin(); i != this->pokemons.end(); i++, j++)
	{
		ret += "id : " + std::to_string(j) + "  "+ (*i)->show_info();
	}
    return ret;
}

Pokemon* User::get_poke(int id)const//获得指定编号精灵
{
	if (id < static_cast<int>(this->pokemons.size()))
		return pokemons[id];
	else
		return NULL;
}

bool User::get_online()const//获得在线情况
{
	return this->online;
}

std::string User::get_username()const//获得用户名
{
	return this->name;
}

bool User::get_user_up_request()const//获得战斗模式
{
	return this->up_request;
}
bool User::get_user_duel_request()const//获得战斗模式
{
	return this->duel_request;
}

SOCKET* User::get_socket()const//获得用户对应通信套接字
{
	return this->sock;
}

int User::get_poke_num()const//获得精灵数目
{
	return this->pokemons.size();
}

int User::get_select_id()const//获得战斗选择信息
{
	return this->select_id;
}

int User::get_server_id()const//获得战斗选择信息
{
	return this->server_id;
}

bool User::get_with_server()const//获得战斗对象选择信息
{
	return this->with_server;
}

std::string User::get_user_info()const//获得用户信息字符串
{
	std::string ret;
	ret += this->get_username();
	ret += "\nTotal_battle : " + std::to_string(this->total_num);
	ret += "  Wining : " + std::to_string(this->win_num);
	ret += "\nBadges :\n";
	//徽章信息
	if (this->get_poke_num() >= BRONZE_NUM)//精灵数量徽章
	{
		if (this->get_poke_num() >= SILVER_NUM)
		{
			if (this->get_poke_num() >= GOLDEN_NUM)
				ret += "Golden badge in pokemon numbers.\n";
			else
				ret += "Silver badge in pokemon numbers.\n";
		}
		else
			ret += "Bronze badge in pokemon numbers.\n";
	}
	else
	{
		ret += "No badges with high_quantity of pokemons.\n";
	}
	if (this->get_level_num() >= BRONZE_LEV)//精灵等级徽章
	{
		if (this->get_level_num() >= SILVER_LEV)
		{
			if (this->get_level_num() >= GOLDEN_LEV)
				ret += "Golden badge in pokemon levels.\n";
			else
				ret += "Silver badge in pokemon levels.\n";
		}
		else
			ret += "Bronze badge in pokemon levels.\n";
	}
	else
	{
		ret += "No badges with high_level of pokemons.\n";
	}

	ret += "Enroll in upgrade battle [";
	ret += to_string(this->get_user_up_request());
	ret += "] Enroll in duel battle [";
	ret += to_string(this->get_user_duel_request());
	ret += "]\nPokemons:\n" + this->show_poke_info();
	return ret;
}

int User::get_level_num()const//获得高等级宠物数目
{
	int i = 0;
	for (auto i = this->pokemons.begin(); i != this->pokemons.end(); i++)
	{
		if ((*i)->get_level() >= LEVEL_STANDARD)
			i++;
	}
	return i;
}