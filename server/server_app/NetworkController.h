#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF

#include <json/json.h>


class Server; // Server.h ��� �������� � ���� ���� h, ������� ������ ���, ����� �� ���� ��������

class NetworkController
{
	public:
		Server *server;
		NetworkController(Server *_server);
		__int64 tickCount;
		
};

#endif