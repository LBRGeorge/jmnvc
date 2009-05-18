#pragma once
#ifndef VCMPSRV_RCON_H
#define VCMPSRV_RCON_H

class CRcon
{
private:
	RakServerInterface* m_pRak;
public:
	CRcon(WORD iPort, char* szPass, WORD iMaxAdmins);
	~CRcon();

	RakServerInterface* GetRakServer() { return m_pRak; };

	void Process();

	void ConsoleOutput(char* szOutput);
	void ConsolePrintf( char* szFormat, ... );

	void Packet_NewIncomingConnection(Packet* pPacket);
	void Packet_DisconnectionNotification(Packet* pPacket);
	void Packet_ConnectionLost(Packet* pPacket);
	void Packet_ConnectionBanned(Packet* pPacket);
	void Packet_RconCommand(Packet* pPacket);

	void OutputChatMessage(char * szNick, char * szText);
	void OutputDeathMessage(int iDeathType,BYTE byteWhoDied,BYTE byteWhoKilled);


};

#endif