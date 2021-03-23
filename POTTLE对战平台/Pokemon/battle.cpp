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

void wait_for_competitor()//等待匹配两个发出同一战斗模式用户的请求
{
	int match = 0;//当match为2时有匹配
	User* u[2];//匹配的两个用户

	while (true)
	{
		//挑战服务器
		//升级赛
		users_mutex.lock();//上锁
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//找到升级赛请求用户放入数组
			if ((*i)->get_with_server() == true && (*i)->get_user_up_request() == true)
			{
				u[0] = *i;			
				std::thread battle(battle_thread, u[0], u[0]->get_select_id(), server_user, u[0]->get_server_id());
				battle.join();
				u[0]->set_with_server(false);
				u[0]->set_user_up_request(false);
			}
		}
		users_mutex.unlock();//解锁

		//决斗赛
		users_mutex.lock();//上锁
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//找到升级赛请求用户放入数组
			if ((*i)->get_with_server() == true && (*i)->get_user_duel_request() == true)
			{
				u[0] = *i;
				std::thread battle(battle_thread, u[0], u[0]->get_select_id(), server_user, u[0]->get_server_id());
				battle.join();
				u[0]->set_user_duel_request(false);
				u[0]->set_with_server(false);

			}
		}
		users_mutex.unlock();//解锁

		//用户间对战
		//匹配升级赛请求
		match = 0;
		users_mutex.lock();//上锁
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//找到升级赛请求用户放入数组
			if ((*i)->get_user_up_request() == true)
			{
				u[match] = *i;
				match++;
			}
			//匹配
			if (match == 2)
				break;
		}
		users_mutex.unlock();//解锁
		//匹配成功，进行战斗
		if (match == 2)
		{
			std::thread battle(battle_thread, u[0], u[0]->get_select_id(), u[1], u[1]->get_select_id());
			battle.join();//等战斗线程完成，继续匹配请求
			u[0]->set_user_up_request(false);
			u[1]->set_user_up_request(false);
		}

		//匹配决斗赛请求
		match = 0;
		users_mutex.lock();//上锁
		for (auto i = users.begin(); i != users.end(); i++)
		{
			//找到决斗赛请求用户放入数组
			if ((*i)->get_user_duel_request() == true)
			{
				u[match] = *i;
				match++;
			}
			//匹配
			if (match == 2)
				break;
		}
		users_mutex.unlock();//解锁
		if (match == 2)
		{
			std::thread battle(battle_thread, u[0], u[0]->get_select_id(), u[1], u[1]->get_select_id());//开始战斗线程
			battle.join();//等战斗线程完成，继续匹配请求
			u[0]->set_user_duel_request(false);
			u[1]->set_user_duel_request(false);//战斗请求取消
		}
	}

}

