#include "ChatNodeClient.h"
#include "JsonConfig.h"
#include "packet/PacketMgr.h"
#include "MsgHandle/ChatPlayer.h"
#include "SeFNetClient.h"
#include "SeNet.h"
#include "LogUtil.h"

void ChatNodeClient::InitHelper()
{
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_CHAT, this, &ChatNodeClient::OnGateRouteChat);
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GAME, GAME_ROUTE_TO_CHAT, this, &ChatNodeClient::OnGameRouteChat);
	SetReportInfo();
	AddConnectServer();
}

void ChatNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_CHAT);
}

void ChatNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(ServerType::SERVER_TYPE_GATE);
	mConnectType.push_back(ServerType::SERVER_TYPE_GAME);
}

void ChatNodeClient::OnGateRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	GateToChatPacket gate_packet;
	if (!ReceivePB(msg_id, msg, msg_len, &gate_packet))
		return;

	ConnectDataPtr pServerData = GetServerNetInfo(sock_fd);
	if (!pServerData)
		return;

	// parse the packet
	Packet* pRecvPacket = g_packetmgr->CreatePakcet(gate_packet.msg_id(), gate_packet.msg_body().c_str(), gate_packet.msg_body().length());

	MsgHandle pHandle = g_packetmgr->GetMsgHandle(gate_packet.msg_id());
	if (pHandle == nullptr)
		return;

	ChatPlayer chatPlayer;
	chatPlayer.m_playerid = gate_packet.player_id();
	chatPlayer.m_servid = pServerData->serv_id;
	if (pHandle(&chatPlayer, pRecvPacket) != 0)
	{
		CLOG_INFO << "msg handle error" << CLOG_END;
	}
}

void ChatNodeClient::OnGameRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}

void ChatNodeClient::SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	ChatToGatePacket chat_gate;
	std::string send_msg = pb_msg->SerializeAsString();
	chat_gate.set_msg_id(msg_id);
	chat_gate.set_msg_body(send_msg);
	chat_gate.set_player_id(playerId);
	mNetCliModule->SendPbByServId(serverid, CHAT_ROUTE_TO_GATE, &chat_gate);
}