#include"header.h"
#include"User.h"
#include"Pokemon.h"

std::vector<IMAGE*>poke_attack_images;
std::vector<IMAGE*>poke_attack_images_cover;
std::vector<IMAGE*>poke_defense_images;
std::vector<IMAGE*>poke_defense_images_cover;

IMAGE background;
IMAGE Charmeleon_defense_image;
IMAGE Charmeleon_defense_image_cover;
IMAGE Charizard_defense_image;
IMAGE Charizard_defense_image_cover;
IMAGE Geodude_defense_image;
IMAGE Geodude_defense_image_cover;
IMAGE Graveler_defense_image;
IMAGE Graveler_defense_image_cover;
IMAGE Shellder_defense_image;
IMAGE Shellder_defense_image_cover;
IMAGE Cloyster_defense_image;
IMAGE Cloyster_defense_image_cover;
IMAGE Pikachu_defense_image;
IMAGE Pikachu_defense_image_cover;
IMAGE Raichu_defense_image;
IMAGE Raichu_defense_image_cover;
IMAGE Charmeleon_attack_image;
IMAGE Charmeleon_attack_image_cover;
IMAGE Charizard_attack_image;
IMAGE Charizard_attack_image_cover;
IMAGE Geodude_attack_image;
IMAGE Geodude_attack_image_cover;
IMAGE Graveler_attack_image;
IMAGE Graveler_attack_image_cover;
IMAGE Shellder_attack_image;
IMAGE Shellder_attack_image_cover;
IMAGE Cloyster_attack_image;
IMAGE Cloyster_attack_image_cover;
IMAGE Pikachu_attack_image;
IMAGE Pikachu_attack_image_cover;
IMAGE Raichu_attack_image;
IMAGE Raichu_attack_image_cover;

IMAGE lightning_image;
IMAGE fire_image;
IMAGE rocks_image;
IMAGE punch_image;
IMAGE lightning_image_cover;
IMAGE fire_image_cover;
IMAGE rocks_image_cover;
IMAGE punch_image_cover;
IMAGE critical_image;
IMAGE dodged_image;
IMAGE critical_image_cover;
IMAGE dodged_image_cover;

int InitServer(SOCKET* server_socket)//启动WSA加载套接字库,创建服务套接字并绑定端口
{
	WORD versionRequested = MAKEWORD(1, 1);
	WSADATA wsa_data;
	int err = WSAStartup(versionRequested, &wsa_data);//加载套接字库返回信息至err
	if (err != FALSE)
	{
		printf("WinSock failed to initialize\n");
		WSACleanup();
		return 1;
	}
	printf("WinSock initialized succesfully\n");

	*server_socket = socket(AF_INET, SOCK_STREAM, 0);//创建服务器的监听套接字
	if (*server_socket == INVALID_SOCKET)
	{
		printf("Socket creation failed\n");
		WSACleanup();
		return 0;
	}
	printf("Socket created successfully\n");

	SOCKADDR_IN server_addr;//定义server发送和接收数据包的地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//均为网络字节顺序

	err = bind(*server_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));//绑定套接字
	if (err == SOCKET_ERROR)
	{
		printf("Binding failed with error: %d\n", err);
		WSACleanup();
		return 1;
	}
	printf("Binding successfully\n");
	return 0;
}

void init_server_user()//初始化服务器用户，用于和服务器对战
{
	Pokemon* poke;
	server_user = new User(std::string("Server"), NULL);//创建服务器用户
	poke = server_user->get_poke(0);
	server_user->set_with_server(true);
	//设置三只精灵等级
	if (poke != NULL)
	{
		for (int i = 0; i < 10; i++)
			poke->exp_inc(0);//1级
	}
	poke = server_user->get_poke(1);
	if (poke != NULL)
	{
		for (int i = 0; i < 20; i++)
			poke->exp_inc(400);//5级
	}
	poke = server_user->get_poke(2);
	if (poke != NULL)
	{
		for (int i = 0; i < 30; i++)
			poke->exp_inc(1400);//15级
	}
}

