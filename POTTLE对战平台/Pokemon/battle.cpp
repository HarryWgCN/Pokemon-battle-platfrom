#include"header.h"
#include"Pokemon.h"
#include"Strength_poke.h"
#include"Health_poke.h"
#include"Defense_poke.h"
#include"Agility_poke.h"
#include"header.h"
#include"User.h"
#include"Pokemon.h"
#include"Strength_poke.h"
#include"Health_poke.h"
#include"Defense_poke.h"
#include"Agility_poke.h"

void wait_for_competitor()//�ȴ�ƥ����������ͬһս��ģʽ�û�������
{
	int match = 0;//��matchΪ2ʱ��ƥ��
	User* u[2];//ƥ��������û�

	while (true)
	{
		//��ս������
		//������
		users_mutex.lock();//����
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//�ҵ������������û���������
			if ((*i)->get_with_server() == true && (*i)->get_user_up_request() == true)
			{
				u[0] = *i;			
				std::thread battle(battle_thread, u[0], u[0]->get_select_id(), server_user, u[0]->get_server_id());
				battle.join();
				u[0]->set_with_server(false);
				u[0]->set_user_up_request(false);
			}
		}
		users_mutex.unlock();//����

		//������
		users_mutex.lock();//����
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//�ҵ������������û���������
			if ((*i)->get_with_server() == true && (*i)->get_user_duel_request() == true)
			{
				u[0] = *i;
				std::thread battle(battle_thread, u[0], u[0]->get_select_id(), server_user, u[0]->get_server_id());
				battle.join();
				u[0]->set_user_duel_request(false);
				u[0]->set_with_server(false);

			}
		}
		users_mutex.unlock();//����

		//�û����ս
		//ƥ������������
		match = 0;
		users_mutex.lock();//����
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//�ҵ������������û���������
			if ((*i)->get_user_up_request() == true)
			{
				u[match] = *i;
				match++;
			}
			//ƥ��
			if (match == 2)
				break;
		}
		users_mutex.unlock();//����
		//ƥ��ɹ�������ս��
		if (match == 2)
		{
			std::thread battle(battle_thread, u[0], u[0]->get_select_id(), u[1], u[1]->get_select_id());
			battle.join();//��ս���߳���ɣ�����ƥ������
			u[0]->set_user_up_request(false);
			u[1]->set_user_up_request(false);
		}

		//ƥ�����������
		match = 0;
		users_mutex.lock();//����
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//�ҵ������������û���������
			if ((*i)->get_user_duel_request() == true)
			{
				u[match] = *i;
				match++;
			}
			//ƥ��
			if (match == 2)
				break;
		}
		users_mutex.unlock();//����
		if (match == 2)
		{
			std::thread battle(battle_thread, u[0], u[0]->get_select_id(), u[1], u[1]->get_select_id());//��ʼս���߳�
			battle.join();//��ս���߳���ɣ�����ƥ������
			u[0]->set_user_duel_request(false);
			u[1]->set_user_duel_request(false);//ս������ȡ��
		}
	}

}

