#pragma once
#include "Pokemon.h"
class Agility_poke :
	public Pokemon
{
private:
	Agility_poke_race race;//����
public:
	Agility_poke(Agility_poke_race race_);//��ʼ�������������ԣ����ӹ����ٶ�
	std::string attack_to(Pokemon* a) override;//���͹����ٶȵĹ���
	void level_inc()override;//�ȼ�����
	std::string get_race() override;//��ȡ������Ϣ
	std::string show_info();//���ؾ�����Ϣ�ַ���
	std::string show_temp_info()const override;//����ս������Ϣ
};

