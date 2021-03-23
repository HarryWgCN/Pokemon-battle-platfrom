#include"header.h"
#include"User.h"
#include"Pokemon.h"
#include"Server_op.h"

std::vector<User*> users;//用户向量，存储系统注册的用户
std::mutex users_mutex;//用户向量锁
User* server_user;//服务器用户


int main()
{
	init_images();//初始化图像
	initgraph(GRAPH_WIDTH, GRAPH_HEIGHT, EW_SHOWCONSOLE);//初始化图形化界面
	putimage(0, 0, &background);//背景地图
	init_server_user();//初始化用户向量

	std::thread battle(wait_for_competitor);//等待战斗匹配线程
	std::thread server(server_accept_thread);//接受客户端连接线程
	server.join();//阻塞
	return 0;
}

void server_accept_thread()//服务端接受客户端连接请求线程
{
	std::cout << "Server accept thread created successfully" << std::endl;
	//初始化
	int err = 0, erro = 0;//返回信息
	SOCKET* server_socket;
	Server_op server_op;
	server_socket = server_op.get_socket();//监听套接字
	err = InitServer(server_socket);
	if (err == 1)
	{
		std::cout << "Failed to initialize server." << std::endl;
		return ;
	}
	listen(*server_socket, 4);//监听连接请求

	//接受连接，创建处理线程
	while (1)
	{
		SOCKADDR_IN clientsocket;
		int len = sizeof(SOCKADDR);
		SOCKET* connect_socket;
		connect_socket = new SOCKET(1);
		*connect_socket = accept(*server_socket, (SOCKADDR*)&clientsocket, &len);
		if (*connect_socket == SOCKET_ERROR)
		{
			printf("Failed to connect！\n");
			WSACleanup();
			return;
		}
		else
		{
			std::cout << "Received connection request" << std::endl;
			//处理客户通信信息线程添加到向量里
			std::thread handle_thread(server_handle_thread, connect_socket);//创建线程
			handle_thread.detach();//剥离线程，线程返回后自动释放资源
		}
	}

	//关闭套接字
	closesocket(*server_socket);
	//关闭winsocket库
	WSACleanup();
}

