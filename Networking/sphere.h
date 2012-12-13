#ifndef SPHERE_H
#define SPHERE_H

#include "GfxObj.h"

class Sphere: public GfxObj
{
public:
	Sphere();
	~Sphere();

	void init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks);
	void draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech);
	void DrawVertices();
	void Move();
private:
	typedef std::vector<Vertex> VertexList;
	typedef std::vector<DWORD> IndexList;

	void buildStacks(VertexList& vertices, IndexList& indices);

private:
	float mRadius;
	UINT  mNumSlices;
	UINT  mNumStacks;
};

#endif // SPHERE_H
