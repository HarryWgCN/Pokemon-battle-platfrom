#pragma once
#include "Pokemon.h"
class Health_poke :
	public Pokemon
{
private:
	Health_poke_race race;//����
public:
	Health_poke(Health_poke_race race_);//��ʼ�������������ԣ���������ֵ
	std::string attack_to(Pokemon* a) override;//û������Ч���Ĺ���
	void level_inc()override;//�ȼ�����
	std::string get_race() override;//��ȡ������Ϣ
	std::string show_info();//���ؾ�����Ϣ�ַ���
	std::string show_temp_info()const override;//����ս������Ϣ
};