int battle_thread(User* a, int a_id, User* b, int b_id)
{
	//初始化战斗参数，战斗对象，战况记录
	User *winner, *loser;
	Pokemon* lost_poke;//胜负对象
	char sendbuf[RECORD_SIZE];
	Pokemon* poke_a = a->get_poke(a_id);
	Pokemon* poke_b = b->get_poke(b_id);//选择精灵出站
	poke_a->battle_init();
	poke_b->battle_init();//战斗初始化
	std::string record = "\n";//战斗信息记录
	std::string winner_record, loser_record;//获胜方和失败方战斗信息不同

	//战斗以回合为单位进行
	for (int round = 1; poke_a->get_hp_temp() > 0 && poke_b->get_hp_temp() > 0; round++)
	{
		Sleep(ROUND_INTERVAL);
		//回合信息
		record += "\nRound : " + std::to_string(round) + "\n";
		std::cout << "\nRound : " << std::to_string(round) << std::endl;
		//a攻击速度快于b，或a登录入平台早于b，优先攻击
		if (poke_a->get_attack_interval_temp() >= poke_b->get_attack_interval_temp())
		{
			if (poke_a->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//记录战斗中信息
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_a->attack_to(poke_b) + "\n";//实施攻击行为
			}
			if (poke_b->get_hp_temp() <= 0)
				break;
			if (poke_b->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//记录战斗中信息
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_b->attack_to(poke_a) + "\n";//实施攻击行为
			}
		}
		//b攻击速度快于a
		else
		{
			if (poke_b->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//记录战斗中信息
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_b->attack_to(poke_a) + "\n";//实施攻击行为
			}
			if (poke_a->get_hp_temp() <= 0)
				break;
			if (poke_a->get_rtta() == 0)
			{
				record += poke_a->show_temp_info();
				record += poke_b->show_temp_info() + "\n\n";//记录战斗中信息
				std::cout << poke_a->show_temp_info();
				std::cout << poke_b->show_temp_info() << std::endl << std::endl;
				record += poke_a->attack_to(poke_b) + "\n";//实施攻击行为
			}
		}
		record += "--------------------------------------------------\n";
		std::cout << "--------------------------------------------------\n";
		//距离下次攻击时间缩短
		poke_a->rtta_dec();
		poke_b->rtta_dec();
	}

	//胜负确定
	if (poke_a->get_hp_temp() <= 0)
	{
		lost_poke = poke_a;
		winner = b;
		loser = a;

		//战斗结束，经验奖励分配
		poke_b->exp_inc(BATTLE_EXP(poke_b->get_level(), poke_a->get_level()));
	}
	else
	{
		lost_poke = poke_b;
		winner = a;
		loser = b;

		//战斗结束，经验奖励分配
		poke_a->exp_inc(BATTLE_EXP(poke_a->get_level(), poke_b->get_level()));
	}
	winner->win_battle(true);
	loser->win_battle(false);

	//比赛结果结算，传输信息（要求用户输入，使recv不阻塞）
	//用户对战
	//升级赛
	if (!(winner->get_with_server() && loser->get_with_server()))
	{
		if (winner->get_user_up_request() && loser->get_user_duel_request())
		{
			winner_record = record + "Congratulations, you won the game.\n" + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game.\n" + "Please send anything to continue.";
		}
		//决斗赛
		else
		{
			//转移精灵
			winner->add_poke(loser->remove_poke(lost_poke));

			//构造信息
			winner_record = record + "Congratulations, you won the game and recieved a pokemon.\n";
			winner_record += lost_poke->show_info() + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game and lost a pokemon.\n";
			loser_record += lost_poke->show_info() + "Please send anything to continue.";
		}

	}

	//服务器对战
	if (winner->get_with_server() && loser->get_with_server())
	{
		//升级赛
		if (winner->get_user_up_request() || loser->get_user_up_request())
		{
			winner_record = record + "Congratulations, you won the game.\n" + "Please send anything to continue.";
			loser_record = record + "Sorry to inform you that you lost the game.\n" + "Please send anything to continue.";
		}
		//决斗赛
		else
		{
			//构造信息
			loser_record = record + "Sorry to inform you that you lost the game and lost a pokemon.\n";
			loser_record += "Please send the id of the pokemon blow to give up.\n";
			
			//用户决斗被击败，系统随机选择用户的三只精灵由用户选择其一送出
			if (winner->get_username() == "Server")
			{
				loser->lost_select_from.clear();//先清空被击败选择向量
				if (loser->get_poke_num() >= 3)//有三个及以上精灵，随机选三个
				{
					int id[3] = { -1, -1, -1 };//数组初始值不与随机选择编号重复
					srand((int)time(0));// 产生随机种子
					for (int i = 0; i < 3;)//找三个
					{
						int result;
						result = rand() % loser->get_poke_num();
						if (result != id[0] && result != id[1] && result != id[2])//不重复
						{
							id[i] = result;
							i++;
						}
					}

					//填入向量供选择
					for (int i = 0; i < 3; i++)
					{
						loser->lost_select_from.push_back(id[i]);
					}
				}
				else//不足三个精灵
				{
					int id[3] = { -1, -1, -1 };//数组初始值不与随机选择编号重复
					srand((int)time(0));// 产生随机种子
					for (int i = 0; i < loser->get_poke_num();)//有几个精灵选几个
					{
						int result;
						result = rand() % loser->get_poke_num();
						if (result != id[0] && result != id[1] && result != id[2])//不重复
						{
							id[i] = result;
							i++;
						}
					}

					//填入向量供选择
					for (int i = 0; i < loser->get_poke_num(); i++)
					{
						loser->lost_select_from.push_back(id[i]);
					}
				}

				//添加候选精灵信息
				for (auto i = loser->lost_select_from.begin(); i != loser->lost_select_from.end(); i++)
				{
					loser_record += "id : " + std::to_string((*i)) + " " + loser->get_poke((*i))->show_info();
				}
			}
			else//用户获胜，直接获得战胜精灵
			{
				//转移精灵
				winner->add_poke(loser->remove_poke(lost_poke));

				//参加构造信息
				winner_record = record + "Congratulations, you won the game and recieved a pokemon.\n";
				winner_record += lost_poke->show_info() + "Please send anything to continue.";
			}
		}
	}

	//发送信息
	if (winner->get_socket())//非服务器发送战斗信息
	{
		strcpy_s(sendbuf, winner_record.length() + 1, winner_record.c_str());
		send(*(winner->get_socket()), sendbuf, strlen(sendbuf) + 1, 0);
	}
	if (loser->get_socket())//非服务器战斗信息
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