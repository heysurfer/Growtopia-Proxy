class serverHandle
{
public:
	serverHandle()
	{
		init();
	}
	void poll();
	void local_handle();
	void server_handle();

	bool onSendParticleEffect(ENetPacket *packet,getType type);
	bool onModifyInventory(ENetPacket* packet, getType type);
	bool onItemChangeObject(ENetPacket* packet, getType type);
	bool onTileChangeRequest(ENetPacket* packet, getType type);
	bool onSendInventoryState(ENetPacket* packet, getType type);
	bool onState(ENetPacket* packet, getType type);
	bool onSendMapState(ENetPacket* packet, getType type);
	bool onCallFunction(ENetPacket* packet, getType type);
	bool onTrack(ENetPacket* packet, getType type);
	bool blockPacket(ENetPacket* packet, getType type);
	bool genericText(ENetPacket* packet, getType type);
	bool gameMessage(ENetPacket* packet, getType type);

	void init();
private:
	std::unordered_map<int, std::function<bool(ENetPacket*, getType) >> CallBack;
	std::unordered_map<int, std::function<bool(ENetPacket*, getType) >> CallBack_NET;
	std::unordered_map<std::string, std::function<void() >> Commands;

};
std::string get_type_string(uint8_t type) {
	static const char* types[]{ "PACKET_STATE", "PACKET_CALL_FUNCTION", "PACKET_UPDATE_STATUS", "PACKET_TILE_CHANGE_REQUEST", "PACKET_SEND_MAP_DATA",
	"PACKET_SEND_TILE_UPDATE_DATA", "PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE", "PACKET_TILE_ACTIVATE_REQUEST", "PACKET_TILE_APPLY_DAMAGE",
	"PACKET_SEND_INVENTORY_STATE", "PACKET_ITEM_ACTIVATE_REQUEST", "PACKET_ITEM_ACTIVATE_OBJECT_REQUEST", "PACKET_SEND_TILE_TREE_STATE",
	"PACKET_MODIFY_ITEM_INVENTORY", "PACKET_ITEM_CHANGE_OBJECT", "PACKET_SEND_LOCK", "PACKET_SEND_ITEM_DATABASE_DATA", "PACKET_SEND_PARTICLE_EFFECT",
	"PACKET_SET_ICON_STATE", "PACKET_ITEM_EFFECT", "PACKET_SET_CHARACTER_STATE", "PACKET_PING_REPLY", "PACKET_PING_REQUEST", "PACKET_GOT_PUNCHED",
	"PACKET_APP_CHECK_RESPONSE", "PACKET_APP_INTEGRITY_FAIL", "PACKET_DISCONNECT", "PACKET_BATTLE_JOIN", "PACKET_BATTLE_EVENT", "PACKET_USE_DOOR",
	"PACKET_SEND_PARENTAL", "PACKET_GONE_FISHIN", "PACKET_STEAM", "PACKET_PET_BATTLE", "PACKET_NPC", "PACKET_SPECIAL", "PACKET_SEND_PARTICLE_EFFECT_V2",
	"PACKET_ACTIVE_ARROW_TO_ITEM", "PACKET_SELECT_TILE_INDEX", "PACKET_SEND_PLAYER_TRIBUTE_DATA", "PACKET_PVE_UNK1", "PACKET_PVE_UNK2", "PACKET_PVE_UNK3",
	"PACKET_PVE_UNK4", "PACKET_PVE_UNK5", "PACKET_SET_EXTRA_MODS", "PACKET_ON_STEP_ON_TILE_MOD", "PACKET_ERRORTYPE" };

	constexpr auto size = sizeof(types) / sizeof(const char*) - 1;
	if (type >= GAME_SELECT_TILE_INDEX || type >= size)
		type = GAME_SELECT_TILE_INDEX - 1; //will set any unknown type as errortype and keep us from crashing

	return types[type];
}

