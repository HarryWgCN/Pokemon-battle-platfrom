#include "Pokemon.h"

Pokemon::Pokemon()//���캯��
{
	this->image_id = 0;//Ϊ���������ʼ��
	this->type = Poke_type::agility_type;//Ϊ���������ʼ��
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
	this->interval_aff = FALSE;//��ʼ������
}

std::string Pokemon::attack_from(int attack_volumn, int attack_dec, int defense_dec, int interval_inc)//�ܵ���������������Ϊ����ֵ�͹���Ӱ��
{
	std::string ret;//����ս����Ϣ
	ret += get_race() + ".\n";
	std::cout << get_race() << std::endl;

	srand((int)time(0));// �����������
	int critical, dodge;
	critical = rand() % 100;//����
	dodge = rand() % 100;//����

	if (dodge <= DODGE_RATE)//�����ܣ�ͼ�λ�������ʾ����
	{
		putimage(440, 100, &dodged_image_cover, SRCPAINT);
		putimage(440, 100, &dodged_image, SRCAND);
	}

	if (critical <= CRITICAL_RATE)//��������ͼ�λ�������ʾ����
	{
		ret += "Critical attack!!!!!!!!!  Double attack volumn.\n";
		std::cout << "Critical attack!!!!!!!!!  Double attack volumn.\n";
		attack_volumn = attack_volumn * 2;//˫������
		
		if (dodge > DODGE_RATE)//�����ܣ��򲻻Ὣ������ʾ��ͼ�λ�������
		{
			//ͼ�λ�����
			putimage(440, 100, &critical_image_cover, SRCPAINT);
			putimage(440, 100, &critical_image, SRCAND);
		}
	}

	ret += get_race();
	std::cout << get_race();

	//����
	if (dodge <= DODGE_RATE)
	{
		ret += " dodged attack!!!!!!!!!.\n";
		std::cout << " dodged attack!!!!!!!!!.\n";
		//������Ѫ��ͼ�λ�������ʾ
		settextcolor(WORD_COLOR);
		settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//�趨����
		std::string text;
		text += "HP : " + std::to_string(this->hp_temp);
		outtextxy(800, 480, text.c_str());
		return ret;//�������ر�
	}


	if (attack_volumn > this->defense_temp)//�ܵ��˺�
	{
		//�ܵ����˺� = ����ֵ-����ֵ
		this->hp_temp -= attack_volumn - this->defense_temp;
		ret = ret + " is damaged by " + std::to_string(attack_volumn - this->defense_temp) + " now HP : " + std::to_string(this->hp_temp) + " of " + std::to_string(this->hp) + ".\n";
	}
	else//û���յ��˺�
	{
		ret += " is not dameged.\n";
	}

	//����ܵ����Թ������Ӱ��
	if (attack_dec && !this->attack_aff)//�������½�
	{
		this->attack_aff = true;
		this->attack_temp -= ATTACK_AFF * attack_dec;
		ret += this->get_race();
		ret += "'s attack is decreased to " + this->attack_temp;
		ret += "\n";
	}
	if (defense_dec && !this->defense_aff)//�������½�
	{
		this->defense_aff = true;
		this->defense_temp -= DEFENSE_AFF * defense_dec;
		ret += this->get_race();
		ret += "'s defense is decreased to " + this->defense_temp;
		ret += "\n";
	}
	if (interval_inc && !this->interval_aff)//�����ٶȱ���
	{
		this->interval_aff = true;
		this->attack_interval_temp += INTERVAL_AFF * interval_inc;
		ret += this->get_race();
		ret += "'s attack_interval is increase to " + this->attack_interval_temp;
		ret += "\n";
	}

	//������Ѫ��ͼ�λ�������ʾ
	settextcolor(WORD_COLOR);
	settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//�趨����
	std::string text;
	text += "HP : " + std::to_string(this->hp_temp);
	outtextxy(800, 480, text.c_str());

	std::cout << ret;
	return ret;
}

void Pokemon::exp_inc(int exp_volumn)//�ɳ���������
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



void Pokemon::battle_init()//ս��׼��,��ʼ��ս���й�������������������ֵ�����������ʣ��ȴ�����ʱ��
{
	//��ʼ��ս������ʱ����
	this->attack_temp = this->attack;
	this->attack_aff = false;
	this->defense_temp = this->defense;
	this->defense_aff = false;
	this->hp_temp = this->hp;
	this->attack_interval_temp = this->attack_interval;
	this->interval_aff = false;
	this->rest_time_to_attack = this->attack_interval_temp;//�ܵ�������ɵĹ������Ӱ�������һ���ȴ�����������Ч
}

void Pokemon::set_image_id(int id)//����ͼ��洢λ��
{
	this->image_id = id;
}


int Pokemon::get_level()const//��õȼ�
{
	return this->level;
}

int Pokemon::get_exp()const//��þ���ֵ
{
	return this->exp;
}

int Pokemon::get_hp()const//�������ֵ
{
	return this->hp;
}

int Pokemon::get_hp_temp()const//���ս����ʱ����ֵ
{
	return this->hp_temp;
}

int Pokemon::get_attack()const//��ù�����
{
	return this->attack;
}

int Pokemon::get_attack_temp()const//���ս����ʱ������
{
	return this->attack_temp;
}

int Pokemon::get_defense()const//��÷�����
{
	return this->defense;
}

int Pokemon::get_defense_temp()const//���ս����ʱ������
{
	return this->defense_temp;
}

int Pokemon::get_attack_interval()const//��ù������
{
	return this->attack_interval;
}

int Pokemon::get_attack_interval_temp()const//���ս����ʱ�������
{
	return this->attack_interval_temp;
}

int Pokemon::get_rtta()const//��þ����´ι������
{
	return this->rest_time_to_attack;
}

int Pokemon::get_image_id()const//���ͼ��洢λ��	
{
	return this->image_id;
}

int Pokemon::rtta_dec()//����ʱ���£�����ս���о�����һ�ι�����ʣ��ʱ�䣬��Ϊ0��ʼ���й���
{
	//rtta��attack_interval_temp-1��0������attack_interval_temp���غϣ����һ�غϽ���һ�ι���
	this->rest_time_to_attack = (--this->rest_time_to_attack + this->attack_interval_temp) % this->attack_interval_temp;
	return this->rest_time_to_attack;//����ʣ��ʱ��
}
