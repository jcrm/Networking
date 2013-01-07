#include "Packet.h"

MyPackets::MyPackets(){
	mPID = 0;
	mBall = false;
	mCID = 0;
	mPos = D3DXVECTOR3(0.0,0.0,0.0);
	mColour = D3DXVECTOR3(RandF(),RandF(),RandF());
	mReadyToRecv = false;
}

MyPackets::~MyPackets(){
}
int MyPackets::GetCID(){
	return mCID;
}
int MyPackets::GetPID(){
	return mPID;
}
bool MyPackets::GetReadyToRecv(){
	return mReadyToRecv;
}
bool MyPackets::GetBall(){
	return mBall;
}
D3DXVECTOR3 MyPackets::GetPos(){
	return mPos;
}
D3DXVECTOR3 MyPackets::GetColour(){
	return mColour;
}
Speed MyPackets::GetSpeed(){
	return mPacketSpeed;
}

void MyPackets::SetCID(int c){
	mCID = c;
}
void MyPackets::SetPID(int p){
	mPID = p;
}
void MyPackets::SetReadyToRecv(bool r){
	mReadyToRecv = r;
}
void MyPackets::SetPos(D3DXVECTOR3 p){
	mPos = p;
}
void MyPackets::SetColour(D3DXVECTOR3 c){
	mColour = c;
}
void MyPackets::SetSpeed(Speed s){
	mPacketSpeed = s;
}
void MyPackets::SetBall(bool b){
	mBall = b;
}