int battle_thread(User* a, int a_id, User* b, int b_id)
{
	//��ʼ��ս��������ս������ս����¼
	User *winner, *loser;
	Pokemon* lost_poke;//ʤ������
	char sendbuf[RECORD_SIZE];
	Pokemon* poke_a = a->get_poke(a_id);
	Pokemon* poke_b = b->get_poke(b_id);//ѡ�����վ
	poke_a->battle_init();
	poke_b->battle_init();//ս����ʼ��
	std::string record = "\n";//ս����Ϣ��¼
	std::string winner_record, loser_record;//��ʤ����ʧ�ܷ�ս����Ϣ��ͬ

	//ս���Իغ�Ϊ��λ����
	for (int round = 1; poke_a->get_hp_temp() > 0 && poke_b->get_hp_temp() > 0; round++)
	{
		Sleep(ROUND_INTERVAL);
		//�غ���Ϣ
		record += "\nRound : " + std::to_string(round) + "\n";
		std::cout << "\nRound : " << std::to_string(round) << std::endl;
		//a�����ٶȿ���b����a��¼��ƽ̨����b�����ȹ���
		if (poke_a->get_attack_interval_temp() >= poke_b->get_attack_interval_temp())
		{
			if (poke_a->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//��¼ս������Ϣ
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_a->attack_to(poke_b) + "\n";//ʵʩ������Ϊ
			}
			if (poke_b->get_hp_temp() <= 0)
				break;
			if (poke_b->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//��¼ս������Ϣ
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_b->attack_to(poke_a) + "\n";//ʵʩ������Ϊ
			}
		}
		//b�����ٶȿ���a
		else
		{
			if (poke_b->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//��¼ս������Ϣ
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_b->attack_to(poke_a) + "\n";//ʵʩ������Ϊ
			}
			if (poke_a->get_hp_temp() <= 0)
				break;
			if (poke_a->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//��¼ս������Ϣ
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_a->attack_to(poke_b) + "\n";//ʵʩ������Ϊ
			}
		}
		record += "--------------------------------------------------\n";
		std::cout << "--------------------------------------------------\n";
		//�����´ι���ʱ������
		poke_a->rtta_dec();
		poke_b->rtta_dec();
	}

	//ʤ��ȷ��
	if (poke_a->get_hp_temp() <= 0)
	{
		lost_poke = poke_a;
		winner = b;
		loser = a;

		//ս�����������齱������
		poke_b->exp_inc(BATTLE_EXP(poke_b->get_level(), poke_a->get_level()));
	}
	else
	{
		lost_poke = poke_b;
		winner = a;
		loser = b;

		//ս�����������齱������
		poke_a->exp_inc(BATTLE_EXP(poke_a->get_level(), poke_b->get_level()));
	}
	winner->win_battle(true);
	loser->win_battle(false);

	//����������㣬������Ϣ��Ҫ���û����룬ʹrecv��������
	//�û���ս
	//������
	if (!(winner->get_with_server() && loser->get_with_server()))
	{
		if (winner->get_user_up_request() && loser->get_user_duel_request())
		{
			winner_record = record + "Congratulations, you won the game.\n" + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game.\n" + "Please send anything to continue.";
		}
		//������
		else
		{
			//ת�ƾ���
			winner->add_poke(loser->remove_poke(lost_poke));

			//������Ϣ
			winner_record = record + "Congratulations, you won the game and recieved a pokemon.\n";
			winner_record += lost_poke->show_info() + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game and lost a pokemon.\n";
			loser_record += lost_poke->show_info() + "Please send anything to continue.";
		}

	}

	//��������ս
	if (winner->get_with_server() && loser->get_with_server())
	{
		//������
		if (winner->get_user_up_request() || loser->get_user_up_request())
		{
			winner_record = record + "Congratulations, you won the game.\n" + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game.\n" + "Please send anything to continue.";
		}
		//������
		else
		{
			//������Ϣ
			loser_record = record + "Sorry to inform you that you lost the game and lost a pokemon.\n";
			loser_record += "Please send the id of the pokemon blow to give up.\n";
			
			//�û����������ܣ�ϵͳ���ѡ���û�����ֻ�������û�ѡ����һ�ͳ�
			if (winner->get_username() == "Server")
			{
				loser->lost_select_from.clear();//����ձ�����ѡ������
				if (loser->get_poke_num() >= 3)//�����������Ͼ��飬���ѡ����
				{
					int id[3] = { -1, -1, -1 };//�����ʼֵ�������ѡ�����ظ�
					srand((int)time(0));// �����������
					for (int i = 0; i < 3;)//������
					{
						int result;
						result = rand() % loser->get_poke_num();
						if (result != id[0] && result != id[1] && result != id[2])//���ظ�
						{
							id[i] = result;
							i++;
						}
					}

					//����������ѡ��
					for (int i = 0; i < 3; i++)
					{
						loser->lost_select_from.push_back(id[i]);
					}
				}
				else//������������
				{
					int id[3] = { -1, -1, -1 };//�����ʼֵ�������ѡ�����ظ�
					srand((int)time(0));// �����������
					for (int i = 0; i < loser->get_poke_num();)//�м�������ѡ����
					{
						int result;
						result = rand() % loser->get_poke_num();
						if (result != id[0] && result != id[1] && result != id[2])//���ظ�
						{
							id[i] = result;
							i++;
						}
					}

					//����������ѡ��
					for (int i = 0; i < loser->get_poke_num(); i++)
					{
						loser->lost_select_from.push_back(id[i]);
					}
				}

				//��Ӻ�ѡ������Ϣ
				for (auto i = loser->lost_select_from.begin(); i != loser->lost_select_from.end(); i++)
				{
					loser_record += "id : " + std::to_string((*i)) + " " + loser->get_poke((*i))->show_info();
				}
			}
			else//�û���ʤ��ֱ�ӻ��սʤ����
			{
				//ת�ƾ���
				winner->add_poke(loser->remove_poke(lost_poke));

				//�μӹ�����Ϣ
				winner_record = record + "Congratulations, you won the game and recieved a pokemon.\n";
				winner_record += lost_poke->show_info() + "Please send anything to continue.";
			}
		}
	}

	//������Ϣ
	if (winner->get_socket())//�Ƿ���������ս����Ϣ
	{
		strcpy_s(sendbuf, winner_record.length() + 1, winner_record.c_str());
		send(*(winner->get_socket()), sendbuf, strlen(sendbuf) + 1, 0);
	}
	if (loser->get_socket())//�Ƿ�����ս����Ϣ
	{
		strcpy_s(sendbuf, loser_record.length() + 1, loser_record.c_str());
		send(*(loser->get_socket()), sendbuf, strlen(sendbuf) + 1, 0);
	}
	std::cout << "Battle results" << std::endl;
	std::cout << record << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Sending results to client " << a->get_username() << std::endl;
	std::cout << "Sending results to client " << b->get_username() << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	return 0;
}