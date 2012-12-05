#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "GfxObj.h"

class Triangle: public GfxObj{
public:
	Triangle();
	~Triangle();
	void draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech);
	void draw();
	void init(ID3D10Device* device);
	void DrawVertices();
	float GetPosX();
	float GetPosY();
	float GetPosZ();

private:


};


#endif