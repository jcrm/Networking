#ifndef GFXOBJ_H
#define GFXOBJ_H

/*** COMPILER OPTIONS ***/

//#ifdef _MSVC
#pragma warning(disable:4244)	// double -> float conversion
#define _CRT_SECURE_NO_WARNINGS
//#endif //_MSVC
#include "D3DUtil.h"
#include "GameTimer.h"
#include "Vertex.h"
#include "Speed.h"
#include <string>
#include <vector>

#pragma warning(disable:4305) // double -> float


class GfxObj{
public:
	GfxObj();
	~GfxObj();
	virtual void init(ID3D10Device* device);
	virtual void init(ID3D10Device* device, D3DXVECTOR3 c);
	virtual void draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech);
	virtual void Move();
	void setTrans();
	void Scale(float x, float y, float z);
	void TranslateTo(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Translate(D3DXVECTOR3 temp);
	D3DXVECTOR3 GetPos();
	D3DXMATRIX GetWorld();
	D3DXVECTOR3 GetColour();
	Speed GetSpeed();
	void ChangeSpeed(Speed tempSpeed);
	void CaclNewDir(D3DXVECTOR3 tempPackPos, D3DXVECTOR3 tempPackDir);
protected:
	Speed mySpeed;
	DWORD mNumVertices;
	DWORD mNumFaces;
	DWORD mNumIndices;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	bool indexed;
	
	typedef std::vector<Vertex> VertexList;
	typedef std::vector<DWORD> IndexList;

	D3DXMATRIX world;
	D3DXVECTOR3 pos, theta, scale;	
	virtual void buildVB(VertexList& vertices);
	virtual void buildIB(IndexList& indices);

	D3DXVECTOR3 mColour;
	

};

#endif // GFXOBJ_H