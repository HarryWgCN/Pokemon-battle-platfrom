#pragma once
#include"header.h"

class Pokemon
{
protected:
	Poke_type type;//��������
	int level;//�ȼ�
	int exp;//����ֵ
	int attack;//������
	int attack_temp;//ս������ʱ������
	int defense;//������
	int defense_temp;//ս������ʱ������
	int hp;//����ֵ
	int hp_temp;//ս������ʱ����ֵ
	int attack_interval;//�������
	int attack_interval_temp;//ս������ʱ�������
	int rest_time_to_attack;//ս���о�����һ�ι�����ʣ��ʱ��
	bool attack_aff;//�����Ѿ��ܵ���Ӱ�죬������Ӱ��
	bool defense_aff;//�������Ѿ��ܵ���Ӱ�죬������Ӱ��
	bool interval_aff;//�����ٶ��Ѿ��ܵ���Ӱ�죬������Ӱ��
	int image_id;//ͼ��洢λ��
public:
	Pokemon();//���캯��
	virtual std::string attack_to(Pokemon* a) = 0; //�������������������������д
	std::string attack_from(int attack_volumn, int attack_dec, int defense_dec, int interval_inc);//�ܵ���������������Ϊ����ֵ�͹���Ӱ��
	void exp_inc(int exp_volumn);//�ɳ���������
	virtual void level_inc() = 0;//�ȼ����ӣ�ÿ�����ྫ���������Բ�ͬ����������д
	void battle_init();//ս��׼��,��ʼ��ս���й�������������������ֵ�����������ʣ��ȴ�����ʱ��
	void set_image_id(int id);//����ͼ��洢λ��
	//��ȡ����
	virtual std::string get_race() = 0;//���������Ϣ����������д
	int get_level()const;//��õȼ�
	int get_exp()const;//��þ���ֵ
	int get_hp()const;//�������ֵ
	int get_hp_temp()const;//���ս����ʱ����ֵ
	int get_attack()const;//��ù�����
	int get_attack_temp()const;//���ս����ʱ������
	int get_defense()const;//��÷�����
	int get_defense_temp()const;//���ս����ʱ������
	int get_attack_interval()const;//��ù������
	int get_attack_interval_temp()const;//���ս����ʱ�������
	int get_rtta()const;//��þ����´ι������
	int get_image_id()const;//���ͼ��洢λ��	
	int rtta_dec();//��������ʱ���£�����ս���о�����һ�ι�����ʣ��ʱ�䣬��Ϊ0��ʼ���й���
	virtual std::string show_temp_info()const = 0;//��þ���ս����ʱ��Ϣ�ַ���
	virtual std::string show_info() = 0;//���ؾ�����Ϣ�ַ���
};