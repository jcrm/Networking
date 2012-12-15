#include "Speed.h"
#include "d3dUtil.h"

Speed::Speed(){
	mDir = D3DXVECTOR3(0.0f,0.0f,0.0f);
	mAccel = 0.0f;
	mVelo = 0.0005f;
	mDirLimit = 10;
	mRandDirCount = 0;
}
Speed::~Speed(){

}
void Speed::CheckLimits(){
	if(mDir.x>mDirLimit){
		mDir.x=mDirLimit;
	}else if(mDir.x < -mDirLimit){
		mDir.x = -mDirLimit;
	}
	if(mDir.y>mDirLimit){
		mDir.y=mDirLimit;
	}else if(mDir.y < -mDirLimit){
		mDir.y = -mDirLimit;
	}
	if(mDir.z>mDirLimit){
		mDir.z=mDirLimit;
	}else if(mDir.z < -mDirLimit){
		mDir.z = -mDirLimit;
	}
}
void Speed::UpdateDir(D3DXVECTOR3 temp){
	mDir = temp;
	CheckLimits();
}
void Speed::UpdateDir(float dx, float dy, float dz){
	mDir.x+=dx;
	mDir.y+=dy;
	mDir.z+=dz;
	CheckLimits();
}
void Speed::RandDir(){
	if(mRandDirCount++ == 1000){
		int temp = int(mDirLimit);
		mDir.x = (rand()%(2*temp))-temp;
		mDir.y = (rand()%(2*temp))-temp;
		mDir.z = (rand()%(2*temp))-temp;
		mRandDirCount =0;
	}
}