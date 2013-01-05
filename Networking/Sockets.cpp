#include "Sockets.h"
#include <sstream>

SOCKETS::SOCKETS(){
	mInitialised = false;
	mSocketAddressSize = sizeof(sockaddr_in);
	mLocalPacket.SetCID(0);
	mUDP = true;
	mInitRead = false;
	mNewConnection = true;
	mConnected = false;

	std::wostringstream outs;
	outs <<L"Press Y to become Server" <<endl;
	outs <<L"Press N to become Client" <<endl;
	NetText = outs.str();
}
SOCKETS::~SOCKETS(){
	Clean();
}
void SOCKETS::Init(int type){
	if(type==0){
		mServer = true;
	}else if(type==1){
		mServer = false;
	}
	mInitialised = true;
	if(!StartWinSock()){
		printf("error starting winSock\n");
	}else{
		printf("WinSock Started \n");
	}
	Create();
	if(mServer){
		if(!Bind()){
			printf("Error:  Unable to bind socket!\n");
		}else{
			printf("Socket Bound\n");
		}
		if(!mUDP){
			if(!Listening()){
				printf("Error:  Unable to listen!\n");
			}else{
				printf("Listening for connections \n");
			}
		}
	}
}
bool SOCKETS::StartWinSock(void){
	// Start-up Winsock
	WSADATA w;
	int error = WSAStartup (0x0202,&w);
	if (error){
		printf("Error:  You need WinSock 2.2!\n");
		return false;
	}
	if (w.wVersion!=0x0202){
		printf("Error:  Wrong WinSock version!\n");
		WSACleanup ();
		return false;
	}
	return true;
}
void SOCKETS::Create(){
	// Create the listening UDP socket
	mSocket = socket (AF_INET,SOCK_DGRAM,0);
	std::wostringstream outs;
	mMe.sin_family = AF_INET;
	if(mServer){
		mMe.sin_port = htons (5555);
		mMe.sin_addr.s_addr = htonl (INADDR_ANY);
		outs << L"127.0.0.1" <<endl;
	}else if(!mServer){
		mMe.sin_port = htons (SERVERPORT);
		mMe.sin_addr.s_addr = inet_addr (SERVERIP);
		printf("The IP address being connected to is: ");
		printf("%d.", (int)mMe.sin_addr.S_un.S_un_b.s_b1);
		printf("%d.", (int)mMe.sin_addr.S_un.S_un_b.s_b2);
		printf("%d.", (int)mMe.sin_addr.S_un.S_un_b.s_b3);
		printf("%d\n", (int)mMe.sin_addr.S_un.S_un_b.s_b4);

		outs << (int)mMe.sin_addr.S_un.S_un_b.s_b1 << L".";
		outs << (int)mMe.sin_addr.S_un.S_un_b.s_b2 << L".";
		outs << (int)mMe.sin_addr.S_un.S_un_b.s_b3 << L".";
		outs << (int)mMe.sin_addr.S_un.S_un_b.s_b4 << endl;
	}
	// notice the different byte order for network
	u_short PortNo;
	PortNo = (mMe.sin_port >> 8)|(mMe.sin_port << 8);
	outs << (int)PortNo << endl;
	if(mServer){
		printf("My PORT address is: ");
		printf("%d\n", (int)PortNo);
	}else if(!mServer){
		printf("The PORT being connected to is: ");
		printf("%d\n\n", (int)PortNo);
		SetDestinationAddress("127.0.0.1", PortNo);
	}
	IPText = outs.str();
	
}
bool SOCKETS::Bind(){
	// Bind the listening socket
	if (bind(mSocket,(LPSOCKADDR)&mMe,sizeof(mMe))==SOCKET_ERROR){
		WSACleanup ();
		return false;
	}
	return true;
}
bool SOCKETS::Listening(){
	// Set s[0] to listen for connections
	if (listen(mSocket,1)==SOCKET_ERROR){
		WSACleanup ();
		return false;
	}
	return true;
}
void SOCKETS::Error(HWND hwnd){
	printf("Socket error\n");
	MessageBox (hwnd,L"Socket Error",L"Information",MB_OK);
}
void SOCKETS::Clean(){
	WSACleanup();
	closesocket(mSocket);
}
void SOCKETS::Close(){
	if(mServer){
		printf("The Server has Quit!");
	}else{
		printf("The Client has Quit!");
	}
	PostQuitMessage (0);
}
void SOCKETS::SetAsync(HWND hwnd){
	// Make socket asynchronous and hook to WM_SOCKET message
	if(mServer){
		WSAAsyncSelect (mSocket,hwnd,WM_SOCKET,(FD_CLOSE | FD_ACCEPT | FD_READ));
	}else if(!mServer){
		WSAAsyncSelect (mSocket,hwnd,WM_SOCKET,(FD_CLOSE | FD_CONNECT | FD_READ));
	}
}
void SOCKETS::Accept(WPARAM wParam){
	int AcceptMsg = accept (wParam,&mYou,&mSocketAddressSize);
	printf("Client has connected!\n");
	sprintf_s (mTempPacket.mText, TEXTSIZE, "%c",MSG_CONNECTED); //A
	printf ("The Following has been sent -> %c\n", mTempPacket.mText[0]);
	memcpy(mBuffer, &mTempPacket, sizeof(MyPackets));
	int SendMsg = send (wParam, mBuffer, BUFFERSIZE, 0);
}
void SOCKETS::Connect(){
	if(!mServer){
		if(!mConnected){
			int temp =connect(mSocket,(LPSOCKADDR)&mMe,mSocketAddressSize);
			if(temp<0){
				printf("connect error\n");
			}else{
				mConnected = true;
				InitSend();
			}
		}else{
			printf("Already connected to server\n");
		}
	}
}
void SOCKETS::SConnected(){
	//if a valid connection is made to the server
	//the  MSG_CONNECTED message will be received.
	printf("Connected to server - got FD_CONNECT\n");
	mConnected = true;
}
//send functions
void SOCKETS::InitSend(){
	if(!mServer){
		if(mConnected){
			sprintf_s(mTempPacket.mText,"Initial Connect");
			SendTo();
		}else{
			printf("Not connected to server yet!\n");
		}
	}
}
void SOCKETS::Send(){
	CommonSend();
	send (mSocket, mBuffer, BUFFERSIZE ,0);
}
void SOCKETS::Send(WPARAM wParam){
	CommonSend();
	send (wParam, mBuffer, BUFFERSIZE ,0);
}
void SOCKETS::SendTo(){
	CommonSend();
	int SendMsg = sendto(mSocket, mBuffer, BUFFERSIZE, 0, (struct sockaddr *)&mRemoteAddress, mSocketAddressSize);
}
void SOCKETS::SendServer(){
	mTempPacket.SetCID(mLocalPacket.GetCID());
	mTempPacket.SetPID(mLocalPacket.GetPID()+1);
	mTempPacket.SetPos(mLocalPacket.GetPos());
	mTempPacket.SetSpeed(mLocalPacket.GetSpeed());
	printf("Sent Packet with ID -> %d\n", mTempPacket.GetPID());
	memcpy(mBuffer, &mTempPacket, sizeof(MyPackets));
	int SendMsg = sendto(mSocket, mBuffer, BUFFERSIZE, 0, (struct sockaddr *)&mRemoteAddress, mSocketAddressSize);
}
void SOCKETS::SendAll(){
	int temp=0;
	for(it=SIDS.begin(); it!=SIDS.end(); it++){
		sprintf_s(mTempPacket.mText,"TEST%d",temp);
		mRemoteAddress = it->sin_addr;
		SendTo();
		temp++;
	}
}
void SOCKETS::SendAll(int ID){
	int temp=0;
	for(it=SIDS.begin(); it!=SIDS.end(); it++){
		if(it->ID != ID){
			sprintf_s(mTempPacket.mText,"TEST%d",temp);
			mRemoteAddress = it->sin_addr;
			SendTo();
		}
		temp++;
	}
}
void SOCKETS::SendAllCubes(std::list<Players> LocalList, std::list<Players>::iterator PlayerListIT){
	for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
		mTempPacket.SetCID(PlayerListIT->ID);
		mTempPacket.SetSpeed(PlayerListIT->PlayerCube.GetSpeed());
		mTempPacket.SetPos(PlayerListIT->PlayerCube.GetPos());
		mTempPacket.SetReadyToRecv(false);
		SendTo();
	}
}
void SOCKETS::SendBall(Speed s, D3DXVECTOR3 p){
	mTempPacket.SetCID(mLocalPacket.GetCID());
	mTempPacket.SetPID(mLocalPacket.GetPID()+1);
	mTempPacket.SetPos(p);
	mTempPacket.SetSpeed(s);
	mTempPacket.SetBall(true);
	printf("Sent Packet with ID -> %d\n", mTempPacket.GetPID());
	memcpy(mBuffer, &mTempPacket, sizeof(MyPackets));
	SendAll();
	mTempPacket.SetBall(false);
}
void SOCKETS::CommonSend(){
	if(!mServer){
		mTempPacket.SetCID(mLocalPacket.GetCID());
		mTempPacket.SetPID(mLocalPacket.GetPID()+1);
		mTempPacket.SetPos(mLocalPacket.GetPos());
		mTempPacket.SetSpeed(mLocalPacket.GetSpeed());
		printf("Sent Packet with ID -> %d\n", mTempPacket.GetPID());
	}
	memcpy(mBuffer, &mTempPacket, sizeof(MyPackets));
}
//read functions
void SOCKETS::InitRead(){
	if(mServer){
		if(!CheckList()){
			mTempPacket.SetCID(SIDS.size());
			SendTo();
			SendAll(mTempPacket.GetCID());
		}
	}else if(!mServer && !mInitRead){
		mLocalPacket.SetCID(mTempPacket.GetCID());
		mTempPacket.SetReadyToRecv(true);
		SendTo();
		mTempPacket.SetReadyToRecv(false);
	}
}
void SOCKETS::Read(){
	int RecvMsg = recv (mSocket,mBuffer, BUFFERSIZE, 0);
	CommonRead();
}
void SOCKETS::Read(WPARAM wParam){
	int RecvMsg = recv (wParam,mBuffer, BUFFERSIZE, 0);
	mSocket= wParam;
	CommonRead();
}
void SOCKETS::ReadFrom(WPARAM wParam){
	int RecvMsg = recvfrom(wParam, mBuffer, BUFFERSIZE, 0, (struct sockaddr *)&mRemoteAddress, &mSocketAddressSize);
	CommonRead();
}
void SOCKETS::CommonRead(){
	memcpy(&mTempPacket, mBuffer, sizeof(MyPackets));
	InitRead();
	printf("Received Packet with ID -> %d\n", mTempPacket.GetPID());
	printf("Msg %s\n", mTempPacket.mText);
	printf("From -> %d\n", mTempPacket.GetCID());
}
//text
void SOCKETS::ChangeText(std::wstring net){
	std::wostringstream outs;
	if(mInitialised){
		outs <<IPText;
	}
	outs << net;
	NetText = outs.str();
}
void SOCKETS::RedrawText(){
	std::wostringstream outs;
	if(mInitialised){
		outs <<IPText;
		outs <<"ID:" << mLocalPacket.GetCID();
	}
	NetText = outs.str();
}
std::wstring SOCKETS::GetText(){
	return NetText;
}
//socket
SOCKET SOCKETS::GetSocket(){
	return mSocket;
}
void SOCKETS::SetDestinationAddress(char * IP, const int Port){
	mRemoteAddress.sin_family = AF_INET;
	mRemoteAddress.sin_port = htons (Port);
	mRemoteAddress.sin_addr.s_addr = inet_addr (IP);
}
//packets
MyPackets SOCKETS::GetTempPacket(){
	return mTempPacket;
}
MyPackets SOCKETS::GetLocalPacket(){
	return mLocalPacket;
}
void SOCKETS::UpdatePacket(D3DXVECTOR3 tempPos, Speed tempSpeed){
	mLocalPacket.SetPos(tempPos);
	mLocalPacket.SetSpeed(tempSpeed);
}
void SOCKETS::UpdatePacket(float x, float y, float z, Speed tempSpeed){
	D3DXVECTOR3 tempPos;
	tempPos = mLocalPacket.GetPos();
	tempPos.x+=x;
	tempPos.y+=y;
	tempPos.z+=z;
	mLocalPacket.SetPos(tempPos);
	mLocalPacket.SetSpeed(tempSpeed);
}
//list
bool SOCKETS::CheckList(){
	for(it=SIDS.begin(); it!=SIDS.end(); it++){
		if(!strcmp(inet_ntoa(it->sin_addr.sin_addr),inet_ntoa(mRemoteAddress.sin_addr))){
			if((ntohs(it->sin_addr.sin_port)==ntohs(mRemoteAddress.sin_port))){	
				mNewConnection = false;
				return true;
			}
		}
	}
	mNewConnection = true;
	SocketID temp;
	temp.sin_addr= mRemoteAddress;
	temp.ID = SIDS.size()+1;
	SIDS.push_back(temp);
	return false;
}
//Variable Checks
int SOCKETS::GetLocalID(){
	return mLocalPacket.GetCID();
}
bool SOCKETS::GetServer(){
	return mServer;
}
bool SOCKETS::GetInitialised(){
	return mInitialised;
}
bool SOCKETS::GetConnected(){
	return mConnected;
}
bool SOCKETS::GetNewConnection(){
	return mNewConnection;
}
bool SOCKETS::GetInitRead(){
	return mInitRead;
}
void SOCKETS::SetInitRead(bool b){
	mInitRead = b;
}
