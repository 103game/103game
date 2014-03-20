#ifndef NETWORKCONTROLLER_CLASS_DEF
#define NETWORKCONTROLLER_CLASS_DEF


class Server; // Server.h уже включает в себя этот h, поэтому делаем так, чтобы не было рекурсии

class NetworkController
{
	public:
		Server *server;
		NetworkController(Server *_server);
		__int64 tickCount;
		
};

#endif