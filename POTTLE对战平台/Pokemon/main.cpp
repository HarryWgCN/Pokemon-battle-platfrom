#include"header.h"
#include"User.h"
#include"Pokemon.h"
#include"Server_op.h"

std::vector<User*> users;//�û��������洢ϵͳע����û�
std::mutex users_mutex;//�û�������
User* server_user;//�������û�


int main()
{
	init_images();//��ʼ��ͼ��
	initgraph(GRAPH_WIDTH, GRAPH_HEIGHT, EW_SHOWCONSOLE);//��ʼ��ͼ�λ�����
	putimage(0, 0, &background);//������ͼ
	init_server_user();//��ʼ���û�����

	std::thread battle(wait_for_competitor);//�ȴ�ս��ƥ���߳�
	std::thread server(server_accept_thread);//���ܿͻ��������߳�
	server.join();//����
	return 0;
}

void server_accept_thread()//����˽��ܿͻ������������߳�
{
	std::cout << "Server accept thread created successfully" << std::endl;
	//��ʼ��
	int err = 0, erro = 0;//������Ϣ
	SOCKET* server_socket;
	Server_op server_op;
	server_socket = server_op.get_socket();//�����׽���
	err = InitServer(server_socket);
	if (err == 1)
	{
		std::cout << "Failed to initialize server." << std::endl;
		return ;
	}
	listen(*server_socket, 4);//������������

	//�������ӣ����������߳�
	while (1)
	{
		SOCKADDR_IN clientsocket;
		int len = sizeof(SOCKADDR);
		SOCKET* connect_socket;
		connect_socket = new SOCKET(1);
		*connect_socket = accept(*server_socket, (SOCKADDR*)&clientsocket, &len);
		if (*connect_socket == SOCKET_ERROR)
		{
			printf("Failed to connect��\n");
			WSACleanup();
			return;
		}
		else
		{
			std::cout << "Received connection request" << std::endl;
			//����ͻ�ͨ����Ϣ�߳���ӵ�������
			std::thread handle_thread(server_handle_thread, connect_socket);//�����߳�
			handle_thread.detach();//�����̣߳��̷߳��غ��Զ��ͷ���Դ
		}
	}

	//�ر��׽���
	closesocket(*server_socket);
	//�ر�winsocket��
	WSACleanup();
}

