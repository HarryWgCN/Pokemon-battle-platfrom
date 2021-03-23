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
    //SOCKET前的一些检查，检查协议库的版本，为了避免别的版本的socket，并且通过  
    //WSAStartup启动对应的版本，WSAStartup的参数一个是版本信息，一个是一些详细的细节
    //WSAStartup与WSACleanup对应 

    err = WSAStartup(versionRequired, &wsaData);//协议库的版本信息  
    if (!err)
    {
        printf("客户端嵌套字已经打开 !\n");
    }
    else
    {
        printf("客户端的嵌套字打开失败!\n");
        return 0;//结束  
    }//通过WSACleanup的返回值来确定socket协议是否启动  

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    //socket三个参数定义了socket的所处的系统，socket的类型，以及一些其他信息  

    SOCKADDR_IN clientsock_in;
    clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    clientsock_in.sin_family = AF_INET;
    clientsock_in.sin_port = htons(6000);
    //socket编程中定义了一个结构体SOCKADDR_IN来存计算机的一些信息，像socket的系统，端口号，ip地址等信息，这里存储的是服务器端的计算机的信息

    //准备工作完成后，然后开始将这个套接字链接到远程的计算机  
    //也就是第一次握手  

    if (connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == -1)
    { 
        printf("Failed to connect the server\n");
        return 0;
    }//测试连接

    while (true)
    {
        char text[BUF_LEN], recvbuf[RECV_BUF_LEN];//存放要发送和接收到的内容
        printf("Please input.......................\n");
        std::cin >> text;//输入发送的内容
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << R"([Sending] ")" << text << R"(" to server)" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        send(clientSocket, text, BUF_LEN, 0);//发送到服务端
        err = recv(clientSocket, recvbuf, RECV_BUF_LEN, 0);
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << R"([Received] ")" << recvbuf << R"(" from server)" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
    }
    closesocket(clientSocket);//关闭套接字 
    WSACleanup();//关闭服务
    return 0;
}


