#pragma once
#include "Pokemon.h"
class Strength_poke :
	public Pokemon
{
private:
	Strength_poke_race race;//����
public:
	Strength_poke(Strength_poke_race race_);//��ʼ�������������ԣ����ӹ�����
	std::string attack_to(Pokemon* a) override;//���ͷ������Ĺ���
	void level_inc()override;//�ȼ�����
	std::string get_race() override;//��ȡ������Ϣ
	std::string show_info();//���ؾ�����Ϣ�ַ���
	std::string show_temp_info()const override;//����ս������Ϣ
};

