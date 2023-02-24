#ifndef Information_H
#define Information_H

class info
{
public:
	info()
	{
		LocalClient = new LocalPlayer;
		World = new _World;
		ENetManager = new _enetManager;
	}
	~info()
	{
		if (LocalClient)
			delete LocalClient;
		if (World)
			delete World;
		if (ENetManager)
			delete ENetManager;

	}
	LocalPlayer* LocalClient = nullptr;
	_World* World = nullptr;
	_enetManager* ENetManager = nullptr;
	ENetPeer* getPeer(getType type)
	{
		switch (type)
		{
		case Local:
			return this->ENetManager->Local_Peer;
		case Growtopia:
			return this->ENetManager->Growtopia_Peer;
		default:
			return nullptr;
		}
		
	}
	ENetHost* getHost(getType type)
	{
		switch (type)
		{
		case Local:
			return this->ENetManager->Local_Host;
		case Growtopia:
			return this->ENetManager->Growtopia_Host;
		default:
			return nullptr;
		}

	}
	void send_log(std::string text)
	{
		this->ENetManager->sendPacket("action|log\nmsg|" + text, getType::Local, NET_MESSAGE_GAME_MESSAGE);
	}
	void init()
	{
		#ifdef _WIN32
		utils::writeFile("C:\\Windows\\System32\\drivers\\etc\\hosts", "127.0.0.1 www.growtopia1.com\n127.0.0.1 www.growtopia2.com");
		#else
		utils::writeFile("/etc/hosts", "127.0.0.1 www.growtopia1.com\n127.0.0.1 www.growtopia2.com");
		#endif
		HttpS_Server* server = new HttpS_Server(defaultProxyIP, defaultProxyPort);
		std::thread(&HttpS_Server::Listen,server).detach();

		httplib::Client cli("https://api.surferwallet.net");
		cli.enable_server_certificate_verification(false);
		cli.set_connection_timeout(3, 0);
		auto res = cli.Get("/Growtopia");
		if (res.error() == httplib::Error::Success) {
			rtvar var = rtvar::parse(res->body);
			this->realIP = var.get("server");
			this->realPort = var.get_int("port");
			this->type2 = var.get_int("type2");
		}
		else
			Print("Requested Failed Error : %s", httplib::to_string(res.error()).c_str());

		ENetManager->createHost(getType::Growtopia);
		ENetManager->createHost(getType::Local);
	}
	const char* defaultProxyIP = "127.0.0.1";
	uint32_t defaultProxyPort = 17192;
	uint32_t threadID;
	std::string realIP;
	std::string captchaSolverKey;
	int realPort;
	int type2;
	std::string currentIp = "213.179.209.168";
	int currentPort = 17197;
private:
};


#endif