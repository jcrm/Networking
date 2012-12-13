
#include "Packet.h"

MyPackets::MyPackets(){
	PID = 0;
	CID = 0;
	pos = D3DXVECTOR3(0.0,0.0,0.0);
	sprintf_s(Text,80,"");

}

MyPackets::~MyPackets(){

}