void server_handle_thread(SOCKET* connect_socket)//����˴���ͻ���ͨ����Ϣ�߳�
{
	std::cout << "Server handle thread created successfully" << std::endl;
	int err = 0, erro = 0;
	User* err_user = NULL;
	enum class MODE{wait_for_signup, wait_for_login, wait_for_username, wait_for_signup_password, wait_for_login_password,
		wait_for_command, wait_for_battle_operate,wait_for_select, wait_for_server_select, wait_for_battle_mode, wait_for_arg,
		wait_for_lost_select, in_battle};//��ǰϵͳ����ģʽ
	MODE mode = MODE::wait_for_signup;//��ʼΪ�ȴ�ע����¼
	enum class BATTLE_MODE{upgrade, duel};//ս��ģʽ
	BATTLE_MODE battle_mode;
	User* user = NULL;//�̶߳�Ӧ�û�
	int timing = NO_RESPONSE_LIMIT;//����ʱ�����û���ʱ��û�в�����ǿ�Ƶǳ�
	while (1)
	{
		char recvbuf[BUF_SIZE];//���ܻ���
		char sendbuf[BUF_SIZE];//���ͻ���
		int recvbuf_len = BUF_SIZE, sendbuf_len = BUF_SIZE;//����ռ��С
		struct sockaddr_in clientAddr;//�û���ַ
		int clientAddrLen = sizeof(clientAddr);//�û���ַ����

		err = recv(*connect_socket, recvbuf, recvbuf_len, 0);//������Ϣ,�������ؽ��ճ���
		if (err == SOCKET_ERROR) //�����쳣
		{

			erro = WSAGetLastError();											//��ȡ�쳣����
			if (erro == WSAEWOULDBLOCK)											//�������ݿɶ�
			{
				Sleep(20);
				timing -= 20;//����ʱ����
				if (timing <= 0)//����ʱ����
				{
					if (user != NULL)//�ѵ�¼
					{
						user->set_online(false);//����
					}
					return;//�����߳�
				}
				continue;
			}
			else//��ȡ����ʧ��
			{
				printf("Failed to receive from client.\n");
				break;
			}
		}
		else
		{
			timing = NO_RESPONSE_LIMIT;//����ʱ��ԭ
			if(user != NULL)
				std::cout << R"([Received] ")" << recvbuf << R"(" from client : )" << user->get_username() << std::endl;
			else
				std::cout << R"([Received] ")" << recvbuf << R"(" from client)" << std::endl;
			std::string request;
			request = recvbuf;
			
			//���ݵ�ǰ״̬ʶ���û����͵���Ϣ
			//�ȴ��û���¼
			if (mode == MODE::wait_for_signup)
			{
				if (request == "signup")//ע�Ტ��¼
				{
					strcpy_s(sendbuf, strlen("Please send your username in less than 10 letters") + 1, "Please send your username in less than 10 letters");
					std::cout << "Client would like to signup" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_username;
					continue;
				}
				if (request == "login")//��¼
				{
					strcpy_s(sendbuf, strlen("Please send your username.") + 1, "Please send your username.");
					std::cout << "Client would like to login" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_login;
					continue;
				}
				if (request == "exit")//�˳�ϵͳ
				{
					strcpy_s(sendbuf, strlen("Exit from POTTLE.......................") + 1, "Exit from POTTLE.......................");
					std::cout << "Client delivered username : " << request << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << request << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					return;//�˳��߳�
				}
				//�޷�ʶ��ָ��
				std::string send_string;
				send_string += "\nWelcome to pokemon battle platform : POTTLE!\n";
				send_string += R"(Please type "signup" or "login" to get your pokemons.)";
				send_string += "\nOr type";
				send_string += R"("exit" to exit from platform.)";
				strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�ȴ������¼�û�����¼
			if (mode == MODE::wait_for_login)
			{
				err_user = check_username(request);
				if (err_user != NULL && !err_user->get_online())//��¼�˺�ȷ����׼����֤����
				{
					user = err_user;
					strcpy_s(sendbuf, strlen("Please send your password") + 1, "Please send your password");
					std::cout << "Client : " << user->get_username() << " intend to login." << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client : )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_login_password;
					continue;
				}
				//��Ч�û���
				strcpy_s(sendbuf, strlen(R"(User name invalid.Please type "signup" or "login" to get your pokemons.)") + 1, R"(User not exist.Please type "signup" or "login" to get your pokemons.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_signup;
				continue;
			}

			//�ȴ��û������û���ע�Ტ��¼
			if (mode == MODE::wait_for_username)
			{
				//����û����Ϸ���
				err_user = check_username(request);
				if (err_user != NULL)//�Ƿ��û���
				{
					strcpy_s(sendbuf, strlen("Username occupied or illegal length. Please send your username in less than 10 letters again") + 1, "Username occupied or illegal length. Please send your username in less than 10 letters again");
					std::cout << "Client delivered illegal username" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )"  << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_username;
					continue;
				}
				strcpy_s(sendbuf, strlen("Please input your password.") + 1, "Please input your password.");
				std::cout << "Client delivered username : " << request <<std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << request << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				user = new User(request, connect_socket);
				users_mutex.lock();//����
				users.push_back(user);//����û�
				user->set_online(true);//����
				users_mutex.unlock();//����
				mode = MODE::wait_for_signup_password;
				continue;				
			}

			//�ȴ�����ע������
			if (mode == MODE::wait_for_signup_password)
			{
				//ע���û���������
				user->set_password(request);
				std::string send_string;
				send_string += "Signup successfully.\n";
				send_string += R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)";
				strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());				
				std::cout << "Client : " << user->get_username() << " sent password" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_command;
				continue;
			}

			//�ȴ������¼����
			if (mode == MODE::wait_for_login_password)
			{
				if (!user->match_password(request))//������ȷ
				{
					std::string send_string;
					send_string += "Login successfully.\n";
					send_string += R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)";
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "Client : " << user->get_username() << " logged in successfully" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client : )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					users_mutex.lock();//����
					users.push_back(user);//����û�
					user->set_online(true);//����
					users_mutex.unlock();//����
					mode = MODE::wait_for_command;
					continue;
				}
				//��Ч�û���
				strcpy_s(sendbuf, strlen("Wrong,password.Please send your username") + 1, "Wrong,password.Please send your username");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_login;
				continue;
			}
			
			//�ȴ��û�����������ָ��
			if (mode == MODE::wait_for_command)
			{
				if (request == "battle")//����ս��ϵͳ
				{
					strcpy_s(sendbuf, strlen(R"(Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)") + 1, R"(Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)");
					std::cout << "Client would like to login" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() <<std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_battle_operate;
					continue;
				}
				if (request == "info")//��ѯ������Ϣ
				{
					if (user == NULL)
					{
						strcpy_s(sendbuf, strlen("Failed to create new user") + 1, "Failed to create new user");
						std::cout << "Client would like to get pokemon info" << std::endl;
						std::cout << "--------------------------------------------------" << std::endl;
						std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
						std::cout << "--------------------------------------------------" << std::endl;
						send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
						return;
					}
					std::string info = user->get_user_info();
					strcpy_s(sendbuf, info.length() + 1, info.c_str());
					std::cout << "Client would like to get pokemon info" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					continue;
				}
				if (request == "exit")//�˳��û���¼״̬
				{
					strcpy_s(sendbuf, strlen("Exit from your account.......................") + 1, "Exit from your account.......................");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					
					//�˳��û���¼
					user->set_online(false);
					mode = MODE::wait_for_signup;
					continue;
				}
				//�޷�ʶ��ָ��
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Can't understand.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�ȴ��û�����ս������
			if (mode == MODE::wait_for_battle_operate)
			{
				if (request == "seek")//��ѯ�����û����������ս������
				{
					std::string send_string;
					send_string += "\nOther users online:\n";
					users_mutex.lock();//����
					for (auto i = users.begin(); i != users.end(); i++)
					{
						if ((*i)->get_username() != user->get_username())
						{
							send_string += (*i)->get_user_info();
						}
					}
					users_mutex.unlock();//����
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					continue;
				}
				if (request == "request")//ս������
				{
					strcpy_s(sendbuf, strlen(R"(Type "server" or "user" to battle with. Type "exit" to exit.)") + 1, R"(Type "server" or "user" to battle with. Type "exit" to exit.)");
					std::cout << "Client would like to battle" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_select;
					continue;
				}
				if (request == "exit")//�˳�
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				//�޷�ʶ��ָ��
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)") + 1, R"(Can't understand.Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�ȴ��û�ѡ���ս����
			if (mode == MODE::wait_for_select)
			{
				if (request == "server")
				{
					std::string send_string;
					send_string += "\nPlese select which pokemon from server you want to battle with\n";
					send_string += server_user->show_poke_info();
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "Client would like to battle with server" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					user->set_with_server(true);
					mode = MODE::wait_for_server_select;
					continue;
				}
				if (request == "user")
				{
					std::string send_string;
					send_string += "\nIn upgrade mode, you will fight to gain experience.\n";
					send_string += "In duel mode, which is about the same but the winner also get a poke from the server.\n";
					send_string += R"(Type "upgrade" or "duel" to enroll in.Type "exit" to exit.)";
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "Client would like to battle with user" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					user->set_with_server(false);
					mode = MODE::wait_for_battle_mode;
					continue;
				}
				//�޷�ʶ��ָ��
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "server" or "user" to battle with.Type "exit" to exit.)") + 1, R"(Can't understand.Type "server" or "user" to battle with.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�ȴ��û�ѡ�������������֮��ս
			if (mode == MODE::wait_for_server_select)
			{
				int id;
				sscanf_s(request.c_str(), "%d", &id);
				if (server_user->get_poke_num() == 0)//�������޾���ɳ�ս
				{
					strcpy_s(sendbuf, strlen(R"(No Pokemons left in server.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(No Pokemons left in server.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "No Pokemons left in server" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << server_user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				if (id >= 0 && id <= server_user->get_poke_num())//ѡ����id����
				{
					std::string send_string;
					send_string += "\nIn upgrade mode, you will fight to gain experience.\n";
					send_string += "In duel mode, which is about the same but the winner also get a poke from the loser.\n";
					send_string += R"(Type "upgrade" or "duel" to enroll in.Type "exit" to exit.)";
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "Client chose server pokemon id : " <<  id <<std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					user->set_server_id(id);
					user->set_with_server(true);
					mode = MODE::wait_for_battle_mode;
					continue;
				}
				if (request == "exit")//�˻�wait_for_command
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				//�޷�ʶ��ָ������û����·���ѡ������
				std::string info = "Invalid id.Select pokemon from server and send the id\nPokemons:\n" + server_user->show_poke_info();
				strcpy_s(sendbuf, info.length() + 1, info.c_str());
				std::cout << "Invalid server_select" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_server_select;
				continue;
			}

			//�ȴ��û�ѡ��ս��ģʽ
			if (mode == MODE::wait_for_battle_mode)
			{
				if (request == "upgrade")//������
				{
					std::string info = "\nYou have entered the upgrade battle queue\nwaiting for competitor.......................\n";
					info = info + "Select your pokemon and send the id\nPokemons:\n" + user->show_poke_info();
					strcpy_s(sendbuf, info.length() + 1, info.c_str());
					std::cout << "Client would like to enroll in upgrade battle" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					battle_mode = BATTLE_MODE::upgrade;
					mode = MODE::wait_for_arg;
					continue;
				}
				if (request == "duel")//������
				{
					std::string info = "\nYou have entered the upgrade battle queue\nwaiting for competitor.......................\n";
					info = info + "Select your pokemon and send the id\nPokemons:\n" + user->show_poke_info();
					strcpy_s(sendbuf, info.length() + 1, info.c_str());					
					std::cout << "Client would like to enroll in duel battle" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_arg;
					battle_mode = BATTLE_MODE::duel;
					continue;
				}
				if (request == "exit")//�˻�wait_for_command
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				//�޷�ʶ��ָ��
				std::string send_string;
				send_string += "\nCan't understand.\nIn upgrade mode, you will fight to gain experience.\n";
				send_string += "In duel mode, which is about the same but the winner also get a poke from the other.\n";
				send_string += R"(Type "upgrade" or "duel" to enroll in.Type "exit" to exit.)";
				strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
				std::cout << "Client would like to battle" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�ȴ��û�ѡ�����ս
			if (mode == MODE::wait_for_arg)
			{
				int id;
				sscanf_s(request.c_str(), "%d", &id);
				if (user->get_poke_num() == 0)//�޾���ɳ�ս
				{
					strcpy_s(sendbuf, strlen(R"(No Pokemons left.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(No Pokemons left.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "No Pokemons left" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				if (id >= 0 && id <= user->get_poke_num())//ѡ����id����
				{
					user->set_select_id(id);
					std::cout << "Client chose "  << id << " to battle" << std::endl;
					if (battle_mode == BATTLE_MODE::duel)
						user->set_user_duel_request(true);
					if (battle_mode == BATTLE_MODE::upgrade)
						user->set_user_up_request(true);
					if (user->get_with_server())
					{
						mode = MODE::wait_for_lost_select;
						continue;
					}
					mode = MODE::wait_for_command;//���߳�������recv����������Ϣ��client��client������recv״̬
					continue;
				}
				if (request == "exit")//�˻�wait_for_command
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				//�޷�ʶ��ָ������û����·���ѡ������
				std::string info = "Invalid id.Select your pokemon and send the id\nPokemons:\n" + user->show_poke_info();
				strcpy_s(sendbuf, info.length() + 1, info.c_str());					
				std::cout << "Client would like to enroll in duel battle" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_arg;
				continue;
			}

			//�ȴ������������ʧ�ܺ�ѡ���ͳ�����
			if (mode == MODE::wait_for_lost_select)
			{
				int id;
				sscanf_s(request.c_str(), "%d", &id);
				if (user->lost_select_from.empty())
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				if (!in_lost_select(user->lost_select_from, id) && !user->lost_select_from.empty())//ѡ����id����
				{
					std::string send_string;
					send_string += "\nYou have lost\n";
					send_string += user->get_poke(id)->show_info();
					send_string += "Send anything to continue";
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					server_user->add_poke(user->remove_poke(user->get_poke(id)));
					std::cout << "Client chose lost_select pokemon id" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					mode = MODE::wait_for_command;
					continue;
				}
				//�޷�ʶ��ָ������û����·���ѡ������
				std::string info = "Invalid id.Select pokemon from the list and send the id\nPokemons:\n";
				for (auto i = user->lost_select_from.begin(); i != user->lost_select_from.end(); i++)
				{
					info += "id : " + std::to_string((*i)) + user->get_poke((*i))->show_info();
				}
				strcpy_s(sendbuf, info.length() + 1, info.c_str());
				std::cout << "Invalid lost_select" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//�û�����ѡ����id����߳̽�������recv���ڽ���ս����ֱ�ӽ���in_battleģʽ
			//�ȴ���ս��
			/*if (mode == MODE::wait_for_competitor)
			{
				if (request == "exit")//�˻�wait_for_command
				{
					strcpy_s(sendbuf, strlen(R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					user->set_user_duel_request(false);
					user->set_user_up_request(false);
					mode = MODE::wait_for_command;
					continue;
				}
				strcpy_s(sendbuf, strlen(R"(Waiting for competitor.Type "exit" to exit.)") + 1, R"(Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
			}*/

			//ս��˲�����
			//���ڶ�ս�����˳�
			/*if (mode == MODE::in_battle)
			{
				strcpy_s(sendbuf, strlen("WARNING [IN BATTLE].No operation is allowed") + 1, "WARNING [IN BATTLE].No operation is allowed");
				std::cout << "Client sent something while in battle" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
			}*/
		}
	}
}

User* check_username(std::string username)//User����Ԫ����������û����Ƿ���ڣ����Ѵ��ڷ����û�ָ�룬���򷵻ؿ�ָ��
{
	users_mutex.lock();//����
	for (auto i = users.begin(); i != users.end(); i++)
	{
		if ((*i)->get_username() == username)
		{
			users_mutex.unlock();//����
			return (*i);
		}	
	}
	users_mutex.unlock();//����
	return NULL;
}

int in_lost_select(std::vector<int> v, int id)//User����Ԫ������������Ƿ��ڷ���������ʧ����ѡ�ͳ������б���
{
	for (auto i = v.begin(); i != v.end(); i++)
	{
		if ((*i) == id)
		{
			return 0;
		}
	}
	return 1;
}