#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "Speed.h"
#include "d3dUtil.h"

class MyPackets{
public:	
	MyPackets();
	~MyPackets();

	int GetCID();
	int GetPID();
	bool GetReadyToRecv();
	bool GetBall();
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetColour();
	Speed GetSpeed();

	void SetCID(int c);
	void SetPID(int p);
	void SetReadyToRecv(bool r);
	void SetPos(D3DXVECTOR3 p);
	void SetColour(D3DXVECTOR3 c);
	void SetSpeed(Speed p);
	void SetBall(bool b);

private:
	int mCID;
	int mPID;
	bool mBall;
	bool mReadyToRecv;
	D3DXVECTOR3 mPos;
	D3DXVECTOR3 mColour;
	Speed mPacketSpeed;
};


#endif