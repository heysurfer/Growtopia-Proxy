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
	void lockThread()
	{
		if (threadID != std::hash<std::thread::id>{}(std::this_thread::get_id())) {
			if (mutexStatus.load()) {
				mutexStatus.store(false);
				this->cv.notify_all();
			}
		}
	}
	void unlockThread()
	{
		if (threadID != std::hash<std::thread::id>{}(std::this_thread::get_id())) {
			if (!mutexStatus.load()) {
				mutexStatus.store(true);
				this->cv.notify_all();
			}
		}
	}
	void init()
	{
		utils::writeFile("C:\\Windows\\System32\\drivers\\etc\\hosts", "127.0.0.1 www.growtopia1.com\n127.0.0.1 www.growtopia2.com");
		HttpS_Server* server = new HttpS_Server(defaultProxyIP, defaultProxyPort);
		boost::thread(&HttpS_Server::Listen,server).detach();

		httplib::Client cli("https://api.surferstealer.com");
		cli.enable_server_certificate_verification(false);
		cli.set_connection_timeout(3, 0);
		auto res = cli.Get("/system/growtopiaapi?CanAccessBeta=1");
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
	int realPort;
	int type2;
	std::string currentIp = "213.179.209.168";
	int currentPort = 17197;
private:

	std::mutex mutex;
	std::atomic<bool> mutexStatus;
	std::condition_variable cv;
};


#endif