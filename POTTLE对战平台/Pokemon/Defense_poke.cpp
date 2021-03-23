#include "Defense_poke.h"

Defense_poke::Defense_poke(Defense_poke_race race_) :Pokemon()//���캯��
{
	this->type = Poke_type::defense_type;//��������
	this->race = race_;//��������
	this->defense += DEFENSE_INC;//��������
}

std::string Defense_poke::attack_to(Pokemon* a)//��������
{
	//ͼ�λ�����
	for (int i = 0; i < (ATTACK_LASTING / BLINK_INTERVAL) / 2; i++)
	{
		//��������ս����
		putimage(0, 0, &background);//������ͼ
		IMAGE* attacker_cover = poke_attack_images_cover[this->image_id];
		IMAGE* attacker = poke_attack_images[this->image_id];
		int loca_x = ATTACK_LOC_X, loca_y = ATTACK_LOC_Y;
		putimage(loca_x, loca_y, attacker_cover, SRCPAINT);
		putimage(loca_x, loca_y, attacker, SRCAND);//������
		IMAGE* defenser_cover = poke_defense_images_cover[a->get_image_id()];
		IMAGE* defenser = poke_defense_images[a->get_image_id()];
		loca_x = DEFENSE_LOC_X;
		loca_y = DEFENSE_LOC_Y;
		putimage(loca_x, loca_y, defenser_cover, SRCPAINT);
		putimage(loca_x, loca_y, defenser, SRCAND);//������
		//������Ѫ��ͼ�λ�������ʾ
		settextcolor(WORD_COLOR);
		settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//�趨����
		std::string text_string;
		text_string += "HP : " + std::to_string(this->hp_temp);
		outtextxy(420, 480, text_string.c_str());//������Ѫ��


		//ͣ��Ƭ��
		Sleep(BLINK_INTERVAL);

		//��������
		putimage(400, 300, &punch_image_cover, SRCPAINT);
		putimage(400, 300, &punch_image, SRCAND);

		//ͣ��Ƭ��
		Sleep(BLINK_INTERVAL);
	}

	//������Ѫ��ͼ�λ�������ʾ
	settextcolor(WORD_COLOR);
	settextstyle(25, 0, _T("Consolas"), 0, 0, 0, 0, 0, 0);//�趨����
	std::string text_string;
	text_string += "HP : " + std::to_string(this->hp_temp);
	outtextxy(420, 480, text_string.c_str());//������Ѫ��

	//��Ϣ����
	std::string send_string;
	send_string += get_race() + " attacking with hard shell to  ";
	std::cout << get_race() << " attacking with hard shell to ";
	send_string += a->attack_from(this->attack, 1, 0, 0);//���Թ���
	return send_string;
}

void Defense_poke::level_inc()//�ȼ�����
{
	this->level++;
	if (level > MAX_LEVEL)//����
		this->level = MAX_LEVEL;
	this->exp -= EXP_CAPACITY;

	this->attack += STRENGTH_INC;
	this->defense += DEFENSE_INC;
	this->hp += HEALTH_INC;
	this->attack_interval -= INTERVAL_DOWN;
	this->defense += DEFENSE_INC;//�����������ӷ��ȴ�
	if (this->attack_interval <= MIN_INTERVAL)//�����ٶ�������
		this->attack_interval = MIN_INTERVAL;
}

std::string Defense_poke::show_info()//���ؾ�����Ϣ
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
	ret += "\n";//������Ϣ�ַ���
	return ret;
}

std::string Defense_poke::get_race()//�������
{
	return to_string(this->race);
}

std::string Defense_poke::show_temp_info()const//���ؾ���ս����ʱ��Ϣ
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