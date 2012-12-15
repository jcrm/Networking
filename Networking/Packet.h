#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "defines.h"
#include "Speed.h"
#include "d3dUtil.h"

class MyPackets{
public:
	char mText[80];
	
	MyPackets();
	~MyPackets();

	int GetCID();
	int GetPID();
	bool GetReadyToRecv();
	D3DXVECTOR3 GetPos();
	Speed GetSpeed();

	void SetCID(int c);
	void SetPID(int p);
	void SetReadyToRecv(bool r);
	void SetPos(D3DXVECTOR3 p);
	void SetSpeed(Speed p);

private:
	int mCID;
	int mPID;
	bool mReadyToRecv;
	D3DXVECTOR3 mPos;
	Speed mPacketSpeed;
};


#endif