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
	bool onDisconnect(ENetPacket* packet, getType type);

	void init();
private:
	struct callbackStruct
	{
		std::chrono::time_point <std::chrono::system_clock>  Timeout;
		std::function<bool(std::string&)> target_Function;
		std::string target_Case;
		std::string target_String;
		bool infinity = false;
		bool enabled = true;
	};
	std::unordered_map<int, std::function<bool(ENetPacket*, getType) >> CallBack;
	std::unordered_map<int, std::function<bool(ENetPacket*, getType) >> CallBack_NET;
	std::unordered_map<uint32_t, callbackStruct> callBack_CALL_FUNCTION;
	std::unordered_map<std::string, std::function<void(std::string) >> Commands;

};
void serverHandle::init()
{
	
	CallBack[PACKET_DISCONNECT] = std::bind(&serverHandle::onDisconnect, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_APP_INTEGRITY_FAIL] = std::bind(&serverHandle::blockPacket, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_CALL_FUNCTION] = std::bind(&serverHandle::onCallFunction, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_STATE] = std::bind(&serverHandle::onState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_SEND_INVENTORY_STATE] = std::bind(&serverHandle::onSendInventoryState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_MODIFY_ITEM_INVENTORY] = std::bind(&serverHandle::onModifyInventory, this, std::placeholders::_1, std::placeholders::_2);
	CallBack[PACKET_SEND_MAP_DATA] = std::bind(&serverHandle::onSendMapState, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_GENERIC_TEXT] = std::bind(&serverHandle::genericText, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_GAME_MESSAGE] = std::bind(&serverHandle::gameMessage, this, std::placeholders::_1, std::placeholders::_2);
	CallBack_NET[NET_MESSAGE_TRACK] = std::bind(&serverHandle::onTrack, this, std::placeholders::_1, std::placeholders::_2);
	Commands["test"] = [=](std::string Text){
		m_Info->send_log("Detected 'Test' Command");
	};
	Commands["drop"]/*drop itemid*/ = [=](std::string itemid) {
		callbackStruct _callbackStruct;
		_callbackStruct.target_Case = ("OnDialogRequest");
		_callbackStruct.target_String = ("add_textbox|How many to drop?|left|");
		_callbackStruct.Timeout = std::chrono::system_clock::now()+std::chrono::milliseconds(5000);
		_callbackStruct.target_Function = [=](std::string Dialog) {
			auto var = rtvar::parse(Dialog);
			if (var.find("embed_data"))
			{
				auto itemID = var.find("embed_data")->m_values[1];/*add_text_input|count||target|5|*/
				auto count = var.find("add_text_input")->m_values[2];/*add_text_input|count||target|5|*/
				auto dropPacket = (std::string)"action|dialog_return\ndialog_name|";
				dropPacket.append(var.get("end_dialog")+"\n");
				dropPacket.append(var.get("embed_data")+"|"+ itemID +"\n");
				dropPacket.append(var.get("add_text_input") + "|" + count);
				m_Info->ENetManager->sendPacket(dropPacket, getType::Growtopia);
			}
			return true;//block dialog request
		};
		m_Info->ENetManager->sendPacket("action|drop\n|itemID|" + itemid, getType::Growtopia);
		this->callBack_CALL_FUNCTION[fnv32("normal_drop")] = _callbackStruct;
	};
	Commands["fd"]/*fast drop*/ = [=](std::string Text) {
		if (!callBack_CALL_FUNCTION.contains(fnv32("fast_drop"))) {
			callbackStruct _callbackStruct;
			_callbackStruct.target_Case = ("OnDialogRequest");
			_callbackStruct.target_String = ("add_textbox|How many to drop?|left|");
			_callbackStruct.target_Function = [=](std::string &Dialog) {
				auto var = rtvar::parse(Dialog);
				if (var.find("embed_data"))
				{
					auto itemID = var.find("embed_data")->m_values[1];/*add_text_input|count||target|5|*/
					auto count = var.find("add_text_input")->m_values[2];/*add_text_input|count||target|5|*/
					auto dropPacket = (std::string)"action|dialog_return\ndialog_name|";
					dropPacket.append(var.get("end_dialog") + "\n");
					dropPacket.append(var.get("embed_data") + "|" + itemID + "\n");
					dropPacket.append(var.get("add_text_input") + "|" + count);
					m_Info->ENetManager->sendPacket(dropPacket, getType::Growtopia);
				}
				return true;//block dialog 
			};
			_callbackStruct.infinity = true;
			this->callBack_CALL_FUNCTION[fnv32("fast_drop")] = _callbackStruct;
		}
		else
		{
			callBack_CALL_FUNCTION[fnv32("fast_drop")].enabled = !callBack_CALL_FUNCTION[fnv32("fast_drop")].enabled;
		}
		if (callBack_CALL_FUNCTION[fnv32("fast_drop")].enabled)
			m_Info->send_log("Fast Drop Enabled");
		else
			m_Info->send_log("Fast Drop Disabled");
	};
	Commands["ft"]/*fast trash*/ = [=](std::string Text) {
		if (!callBack_CALL_FUNCTION.contains(fnv32("fast_trash"))) {
			callbackStruct _callbackStruct;
			_callbackStruct.target_Case = ("OnDialogRequest");
			_callbackStruct.target_String = ("end_dialog|trash_item|Cancel|OK|");
			_callbackStruct.target_Function = [=](std::string Dialog) {
				auto var = rtvar::parse(Dialog);
				if (var.find("embed_data"))
				{
					auto itemID = var.find("embed_data")->m_values[1];/*embed_data|itemID|target*/
					std::string count = Dialog.substr(Dialog.find("you have ") + 9, Dialog.length() - Dialog.find("you have ") - 1);
					auto dropPacket = (std::string)"action|dialog_return\ndialog_name|";
					dropPacket.append(var.get("end_dialog") + "\n");
					dropPacket.append(var.get("embed_data") + "|" + itemID + "\n");
					dropPacket.append(var.get("add_text_input") + "|" + count);
					m_Info->ENetManager->sendPacket(dropPacket, getType::Growtopia);
				}
				return true;//block dialog 
			};
			_callbackStruct.infinity = true;
			this->callBack_CALL_FUNCTION[fnv32("fast_trash")] = _callbackStruct;
		}
		else
		{
			callBack_CALL_FUNCTION[fnv32("fast_trash")].enabled = !callBack_CALL_FUNCTION[fnv32("fast_trash")].enabled;
		}
		if (callBack_CALL_FUNCTION[fnv32("fast_trash")].enabled)
			m_Info->send_log("Fast Trash Enabled");
		else
			m_Info->send_log("Fast Trash Disabled");
	};
	Commands["warp"] = [=](std::string Text) {
		m_Info->ENetManager->sendPacket("action|join_request\nname|" + Text+"\ninvitedWorld|0", getType::Growtopia, NET_MESSAGE_GAME_MESSAGE);
		m_Info->send_log("Warping to " + Text);
	};

	callbackStruct _callbackStruct;
	_callbackStruct.target_Case = ("OnDialogRequest");
	_callbackStruct.target_String = ("The Growtopia Gazette");
	_callbackStruct.target_Function = [=](std::string &Dialog)
	{
		utils::ReplaceAll(Dialog, "The Growtopia Gazette", "the Proxy Gazette");
		return false;
	};
	_callbackStruct.infinity = true;
	callBack_CALL_FUNCTION[fnv32("replace_Gazette")] = _callbackStruct;
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
				enet_peer_timeout(m_Info->ENetManager->Local_Peer, ENET_PEER_TIMEOUT_LIMIT* 100, ENET_PEER_TIMEOUT_MINIMUM * 100, ENET_PEER_TIMEOUT_MAXIMUM * 100);
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
						auto game_struct = utils::get_struct(event.packet);
						if (game_struct) {
							Print("Received Packet From Client %s", get_type_string(game_struct->m_type).c_str());
							if (
								CallBack.contains(game_struct->m_type)
								&& 
								CallBack[game_struct->m_type](event.packet, getType::Local)
								)
								{
									enet_packet_destroy(event.packet);
									return;
								}
						}
					}
					else
					{
						Print("Received Packet From Client %i", packet_type);
						if (
							CallBack_NET.contains(packet_type)
							&&
							CallBack_NET[packet_type](event.packet, getType::Local)
							)
						{
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
					auto game_struct = utils::get_struct(event.packet);
					if (game_struct) {
						Print("Received Packet From Server %s", get_type_string(game_struct->m_type).c_str());
						if (
							CallBack.contains(game_struct->m_type)
							&&
							CallBack[game_struct->m_type](event.packet, getType::Growtopia)
							)
							{
								enet_packet_destroy(event.packet);
								return;
							}
					}
				}

				else
				{
					Print("Received Packet From Server %i", packet_type);
					if (
						CallBack_NET.contains(packet_type)
						&&
						CallBack_NET[packet_type](event.packet, getType::Growtopia)
						)
						{
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
	std::vector<int> deleteList;
	local_handle();
	server_handle();
	for (auto x : callBack_CALL_FUNCTION)
		if (std::chrono::system_clock::now() >= x.second.Timeout && !x.second.infinity)
			deleteList.push_back(x.first);
	for (auto x : deleteList)
		callBack_CALL_FUNCTION.erase(x);
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
				m_Info->LocalClient->userID = varlist.get(3).get_uint32();
				if (varlist.get(2).get_uint32() != -1) {
					m_Info->LocalClient->token = varlist.get(2).get_uint32();
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
				clearTempInfo();
				return true;
			}break;
			case fnv32("OnSpawn"): {
				rtvar var = rtvar::parse(varlist[1].get_string());
				if (var.find("name") && var.find("netID"))
				{
					player _player;
					_player.name = var.get("name");
					_player.country = var.get("country");
					_player.netid = var.get_int("netID");
					_player.userid = var.get_int("userID");
					_player.invis = var.get_int("invis");
					_player.mod = (_player.invis ? true : (_player.userid == 0 ? true : false));
					if (var.find("posXY"))
						_player.pos = vector2_t(atoi( (var.find("posXY")->m_values.size() > 0 ? var.find("posXY")->m_values[0].c_str() : "") ), atoi((var.find("posXY")->m_values.size() > 1 ? var.find("posXY")->m_values[1].c_str() : "")));
					if (var.get("type") == "local")
						m_Info->LocalClient->_Player = _player;
					m_Info->World->Players[var.get_int("netID")] = _player;
				}
			} break;
			case fnv32("OnRemove"): {
				rtvar var = rtvar::parse(varlist[1].get_string());
				if (var.find("netID"))
					if (m_Info->World->Players.contains(var.get_int("netID")))
						m_Info->World->Players.erase(var.get_int("netID"));
			} break;
			case fnv32("OnRequestWorldSelectMenu"):
				clearTempInfo();
				break;
			case fnv32("onShowCaptcha"):{
				auto g = utils::explode("|", varlist[1].get_string());
				std::string captchaid = g[1];
				utils::replace(captchaid, "0098/captcha/generated/", "");
				utils::replace(captchaid, "PuzzleWithMissingPiece.rttex", "");
				captchaid = captchaid.substr(0, captchaid.size() - 1);
				auto x = PuzzleSolver(m_Info->captchaSolverKey);
				x.GetAnswer(captchaid);
				if (x.Solved) {
					m_Info->send_log("Solved Captcha As " + x.LatestAnswer);
					m_Info->ENetManager->sendPacket("action|dialog_return\ndialog_name|puzzle_captcha_submit\ncaptcha_answer|" + x.LatestAnswer + "|CaptchaID|" + g[4],getType::Growtopia);
					return true;
				}
				else
				{
					m_Info->send_log("Captcha Solve Failed");
					Print("Captcha Response : %s", x.Error.c_str());
				}
			}break;
		}
	}
	else if (type == getType::Local)
	{
		varlist.serialize_from_mem(utils::get_extended(packet));
		Print(varlist.print().c_str());
	}
	for (auto &x : callBack_CALL_FUNCTION)
	{
		if (x.second.target_Case == varlist[0].get_string()  && x.second.enabled)
		{
			for (int i = 1; i < C_MAX_VARIANT_LIST_PARMS; i++) {
				if (varlist[i].get_type() == variant_t::vartype_t::TYPE_STRING)
				{
					if (varlist[i].get_string().find(x.second.target_String) != std::string::npos)
					{
						std::string DialogContent = varlist[i].get_string();
						x.second.Timeout = std::chrono::system_clock::now() ;
						auto status= x.second.target_Function(DialogContent);
						if (!status)
						{
							varlist[i] = DialogContent;
							m_Info->ENetManager->sendPacket(varlist);
						}
						return true;
					}
				}

			}
		}
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
			m_Info->ENetManager->disconnectPeer(getType::Local);
			return false;
		}
	}
	
	return false;
}
bool serverHandle::genericText(ENetPacket* packet, getType type)
{
	auto packetStr = std::string{ utils::get_text(packet) };
	Print("Generic text: %s", packetStr.c_str());
	rtvar var = rtvar::parse(packetStr);

	if (type == getType::Growtopia)
	{
		
	}
	else if (type == getType::Local)
	{
		if (var.find("game_version"))
		{
			if (var.find("tankIDName"))
				m_Info->LocalClient->name = var.get("tankIDName");
			if (m_Info->currentIp == m_Info->realIP) {
				httplib::Client cli("https://api.surferwallet.net");
				cli.enable_server_certificate_verification(false);
				cli.set_connection_timeout(3, 0);
				auto res = cli.Get("/Growtopia");
				if (res.error() == httplib::Error::Success) {
					var.set("meta", rtvar::parse({ res->body }).get("meta"));
				}
				else
					Print("Requested Failed Error : %s", httplib::to_string(res.error()).c_str());
			}
			m_Info->ENetManager->sendPacket(var.serialize(), getType::Growtopia);
			return true;
		}
		else if (var.get("action") == "input")
		{
			auto chatText =  (var.get(1).m_values.size()>0) ? var.get(1).m_values[1] : "Null";
			chatText.erase(0, 1);
			auto vec = utils::explode(" ", chatText);
			if ( Commands.contains(vec[0]) )
			{
				Commands[vec[0]]( (vec.size()>1 ? vec[1] : ""));
				return true;
			}
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
bool serverHandle::onDisconnect(ENetPacket* packet, getType type)
{
	m_Info->ENetManager->disconnectPeer(getType::Growtopia);
	m_Info->ENetManager->disconnectPeer(getType::Local);

	return true;

}
