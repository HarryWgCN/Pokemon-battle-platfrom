#include <winsock2.h>                
#include<iostream>
#define BUF_LEN 1024
#define RECV_BUF_LEN (10 * BUF_LEN)
#pragma comment(lib,"ws2_32.lib") 

int main()
{

    int err;
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired = MAKEWORD(1, 1);
    //SOCKETǰ��һЩ��飬���Э���İ汾��Ϊ�˱����İ汾��socket������ͨ��  
    //WSAStartup������Ӧ�İ汾��WSAStartup�Ĳ���һ���ǰ汾��Ϣ��һ����һЩ��ϸ��ϸ��
    //WSAStartup��WSACleanup��Ӧ 

    err = WSAStartup(versionRequired, &wsaData);//Э���İ汾��Ϣ  
    if (!err)
    {
        printf("�ͻ���Ƕ�����Ѿ��� !\n");
    }
    else
    {
        printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
        return 0;//����  
    }//ͨ��WSACleanup�ķ���ֵ��ȷ��socketЭ���Ƿ�����  

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    //socket��������������socket��������ϵͳ��socket�����ͣ��Լ�һЩ������Ϣ  

    SOCKADDR_IN clientsock_in;
    clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    clientsock_in.sin_family = AF_INET;
    clientsock_in.sin_port = htons(6000);
    //socket����ж�����һ���ṹ��SOCKADDR_IN����������һЩ��Ϣ����socket��ϵͳ���˿ںţ�ip��ַ����Ϣ������洢���Ƿ������˵ļ��������Ϣ

    //׼��������ɺ�Ȼ��ʼ������׽������ӵ�Զ�̵ļ����  
    //Ҳ���ǵ�һ������  

    if (connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == -1)
    { 
        printf("Failed to connect the server\n");
        return 0;
    }//��������

    while (true)
    {
        char text[BUF_LEN], recvbuf[RECV_BUF_LEN];//���Ҫ���ͺͽ��յ�������
        printf("Please input.......................\n");
        std::cin >> text;//���뷢�͵�����
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << R"([Sending] ")" << text << R"(" to server)" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        send(clientSocket, text, BUF_LEN, 0);//���͵������
        err = recv(clientSocket, recvbuf, RECV_BUF_LEN, 0);
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << R"([Received] ")" << recvbuf << R"(" from server)" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
    }
    closesocket(clientSocket);//�ر��׽��� 
    WSACleanup();//�رշ���
    return 0;
}


