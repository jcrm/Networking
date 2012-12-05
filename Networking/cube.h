#ifndef _CUBE_H_
#define _CUBE_H_


#include "GfxObj.h"

class Cube: public GfxObj{
public:
	Cube();
	~Cube();
	void draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech);
	void draw();
	void buildVertices(VertexList& vertices, IndexList& indices);
	void init(ID3D10Device* device);
	void DrawVertices();
	float GetPosX();
	float GetPosY();
	float GetPosZ();

private:


};

#endif