#ifndef _SPEED_H_
#define _SPEED_H_

#include "defines.h"
#include "d3dUtil.h"

class Speed{
public:
	D3DXVECTOR3 Dir;
	float velo;
	float accel;
	Speed();
	~Speed();
	void UpdateDir(D3DXVECTOR3 temp);
	void UpdateDir(float dx,float dy,float dz);

private:
	void CheckLimits();
	float DirLimit;
};


#endif