void init_images()//初始化图像信息
{
	//战斗背景
	loadimage(&background, _T("./image/background.png"));

	//战斗防御方图像
	loadimage(&Charmeleon_defense_image, _T("./image/Charmeleon_defense.png"));
	poke_defense_images.push_back(&Charmeleon_defense_image);
	loadimage(&Charmeleon_defense_image_cover, _T("./image/Charmeleon_defense_cover.png"));
	poke_defense_images_cover.push_back(&Charmeleon_defense_image_cover);
	loadimage(&Charizard_defense_image, _T("./image/Charizard_defense.png"));
	poke_defense_images.push_back(&Charizard_defense_image);
	loadimage(&Charizard_defense_image_cover, _T("./image/Charizard_defense_cover.png"));
	poke_defense_images_cover.push_back(&Charizard_defense_image_cover);
	loadimage(&Geodude_defense_image, _T("./image/Geodude_defense.png"));
	poke_defense_images.push_back(&Geodude_defense_image);
	loadimage(&Geodude_defense_image_cover, _T("./image/Geodude_defense_cover.png"));
	poke_defense_images_cover.push_back(&Geodude_defense_image_cover);
	loadimage(&Graveler_defense_image, _T("./image/Graveler_defense.png"));
	poke_defense_images.push_back(&Graveler_defense_image);
	loadimage(&Graveler_defense_image_cover, _T("./image/Graveler_defense_cover.png"));
	poke_defense_images_cover.push_back(&Graveler_defense_image_cover);
	loadimage(&Shellder_defense_image, _T("./image/Shellder_defense.png"));
	poke_defense_images.push_back(&Shellder_defense_image);
	loadimage(&Shellder_defense_image_cover, _T("./image/Shellder_defense_cover.png"));
	poke_defense_images_cover.push_back(&Shellder_defense_image_cover);
	loadimage(&Cloyster_defense_image, _T("./image/Cloyster_defense.png"));
	poke_defense_images.push_back(&Cloyster_defense_image);
	loadimage(&Cloyster_defense_image_cover, _T("./image/Cloyster_defense_cover.png"));
	poke_defense_images_cover.push_back(&Cloyster_defense_image_cover);
	loadimage(&Pikachu_defense_image, _T("./image/Pikachu_defense.png"));
	poke_defense_images.push_back(&Pikachu_defense_image);
	loadimage(&Pikachu_defense_image_cover, _T("./image/Pikachu_defense_cover.png"));
	poke_defense_images_cover.push_back(&Pikachu_defense_image_cover);
	loadimage(&Raichu_defense_image, _T("./image/Raichu_defense.png"));
	poke_defense_images.push_back(&Raichu_defense_image);
	loadimage(&Raichu_defense_image_cover, _T("./image/Raichu_defense_cover.png"));
	poke_defense_images_cover.push_back(&Raichu_defense_image_cover);

	//战斗攻击方图像
	loadimage(&Charmeleon_attack_image, _T("./image/Charmeleon_attack.png"));
	poke_attack_images.push_back(&Charmeleon_attack_image);
	loadimage(&Charmeleon_attack_image_cover, _T("./image/Charmeleon_attack_cover.png"));
	poke_attack_images_cover.push_back(&Charmeleon_attack_image_cover);
	loadimage(&Charizard_attack_image, _T("./image/Charizard_attack.png"));
	poke_attack_images.push_back(&Charizard_attack_image);
	loadimage(&Charizard_attack_image_cover, _T("./image/Charizard_attack_cover.png"));
	poke_attack_images_cover.push_back(&Charizard_attack_image_cover);
	loadimage(&Geodude_attack_image, _T("./image/Geodude_attack.png"));
	poke_attack_images.push_back(&Geodude_attack_image);
	loadimage(&Geodude_attack_image_cover, _T("./image/Geodude_attack_cover.png"));
	poke_attack_images_cover.push_back(&Geodude_attack_image_cover);
	loadimage(&Graveler_attack_image, _T("./image/Graveler_attack.png"));
	poke_attack_images.push_back(&Graveler_attack_image);
	loadimage(&Graveler_attack_image_cover, _T("./image/Graveler_attack_cover.png"));
	poke_attack_images_cover.push_back(&Graveler_attack_image_cover);
	loadimage(&Shellder_attack_image, _T("./image/Shellder_attack.png"));
	poke_attack_images.push_back(&Shellder_attack_image);
	loadimage(&Shellder_attack_image_cover, _T("./image/Shellder_attack_cover.png"));
	poke_attack_images_cover.push_back(&Shellder_attack_image_cover);
	loadimage(&Cloyster_attack_image, _T("./image/Cloyster_attack.png"));
	poke_attack_images.push_back(&Cloyster_attack_image);
	loadimage(&Cloyster_attack_image_cover, _T("./image/Cloyster_attack_cover.png"));
	poke_attack_images_cover.push_back(&Cloyster_attack_image_cover);
	loadimage(&Pikachu_attack_image, _T("./image/Pikachu_attack.png"));
	poke_attack_images.push_back(&Pikachu_attack_image);
	loadimage(&Pikachu_attack_image_cover, _T("./image/Pikachu_attack_cover.png"));
	poke_attack_images_cover.push_back(&Pikachu_attack_image_cover);
	loadimage(&Raichu_attack_image, _T("./image/Raichu_attack.png"));
	poke_attack_images.push_back(&Raichu_attack_image);
	loadimage(&Raichu_attack_image_cover, _T("./image/Raichu_attack_cover.png"));
	poke_attack_images_cover.push_back(&Raichu_attack_image_cover);

	//攻击特效图像
	loadimage(&lightning_image, _T("./image/lightning.png"));
	loadimage(&lightning_image_cover, _T("./image/lightning_cover.png"));
	loadimage(&fire_image, _T("./image/fire.png"));
	loadimage(&fire_image_cover, _T("./image/fire_cover.png"));
	loadimage(&rocks_image, _T("./image/rocks.png"));
	loadimage(&rocks_image_cover, _T("./image/rocks_cover.png"));
	loadimage(&punch_image, _T("./image/punch.png"));
	loadimage(&punch_image_cover, _T("./image/punch_cover.png"));
	loadimage(&critical_image, _T("./image/critical.png"));
	loadimage(&critical_image_cover, _T("./image/critical_cover.png"));
	loadimage(&dodged_image, _T("./image/dodged.png"));
	loadimage(&dodged_image_cover, _T("./image/dodged_cover.png"));
}
