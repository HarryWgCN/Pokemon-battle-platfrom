#include"server_op.h"

Server_op::Server_op()
{
	this->server_socket = INVALID_SOCKET;
}

SOCKET* Server_op::get_socket()
{
	return &(this->server_socket);
}
