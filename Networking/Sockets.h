#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include "d3dUtil.h"
#include "Packet.h"

class SOCKETS{
public:
	SOCKETS();
	~SOCKETS();
	void init(int type);
	void Create();
	void Close(int type);
	void Accept(WPARAM wParam);
	void Read(WPARAM wParam);
	void SetAsync(HWND hwnd);
	void NumCon();
	void Clean();
	void Error(HWND hwnd);
	void Connect();
	void SConnected();
	void Send();
	bool CheckType();
	bool GetInit();
	void ChangeText(std::wstring net);
	void RedrawText();
	std::wstring GetText();
	SOCKET GetSocket();
	MyPackets MyPacket;
private:
	SOCKET s;
	sockaddr_in me;
	sockaddr you;
	int sa_size;
	int Connections;
	int AcceptMsg;
	int SendMsg;
	int RecvMsg;
	int NoCon;
	int LocalID;
	std::wstring NetText;
	std::wstring IPText;
	char Buffer[BUFFERSIZE];
	bool Connected;
	bool Server;
	bool Initialised;
	int StartWinSock(void);
	bool Bind();
	bool Listening();
};

#endif