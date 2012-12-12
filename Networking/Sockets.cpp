#include "Sockets.h"
#include <sstream>

SOCKETS::SOCKETS(){
	Initialised = false;
	m_SocketAddressSize = sizeof(sockaddr_in);
	std::wostringstream outs;
	outs <<L"Press Y to become Server" <<endl;
	outs <<L"Press N to become Client" <<endl;
	NetText = outs.str();
	NoCon = 0;
	LocalID=0;
	UDP = true;
	initRead = false;
}
SOCKETS::~SOCKETS(){
	Clean();
}
void SOCKETS::Clean(){
	WSACleanup();
	closesocket(s);
}
void SOCKETS::Error(HWND hwnd){
	printf("Socket error\n");
	MessageBox (hwnd,L"Socket Error",L"Information",MB_OK);
}
void SOCKETS::init(int type){
	if(type==0){
		Server = true;
	}else if(type==1){
		Server = false;
	}
	Initialised = true;
	Connected = false;
	sa_size = sizeof(sockaddr);
	Connections = 0;

	if(!StartWinSock()){
		printf("error starting winSock\n");
	}else{
		printf("WinSock Started \n");
	}
	Create();
	if(Server){
		if(!Bind()){
			printf("Error:  Unable to bind socket!\n");
		}else{
			printf("Socket Bound\n");
		}
		if(!UDP){
			if(!Listening()){
				printf("Error:  Unable to listen!\n");
			}else{
				printf("Listening for connections \n");
			}
		}
	}
}
int SOCKETS::StartWinSock(void){
	// Startup Winsock
	WSADATA w;
	int error = WSAStartup (0x0202,&w);
	if (error){
		printf("Error:  You need WinSock 2.2!\n");
		return 0;
	}
	if (w.wVersion!=0x0202){
		printf("Error:  Wrong WinSock version!\n");
		WSACleanup ();
		return 0;
	}
	return 1;
}
void SOCKETS::Create(){
	// Create the listening UDP socket
	s = socket (AF_INET,SOCK_DGRAM,0);
	std::wostringstream outs;
	me.sin_family = AF_INET;
	if(Server){
		me.sin_port = htons (5555);
		me.sin_addr.s_addr = htonl (INADDR_ANY);
		outs << L"127.0.0.1" <<endl;
	}else if(!Server){
		me.sin_port = htons (SERVERPORT);
		me.sin_addr.s_addr = inet_addr (SERVERIP);
		printf("The IP address being connected to is: ");
		printf("%d.", (int)me.sin_addr.S_un.S_un_b.s_b1);
		printf("%d.", (int)me.sin_addr.S_un.S_un_b.s_b2);
		printf("%d.", (int)me.sin_addr.S_un.S_un_b.s_b3);
		printf("%d\n", (int)me.sin_addr.S_un.S_un_b.s_b4);

		outs << (int)me.sin_addr.S_un.S_un_b.s_b1 << L".";
		outs << (int)me.sin_addr.S_un.S_un_b.s_b2 << L".";
		outs << (int)me.sin_addr.S_un.S_un_b.s_b3 << L".";
		outs << (int)me.sin_addr.S_un.S_un_b.s_b4 << endl;
	}
	
	// notice the different byte order for network
	u_short PortNo;
	PortNo = (me.sin_port >> 8)|(me.sin_port << 8);
	outs << (int)PortNo << endl;
	if(Server){
		printf("My PORT address is: ");
		printf("%d\n", (int)PortNo);
	}else if(!Server){
		printf("The PORT being connected to is: ");
		printf("%d\n\n", (int)PortNo);
		SetDestinationAddress("127.0.0.1", PortNo);
	}
	IPText = outs.str();
	
}
bool SOCKETS::Bind(){
	// Bind the listening socket
	if (bind(s,(LPSOCKADDR)&me,sizeof(me))==SOCKET_ERROR){
		WSACleanup ();
		return false;
	}
	return true;
}
bool SOCKETS::Listening(){
	// Set s[0] to listen for connections
	if (listen(s,1)==SOCKET_ERROR){
		WSACleanup ();
		return false;
	}
	return true;
}
void SOCKETS::NumCon(){
	printf("Listening for  a connections...\n");
	printf("Number of current connection = %d\n\n", Connections);
}
void SOCKETS::SetAsync(HWND hwnd){
	// Make socket asynchronous and hook to WM_SOCKET message
	if(Server){
		WSAAsyncSelect (s,hwnd,WM_SOCKET,(FD_CLOSE | FD_ACCEPT | FD_READ));
	}else if(!Server){
		WSAAsyncSelect (s,hwnd,WM_SOCKET,(FD_CLOSE | FD_CONNECT | FD_READ));
	}
	
}
void SOCKETS::SConnected(){
	//if a valid connection is made to the server
	//the  MSG_CONNECTED message will be recieved.
	printf("Connected to server - got FD_CONNECT\n");
	Connected = true;
}
void SOCKETS::Accept(WPARAM wParam){
	AcceptMsg = accept (wParam,&you,&sa_size);
	printf("Client has connected!\n");
	sprintf_s (MyPacket.Text, TEXTSIZE, "%c",MSG_CONNECTED); //A
	printf ("The Following has been sent -> %c\n", MyPacket.Text[0]);
	memcpy(Buffer, &MyPacket, sizeof(MyPackets));
	SendMsg = send (wParam, Buffer, BUFFERSIZE, 0);
}
void SOCKETS::Connect(){
	if(!Server){
		if(!Connected){
			int temp =connect(s,(LPSOCKADDR)&me,sa_size);
			if(temp<0){
				printf("connect error\n");
			}else{
				Connected = true;
				InitSend();
			}
		}else{
			printf("Already connected to server\n");
		}
	}
}
void SOCKETS::Close(int type){
	if(type==0){
		printf("The Server has Quit!");
	}else if(type==1){
		printf("The Client has Quit!");
	}
	PostQuitMessage (0);
}
//send functions
void SOCKETS::Send(){
	CommonSend();
	send (s, Buffer, BUFFERSIZE ,0);
}
void SOCKETS::Send(WPARAM wParam){
	CommonSend();
	send (wParam, Buffer, BUFFERSIZE ,0);
}
void SOCKETS::SendTo(){
	CommonSend();
	int n = sendto(s, Buffer, BUFFERSIZE, 0, (struct sockaddr *)&m_RemoteAddress, m_SocketAddressSize);
}
void SOCKETS::CommonSend(){
	if(!Server){
		MyPacket.CID = LocalID;
	}
	memcpy(Buffer, &MyPacket, sizeof(MyPackets));
}
void SOCKETS::InitSend(){
	if(!Server){
		if(Connected){
			sprintf(MyPacket.Text,"Initial Connect");
			SendTo();
		}else{
			printf("Not connected to server yet!\n");
		}
	}
}
//read functions
void SOCKETS::Read(){
	RecvMsg = recv (s,Buffer, BUFFERSIZE, 0);
	CommonRead();
}
void SOCKETS::Read(WPARAM wParam){
	RecvMsg = recv (wParam,Buffer, BUFFERSIZE, 0);
	s= wParam;
	CommonRead();
}
void SOCKETS::ReadFrom(WPARAM wParam){
	int n = recvfrom(wParam, Buffer, BUFFERSIZE, 0, (struct sockaddr *)&m_RemoteAddress, &m_SocketAddressSize);
	CommonRead();
}
void SOCKETS::InitRead(){
	if(Server){
		if(!CheckList()){
			MyPacket.CID = NoCon;
			SendTo();
		}
	}else if(!Server && !initRead){
		LocalID = MyPacket.CID;
		initRead = true;
	}
}
void SOCKETS::CommonRead(){
	memcpy(&MyPacket, Buffer, sizeof(MyPackets));
	InitRead();
	printf("Received Packet with ID -> %d\n", MyPacket.PID);
	printf("From -> %d\n", MyPacket.CID);
}
void SOCKETS::ChangeText(std::wstring net){
	std::wostringstream outs;
	if(Initialised){
		outs <<IPText;
	}
	outs << net;
	NetText = outs.str();
}
void SOCKETS::RedrawText(){
	std::wostringstream outs;
	if(Initialised){
		outs <<IPText;
		outs <<"ID:" << LocalID;
	}
	NetText = outs.str();
}
std::wstring SOCKETS::GetText(){
	return NetText;
}
SOCKET SOCKETS::GetSocket(){
	return s;
}
bool SOCKETS::CheckType(){
	return Server;
}
void SOCKETS::SetDestinationAddress(char * IP, const int Port)
{
	m_RemoteAddress.sin_family = AF_INET;
	m_RemoteAddress.sin_port = htons (Port);
	m_RemoteAddress.sin_addr.s_addr = inet_addr (IP);
}
bool SOCKETS::GetInit(){
	return Initialised;
}
bool SOCKETS::CheckList(){
	for(it=SIDS.begin(); it!=SIDS.end(); it++){
		if(!strcmp(inet_ntoa(it->sin_addr.sin_addr),inet_ntoa(m_RemoteAddress.sin_addr))){
			if((ntohs(it->sin_addr.sin_port)==ntohs(m_RemoteAddress.sin_port))){			
				return true;
			}
		}
	}
	SocketID temp;
	temp.sin_addr= m_RemoteAddress;
	temp.ID = ++NoCon;
	SIDS.push_back(temp);
	return false;
}
/*
bool checklist(sockaddr_in temp1, char msg[PACKETSIZE]){
	id temp;

	for(std::list<id>::iterator it=ident.begin(); it!=ident.end(); it++){
		if(!strcmp(inet_ntoa(it->sin_addr.sin_addr),inet_ntoa(temp1.sin_addr))){
			if((ntohs(it->sin_addr.sin_port)==ntohs(temp1.sin_port))){
				printf("Received from %s: %s \n", it->name, msg);   
				return true;
			}
		}
	}
	temp.sin_addr = temp1;
	strcpy(temp.name,msg);
	ident.push_back(temp);
	printf("%s has joined.\n", temp.name);   
	return false;
}
*/
/*
struct id{
	struct sockaddr_in sin_addr;
	char name[PACKETSIZE];
};

*/