#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "defines.h"
#include "d3dUtil.h"

class MyPackets
{
public:
	int PID;
	int CID;
	D3DXVECTOR3 pos;
	char Text[80];
	
	MyPackets();
	~MyPackets();
};


#endif