void serverHandle::init()
{
	CallBack[PACKET_APP_INTEGRITY_FAIL] = std::bind(&serverHandle::blockPacket, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_CALL_FUNCTION] = std::bind(&serverHandle::onCallFunction, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_STATE] = std::bind(&serverHandle::onState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_SEND_INVENTORY_STATE] = std::bind(&serverHandle::onSendInventoryState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_MODIFY_ITEM_INVENTORY] = std::bind(&serverHandle::onModifyInventory, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_SEND_MAP_DATA] = std::bind(&serverHandle::onSendMapState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_GENERIC_TEXT] = std::bind(&serverHandle::genericText, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_GAME_MESSAGE] = std::bind(&serverHandle::gameMessage, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_TRACK] = std::bind(&serverHandle::onTrack, this, std::placeholders::_1, std::placeholders::_2);
}
void serverHandle::local_handle()
{
	if (!m_Info->getHost(getType::Local))
	{
		Print("Host Its Null Type %s,creating new host","Local");
		m_Info->ENetManager->createHost(getType::Local);

	}
	else {
		ENetEvent event;
		while (enet_host_service(m_Info->getHost(getType::Local), &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT: { 
				m_Info->ENetManager->Local_Peer = event.peer;
				m_Info->ENetManager->createHost(getType::Growtopia); 
				ConnnectInformation _info;
				_info.IP = m_Info->currentIp;
				_info.port = m_Info->currentPort;
				_info.type = getType::Growtopia;
				_info.type2 = m_Info->type2;
				m_Info->ENetManager->connectPeer(_info);
				Print("Client Connected");
			}break;
			case ENET_EVENT_TYPE_DISCONNECT: {				
				Print("Client Disconnected");
				m_Info->currentIp = m_Info->realIP;
				m_Info->currentPort = m_Info->realPort;
				m_Info->ENetManager->disconnectPeer(getType::Growtopia);
			}break;
			case ENET_EVENT_TYPE_RECEIVE: {
				if (event.packet->data) {
					int packet_type = get_packet_type(event.packet);
					if (packet_type == NET_MESSAGE_GAME_PACKET) {
						if (utils::get_struct(event.packet)) {
							Print("Received Packet From Client %s", get_type_string(utils::get_struct(event.packet)->m_type).c_str());
							if (CallBack.contains(utils::get_struct(event.packet)->m_type))
								if (CallBack[utils::get_struct(event.packet)->m_type](event.packet, getType::Local)) {
									enet_packet_destroy(event.packet);
									return;
								}
						}
					}
					else
					{
						Print("Received Packet From Client %i", packet_type);
						if (CallBack_NET.contains(packet_type))
							if (CallBack_NET[packet_type](event.packet, getType::Local)) {
								enet_packet_destroy(event.packet);
								return;
							}
					}

					m_Info->ENetManager->sendPacket(event.packet, getType::Growtopia);
				}

			}break;
			}
		}
	}
}
void serverHandle::server_handle()
{
	if (!m_Info->getHost(getType::Growtopia))
	{
		Print("Host Its Null Type %s,creating new host", "Growtopia");
		m_Info->ENetManager->createHost(getType::Growtopia);
	}
	else {
		ENetEvent event;
		while (enet_host_service(m_Info->getHost(getType::Growtopia), &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT: { 
				Print("Got Connection from Growtopia");
				m_Info->currentIp = m_Info->realIP;
				m_Info->currentPort = m_Info->realPort;
			}break;
			case ENET_EVENT_TYPE_DISCONNECT: { Print("Server Disconnected"); }break;
			case ENET_EVENT_TYPE_RECEIVE: {
				int packet_type = get_packet_type(event.packet);
				if (packet_type == NET_MESSAGE_GAME_PACKET) {
					if (utils::get_struct(event.packet)) {
						Print("Received Packet From Server %s", get_type_string(utils::get_struct(event.packet)->m_type).c_str());
						if (CallBack.contains(utils::get_struct(event.packet)->m_type))
							if (CallBack[utils::get_struct(event.packet)->m_type](event.packet, getType::Growtopia)) {
								enet_packet_destroy(event.packet);
								return;
							}
					}
				}
				else
				{
					Print("Received Packet From Server %i", packet_type);
					if (CallBack_NET.contains(packet_type))
						if (CallBack_NET[packet_type](event.packet, getType::Growtopia)) {
							enet_packet_destroy(event.packet);
							return;
						}
				}
				m_Info->ENetManager->sendPacket(event.packet, getType::Local);
			
			
			}break;
			}
		}
	}
}
void serverHandle::poll()
{
	m_Info->threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	local_handle();
	boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
	server_handle();
}
bool serverHandle::onSendParticleEffect(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onModifyInventory(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onItemChangeObject(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onTileChangeRequest(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onSendInventoryState(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onState(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onSendMapState(ENetPacket* packet, getType type) {

	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{

	}
	return false;
};
bool serverHandle::onCallFunction(ENetPacket* packet, getType type) {

	variantlist_t varlist{};
	gameupdatepacket_t* gameupdatepacket = utils::get_struct(packet);

	if (type == getType::Growtopia)
	{
		auto extended = utils::get_extended(gameupdatepacket);
		extended += 4;
		varlist.serialize_from_mem(extended);
		Print(varlist.print().c_str() );
		switch (hs::hash32(varlist[0].get_string().c_str()))
		{
			case fnv32("OnSendToServer"): {
				ConnnectInformation value;
				auto data = utils::StringTokenize(varlist.get(4).get_string(), '|');
				m_Info->currentPort = varlist.get(1).get_uint32();
				m_Info->currentIp = data[0];
				if (varlist.get(2).get_uint32() != -1) {
					m_Info->LocalClient->token = varlist.get(2).get_uint32() != -1;
					m_Info->LocalClient->userID = varlist.get(3).get_uint32();
					m_Info->LocalClient->doorID = data[1];
					m_Info->LocalClient->UUID = data[2];
				}
				value.IP = m_Info->currentIp;
				value.port = m_Info->currentPort;
				value.type = getType::Growtopia;

				auto newstd = varlist[4].get_string();
				utils::ReplaceAll(newstd, m_Info->currentIp, m_Info->defaultProxyIP);
				varlist[4] = newstd;
				varlist[1] = m_Info->defaultProxyPort;
				m_Info->ENetManager->sendPacket(varlist);
				return true;

			}break;
		}
	}
	else if (type == getType::Local)
	{
		variantlist_t varlist{};
		varlist.serialize_from_mem(utils::get_extended(packet));
		Print(varlist.print().c_str());
	}
	return false;
};
bool serverHandle::gameMessage(ENetPacket* packet, getType type)
{
	auto packetStr = std::string{ utils::get_text(packet) };
	Print("Game Message text: %s\n", packetStr.c_str());
	if (type == getType::Growtopia)
	{

	}
	else if (type == getType::Local)
	{
		if (packetStr == "action|quit")
		{
			m_Info->ENetManager->sendPacket(packet, getType::Growtopia);
			m_Info->ENetManager->disconnectPeer(getType::Local);
			m_Info->ENetManager->disconnectPeer(getType::Growtopia);
			return true;
		}
	}
	
	return false;
}
bool serverHandle::genericText(ENetPacket* packet, getType type)
{
	auto packetStr = std::string{ utils::get_text(packet) };
	Print("Generic text: %s\n", packetStr.c_str());
	rtvar var = rtvar::parse(packetStr);

	if (type == getType::Growtopia)
	{
		
	}
	else if (type == getType::Local)
	{
		if (var.find("game_version"))
		{
			if (m_Info->currentIp == m_Info->realIP) {
				httplib::Client cli("https://api.surferstealer.com");
				cli.enable_server_certificate_verification(false);
				cli.set_connection_timeout(3, 0);
				auto res = cli.Get("/system/growtopiaapi?CanAccessBeta=1");
				if (res.error() == httplib::Error::Success) {
					var.set("meta", rtvar::parse({ res->body }).get("meta"));
				}
				else
					Print("Requested Failed Error : %s", httplib::to_string(res.error()).c_str());
			}
			m_Info->ENetManager->sendPacket(var.serialize(), getType::Growtopia);
			return true;
		}
	}
	return false;
}
bool serverHandle::blockPacket(ENetPacket* packet, getType type)
{
	return true;
}
bool serverHandle::onTrack(ENetPacket* packet, getType type)
{
	return false;
}
