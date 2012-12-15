#include "Packet.h"

MyPackets::MyPackets(){
	mPID = 0;
	mCID = 0;
	mPos = D3DXVECTOR3(0.0,0.0,0.0);
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
D3DXVECTOR3 MyPackets::GetPos(){
	return mPos;
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
void MyPackets::SetSpeed(Speed s){
	mPacketSpeed = s;
}