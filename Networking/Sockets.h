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
struct TextState{
	enum State{sDefault, sChoose, sAdd, sLatest};
	State mState;

	TextState(){
		mState = sDefault;
	}
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
	void Create();
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
	void SendBall(Speed s, D3DXVECTOR3 p);
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
	void UpdatePacket(D3DXVECTOR3 tempPos, Speed tempSpeed, D3DXVECTOR3 tempColour);
	void UpdatePacket(float x, float y, float z, Speed tempSpeed, D3DXVECTOR3 tempColour);
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
	void AddToIP(int temp);
	void ClearIP();
private:
	char mIP[20];
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

	TextState mState;
private:
	//functions
	bool StartWinSock(void);
	bool Bind();
	bool Listening();
	void CommonRead();
	void CommonSend();
};

#endif