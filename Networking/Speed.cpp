#include "Speed.h"
#include "d3dUtil.h"

Speed::Speed(){
	Dir = D3DXVECTOR3(0.0f,0.0f,0.0f);
	accel = 0.0f;
	velo = 0.0005f;
	DirLimit = 10;
	RandDirCount = 0;
}
Speed::~Speed(){

}
void Speed::CheckLimits(){
	if(Dir.x>DirLimit){
		Dir.x=DirLimit;
	}else if(Dir.x < -DirLimit){
		Dir.x = -DirLimit;
	}
	if(Dir.y>DirLimit){
		Dir.y=DirLimit;
	}else if(Dir.y < -DirLimit){
		Dir.y = -DirLimit;
	}
	if(Dir.z>DirLimit){
		Dir.z=DirLimit;
	}else if(Dir.z < -DirLimit){
		Dir.z = -DirLimit;
	}
}
void Speed::UpdateDir(D3DXVECTOR3 temp){
	Dir = temp;
	CheckLimits();
}
void Speed::UpdateDir(float dx, float dy, float dz){
	Dir.x+=dx;
	Dir.y+=dy;
	Dir.z+=dz;
	CheckLimits();
}
void Speed::RandDir(){
	if(RandDirCount++ == 1000){
		int temp = int(DirLimit);
		Dir.x = (rand()%(2*temp))-temp;
		Dir.y = (rand()%(2*temp))-temp;
		Dir.z = (rand()%(2*temp))-temp;
		RandDirCount =0;
	}
}