void server_handle_thread(SOCKET* connect_socket)//服务端处理客户端通信信息线程
{
	std::cout << "Server handle thread created successfully" << std::endl;
	int err = 0, erro = 0;
	User* err_user = NULL;
	enum class MODE{wait_for_signup, wait_for_login, wait_for_username, wait_for_signup_password, wait_for_login_password,
		wait_for_command, wait_for_battle_operate,wait_for_select, wait_for_server_select, wait_for_battle_mode, wait_for_arg,
		wait_for_lost_select, in_battle};//当前系统所处模式
	MODE mode = MODE::wait_for_signup;//初始为等待注册或登录
	enum class BATTLE_MODE{upgrade, duel};//战斗模式
	BATTLE_MODE battle_mode;
	User* user = NULL;//线程对应用户
	int timing = NO_RESPONSE_LIMIT;//倒计时器，用户长时间没有操作会强制登出
	while (1)
	{
		char recvbuf[BUF_SIZE];//接受缓存
		char sendbuf[BUF_SIZE];//发送缓存
		int recvbuf_len = BUF_SIZE, sendbuf_len = BUF_SIZE;//缓存空间大小
		struct sockaddr_in clientAddr;//用户地址
		int clientAddrLen = sizeof(clientAddr);//用户地址长度

		err = recv(*connect_socket, recvbuf, recvbuf_len, 0);//接收信息,函数返回接收长度
		if (err == SOCKET_ERROR) //接收异常
		{

			erro = WSAGetLastError();											//获取异常类型
			if (erro == WSAEWOULDBLOCK)											//暂无数据可读
			{
				Sleep(20);
				timing -= 20;//倒计时继续
				if (timing <= 0)//倒计时结束
				{
					if (user != NULL)//已登录
					{
						user->set_online(false);//下线
					}
					return;//返回线程
				}
				continue;
			}
			else//读取数据失败
			{
				printf("Failed to receive from client.\n");
				break;
			}
		}
		else
		{
			timing = NO_RESPONSE_LIMIT;//倒计时复原
			if(user != NULL)
				std::cout << R"([Received] ")" << recvbuf << R"(" from client : )" << user->get_username() << std::endl;
			else
				std::cout << R"([Received] ")" << recvbuf << R"(" from client)" << std::endl;
			std::string request;
			request = recvbuf;
			
			//根据当前状态识别用户发送的信息
			//等待用户登录
			if (mode == MODE::wait_for_signup)
			{
				if (request == "signup")//注册并登录
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
				if (request == "login")//登录
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
				if (request == "exit")//退出系统
				{
					strcpy_s(sendbuf, strlen("Exit from POTTLE.......................") + 1, "Exit from POTTLE.......................");
					std::cout << "Client delivered username : " << request << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << request << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					return;//退出线程
				}
				//无法识别指令
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

			//等待输入登录用户名登录
			if (mode == MODE::wait_for_login)
			{
				err_user = check_username(request);
				if (err_user != NULL && !err_user->get_online())//登录账号确定，准备验证密码
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
				//无效用户名
				strcpy_s(sendbuf, strlen(R"(User name invalid.Please type "signup" or "login" to get your pokemons.)") + 1, R"(User not exist.Please type "signup" or "login" to get your pokemons.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_signup;
				continue;
			}

			//等待用户输入用户名注册并登录
			if (mode == MODE::wait_for_username)
			{
				//检查用户名合法性
				err_user = check_username(request);
				if (err_user != NULL)//非法用户名
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
				users_mutex.lock();//上锁
				users.push_back(user);//添加用户
				user->set_online(true);//上线
				users_mutex.unlock();//解锁
				mode = MODE::wait_for_signup_password;
				continue;				
			}

			//等待输入注册密码
			if (mode == MODE::wait_for_signup_password)
			{
				//注册用户设置密码
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

			//等待输入登录密码
			if (mode == MODE::wait_for_login_password)
			{
				if (!user->match_password(request))//密码正确
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
					users_mutex.lock();//上锁
					users.push_back(user);//添加用户
					user->set_online(true);//上线
					users_mutex.unlock();//解锁
					mode = MODE::wait_for_command;
					continue;
				}
				//无效用户名
				strcpy_s(sendbuf, strlen("Wrong,password.Please send your username") + 1, "Wrong,password.Please send your username");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				mode = MODE::wait_for_login;
				continue;
			}
			
			//等待用户输入主界面指令
			if (mode == MODE::wait_for_command)
			{
				if (request == "battle")//进入战斗系统
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
				if (request == "info")//查询个人信息
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
				if (request == "exit")//退出用户登录状态
				{
					strcpy_s(sendbuf, strlen("Exit from your account.......................") + 1, "Exit from your account.......................");
					std::cout << "Client would like to exit" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					
					//退出用户登录
					user->set_online(false);
					mode = MODE::wait_for_signup;
					continue;
				}
				//无法识别指令
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)") + 1, R"(Can't understand.Type "battle" to battle.Type "info" to see your pokemons.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//等待用户输入战斗操作
			if (mode == MODE::wait_for_battle_operate)
			{
				if (request == "seek")//查询其他用户在线情况和战斗意向
				{
					std::string send_string;
					send_string += "\nOther users online:\n";
					users_mutex.lock();//上锁
					for (auto i = users.begin(); i != users.end(); i++)
					{
						if ((*i)->get_username() != user->get_username())
						{
							send_string += (*i)->get_user_info();
						}
					}
					users_mutex.unlock();//解锁
					strcpy_s(sendbuf, send_string.length() + 1, send_string.c_str());
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << R"([Sending] ")" << sendbuf << R"(" to client: )" << user->get_username() << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
					continue;
				}
				if (request == "request")//战斗请求
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
				if (request == "exit")//退出
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
				//无法识别指令
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)") + 1, R"(Can't understand.Type "seek" to find users.Type "request" to battle.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//等待用户选择对战对象
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
				//无法识别指令
				strcpy_s(sendbuf, strlen(R"(Can't understand.Type "server" or "user" to battle with.Type "exit" to exit.)") + 1, R"(Can't understand.Type "server" or "user" to battle with.Type "exit" to exit.)");
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << R"([Sending] ")" << sendbuf << R"(" to client)" << user->get_username() << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				send(*connect_socket, sendbuf, strlen(sendbuf) + 1, 0);
				continue;
			}

			//等待用户选择服务器精灵与之对战
			if (mode == MODE::wait_for_server_select)
			{
				int id;
				sscanf_s(request.c_str(), "%d", &id);
				if (server_user->get_poke_num() == 0)//服务器无精灵可出战
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
				if (id >= 0 && id <= server_user->get_poke_num())//选择精灵id可用
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
				if (request == "exit")//退回wait_for_command
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
				//无法识别指令，请求用户重新发送选择精灵编号
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

			//等待用户选择战斗模式
			if (mode == MODE::wait_for_battle_mode)
			{
				if (request == "upgrade")//升级赛
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
				if (request == "duel")//决斗赛
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
				if (request == "exit")//退回wait_for_command
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
				//无法识别指令
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

			//等待用户选择精灵出战
			if (mode == MODE::wait_for_arg)
			{
				int id;
				sscanf_s(request.c_str(), "%d", &id);
				if (user->get_poke_num() == 0)//无精灵可出战
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
				if (id >= 0 && id <= user->get_poke_num())//选择精灵id可用
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
					mode = MODE::wait_for_command;//此线程阻塞于recv，不发送信息至client，client阻塞于recv状态
					continue;
				}
				if (request == "exit")//退回wait_for_command
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
				//无法识别指令，请求用户重新发送选择精灵编号
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

			//等待与服务器决斗失败后选择送出精灵
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
				if (!in_lost_select(user->lost_select_from, id) && !user->lost_select_from.empty())//选择精灵id可用
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
				//无法识别指令，请求用户重新发送选择精灵编号
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

			//用户发送选择精灵id后此线程将阻塞于recv，在进入战斗后直接进入in_battle模式
			//等待对战者
			/*if (mode == MODE::wait_for_competitor)
			{
				if (request == "exit")//退回wait_for_command
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

			//战斗瞬间结束
			//正在对战不能退出
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

User* check_username(std::string username)//User类友元函数，检查用户名是否存在，若已存在返回用户指针，否则返回空指针
{
	users_mutex.lock();//上锁
	for (auto i = users.begin(); i != users.end(); i++)
	{
		if ((*i)->get_username() == username)
		{
			users_mutex.unlock();//解锁
			return (*i);
		}	
	}
	users_mutex.unlock();//解锁
	return NULL;
}

int in_lost_select(std::vector<int> v, int id)//User类友元函数，检查编号是否在服务器决斗失败挑选送出精灵列表中
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