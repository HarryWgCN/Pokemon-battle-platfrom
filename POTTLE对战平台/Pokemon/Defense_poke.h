#pragma once
#include "Pokemon.h"
class Defense_poke :
	public Pokemon
{
private:
	Defense_poke_race race;//����
public:
	Defense_poke(Defense_poke_race race_);//��ʼ�������������ԣ����ӷ�����
	std::string attack_to(Pokemon* a) override;//���͹������Ĺ���
	void level_inc()override;//�ȼ�����
	std::string get_race() override;//��ȡ������Ϣ
	std::string show_info();//���ؾ�����Ϣ�ַ���
	std::string show_temp_info()const override;//����ս������Ϣ
};

