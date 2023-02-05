#ifndef EnetManager_H
#define EnetManager_H
class _enetManager
{
public:
	_enetManager()
	{
		enet_initialize();
	}
	ENetPeer* Local_Peer = nullptr;
	ENetHost* Local_Host = nullptr;
	ENetHost* Growtopia_Host = nullptr;
	ENetPeer* Growtopia_Peer = nullptr;
	void createHost(getType type)
	{
		auto& host = (type == getType::Growtopia) ? Growtopia_Host : Local_Host;
		if (host != nullptr)
			destoryHost(type);
		ENetAddress address;
		if (type == getType::Growtopia)
		{
			host = enet_host_create(0, 2, 2, 0, 0);
			BOOST_ASSERT_MSG(host != nullptr, "Host Its Null");
			host->usingNewPacket = true;
			host->checksum = enet_crc32;
			auto code = enet_host_compress_with_range_coder(host);
			BOOST_ASSERT_MSG(code == 0, "enet host compressing failed");
		}
		else if (type == getType::Local)
		{

			enet_address_set_host(&address, "0.0.0.0");
			address.port = 17192;
			host = enet_host_create(&address, ENET_PROTOCOL_MAXIMUM_PEER_ID, 10, 0, 0);
			BOOST_ASSERT_MSG(host != nullptr, "Host Its Null");
	
			host->usingNewPacket = false;
			host->checksum = enet_crc32;
			auto code = enet_host_compress_with_range_coder(host);
			BOOST_ASSERT_MSG(code == 0, "enet host compressing failed");
			
		}
	}
	void destoryHost(getType type)
	{
		auto& host = (type == getType::Growtopia) ? Growtopia_Host : Local_Host;
		if (host)
		{
			enet_host_destroy(host);
		}
		host = nullptr;
	}
	void connectPeer(ConnnectInformation information)
	{
		auto& host = (information.type == getType::Growtopia) ? Growtopia_Host : Local_Host;
		auto& peer = (information.type == getType::Growtopia) ? Growtopia_Peer : Local_Peer;
		ENetAddress address;
		BOOST_ASSERT_MSG(host != nullptr , "Host Its Null");

		if (information.type == getType::Growtopia)
		{
			enet_address_set_host_ip(&address, information.IP.c_str());
			address.port = information.port;
			host->checksum = enet_crc32;
			host->usingNewPacket = information.type2;
			peer = enet_host_connect(host, &address, 2, 0);
			if (!peer)
			{
				Print("No available peers for initiating an ENet connection");
			}
			else
			{
				Print("Connecting to %s:%i", information.IP.c_str(), information.port);

			}
		}
	}
	void destoryPeer(getType type)
	{
		auto& host = (type == getType::Growtopia) ? Growtopia_Host : Local_Host;
		auto& peer = (type == getType::Growtopia) ? Growtopia_Peer : Local_Peer;
		BOOST_ASSERT_MSG(host != nullptr, "Host Its Null failed to destory peer");
		ENetPeer* currentPeer;
		for (currentPeer = host->peers; currentPeer < &host->peers[host->peerCount]; ++currentPeer) {
			enet_peer_reset(currentPeer);
		}
	}
	void disconnectPeer(getType type)
	{
		auto& peer = (type == getType::Growtopia) ? Growtopia_Peer : Local_Peer;
		if(peer)
			enet_peer_disconnect_now(peer, 0);
	}
	bool sendPacket(std::string packet, getType client,int type=2)
	{
		ENetPacket* Enpacket = enet_packet_create(0, packet.length() + 5, 1);
		memcpy(Enpacket->data, &type, 4);
		memcpy((Enpacket->data) + 4, packet.c_str(), packet.length());
		return SendEnetPacket(Enpacket,client);
	}
	bool sendPacket(GameUpdatePacket data, getType client)
	{
		int type = NET_MESSAGE_GAME_PACKET;
		ENetPacket* packet = enet_packet_create(0, sizeof(GameUpdatePacket) + 5, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &type, 4);
		memcpy(packet->data + 4, (uint8_t*)&data, sizeof(GameUpdatePacket));
		return SendEnetPacket(packet, client);
	}
	bool sendPacket(gameupdatepacket_t data, getType client)
	{
		int type = NET_MESSAGE_GAME_PACKET;
		ENetPacket* packet = enet_packet_create(0, sizeof(gameupdatepacket_t) + 5, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &type, 4);
		memcpy(packet->data + 4, (uint8_t*)&data, sizeof(gameupdatepacket_t));
		return SendEnetPacket(packet, client);
	}
	bool sendPacket(ENetPacket*packet, getType client)
	{
		return SendEnetPacket(packet, client);
	}
	bool sendPacket(variantlist_t& list, int32_t netid = -1, int32_t delay = -1) {
		auto& host = this->Local_Host;
		auto& peer = this->Local_Peer;

		if (!peer || !host)
			return false;

		uint32_t data_size = 0;
		void* data = list.serialize_to_mem(&data_size, nullptr);
		#define MALLOC(type, ...) (type*)(malloc(sizeof(type) __VA_ARGS__))

		//optionally we wouldnt allocate this much but i dont want to bother looking into it
		auto update_packet = MALLOC(gameupdatepacket_t, +data_size);
		auto game_packet = MALLOC(gametextpacket_t, +sizeof(gameupdatepacket_t) + data_size);

		if (!game_packet || !update_packet)
			return false;

		memset(update_packet, 0, sizeof(gameupdatepacket_t) + data_size);
		memset(game_packet, 0, sizeof(gametextpacket_t) + sizeof(gameupdatepacket_t) + data_size);
		game_packet->m_type = NET_MESSAGE_GAME_PACKET;

		update_packet->m_type = PACKET_CALL_FUNCTION;
		update_packet->m_player_flags = netid;
		update_packet->m_packet_flags |= 8;
		update_packet->m_int_data = delay;
		memcpy(&update_packet->m_data, data, data_size);
		update_packet->m_data_size = data_size;
		memcpy(&game_packet->m_data, update_packet, sizeof(gameupdatepacket_t) + data_size);
		free(update_packet);

		auto packet = enet_packet_create(game_packet, data_size + sizeof(gameupdatepacket_t), ENET_PACKET_FLAG_RELIABLE);
		return SendEnetPacket(packet, getType::Local);
	}
	bool SendEnetPacket(ENetPacket* packet, getType client)
	{
		auto& host = (client == getType::Growtopia) ? this->Growtopia_Host : this->Local_Host;
		auto& peer = (client == getType::Growtopia) ? this->Growtopia_Peer : this->Local_Peer;
		if (!peer || !host)
		{
			Print("The packet could not be sent due to the peer or host its null. Type %s", (client == getType::Growtopia) ? "Growtopia" : "Local");
			enet_packet_destroy(packet);
			goto failed;
		}
		if (peer->state != ENET_PEER_STATE_CONNECTED)
		{
			Print("The packet could not be sent due to the peer state not connected.");
			enet_packet_destroy(packet);
			goto failed;
		}
		if (enet_list_size(&host->peers->sentReliableCommands) > 45)
		{
			Print("Packets have been cleared due to an excessive accumulation of packets.");
			enet_list_clear(&host->peers->sentReliableCommands);
			enet_packet_destroy(packet);
			goto failed;
		}
		if (enet_peer_send(peer, 0, packet) != 0)
		{
			Print("The packet could not be sent due to the enet_peer_send function return false");
			enet_packet_destroy(packet);
			goto failed;
		}
		else
			enet_host_flush(host);
	failed:
		return false;
	}
};

#endif