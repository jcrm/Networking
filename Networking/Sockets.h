#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include "d3dUtil.h"
#include "Packet.h"
#include "Speed.h"
#include <list>

struct SocketID{
	struct sockaddr_in sin_addr;
	int ID;
};

class SOCKETS{
public:

	SOCKETS();
	~SOCKETS();
	void init(int type);
	void Create();
	void Close(int type);
	void Accept(WPARAM wParam);

	void InitSend();
	void Send();
	void Send(WPARAM wParam);

	void Read();
	void Read(WPARAM wParam);
	void InitRead();

	void SetAsync(HWND hwnd);
	void NumCon();
	void Clean();
	void Error(HWND hwnd);
	void Connect();
	void SConnected();
	void SendTo();
	bool CheckType();
	void ReadFrom(WPARAM wParam);
	bool GetInit();
	void ChangeText(std::wstring net);
	void RedrawText();
	std::wstring GetText();
	SOCKET GetSocket();
	void SetDestinationAddress(char * IP, const int Port);
	void SendAll();
	void UpdatePacket(D3DXVECTOR3 temp, Speed tempSpeed);
	void UpdatePacket(float x, float y, float z, Speed tempSpeed);
	int GetLocalID();
	bool CheckList();
	bool GetNewConnection();

	MyPackets MyPacket;
	bool Connected;
	bool initRead;
private:
	bool UDP;
	bool NewConnection;
	SOCKET s;
	sockaddr_in me;
	sockaddr you;
	int sa_size;
	int Connections;
	int AcceptMsg;
	int SendMsg;
	int RecvMsg;
	int NoCon;
	MyPackets LocalPacket;
	struct sockaddr_in m_RemoteAddress;
	std::wstring NetText;
	std::wstring IPText;
	int m_SocketAddressSize;
	std::list<SocketID> SIDS;
	std::list<SocketID>::iterator it;

	char Buffer[BUFFERSIZE];
	
	bool Server;
	bool Initialised;
	int StartWinSock(void);
	bool Bind();
	bool Listening();
	void CommonRead();
	void CommonSend();
	
	
};

#endif