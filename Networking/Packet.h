#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "defines.h"
#include "Speed.h"
#include "d3dUtil.h"

class MyPackets
{
public:
	int PID;
	int CID;
	bool ReadyToRecv;
	D3DXVECTOR3 pos;
	Speed PacketSpeed;
	char Text[80];
	
	MyPackets();
	~MyPackets();
	void UpdatePos();
};


#endif