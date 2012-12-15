#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include "d3dUtil.h"
#include "Packet.h"
#include "Speed.h"
#include "PlayerList.h"
#include <list>

struct SocketID{
	struct sockaddr_in sin_addr;
	int ID;
};

class SOCKETS{
public:
	SOCKETS();
	~SOCKETS();
	void Init(int type);
	void Error(HWND hwnd);
	void Clean();
	void Close();
	void SetAsync(HWND hwnd);
	//FD functions
	void Accept(WPARAM wParam);
	void Connect();
	void SConnected();
	//send 
	void InitSend();
	void Send();
	void Send(WPARAM wParam);
	void SendTo();
	void SendServer();
	void SendAll();
	void SendAll(int ID);
	void SendAllCubes(std::list<Players> LocalList, std::list<Players>::iterator PlayerListIT);
	//read
	void InitRead();
	void Read();
	void Read(WPARAM wParam);
	void ReadFrom(WPARAM wParam);
	//text
	void ChangeText(std::wstring net);
	void RedrawText();
	std::wstring GetText();
	//socket
	SOCKET GetSocket();
	void SetDestinationAddress(char * IP, const int Port);
	//packet
	MyPackets GetTempPacket();
	MyPackets GetLocalPacket();
	void UpdatePacket(D3DXVECTOR3 temp, Speed tempSpeed);
	void UpdatePacket(float x, float y, float z, Speed tempSpeed);
	//List
	bool CheckList();
	//Variable Checks
	int GetLocalID();
	bool GetServer();
	bool GetInitialised();
	bool GetConnected();
	bool GetNewConnection();
	bool GetInitRead();
	void SetInitRead(bool b);
private:
	//socket
	SOCKET mSocket;
	sockaddr_in mMe;
	sockaddr_in mRemoteAddress;
	sockaddr mYou;
	int mSocketAddressSize;
	//packets
	MyPackets mTempPacket;
	MyPackets mLocalPacket;
	char mBuffer[BUFFERSIZE];
	//text
	std::wstring NetText;
	std::wstring IPText;
	//list
	std::list<SocketID> SIDS;
	std::list<SocketID>::iterator it;
	//variable checks
	bool mServer;
	bool mUDP;
	bool mInitialised;
	bool mNewConnection;
	bool mConnected;
	bool mInitRead;
private:
	//functions
	bool StartWinSock(void);
	void Create();
	bool Bind();
	bool Listening();
	void CommonRead();
	void CommonSend();
};

#endif