#ifndef GFXOBJ_H
#define GFXOBJ_H

/*** COMPILER OPTIONS ***/

//#ifdef _MSVC
#pragma warning(disable:4244)	// double -> float conversion
#define _CRT_SECURE_NO_WARNINGS
//#endif //_MSVC
#include "d3dUtil.h"
#include "GameTimer.h"
#include "Vertex.h"
#include <vector>
#include <string>

#pragma warning(disable:4305) // double -> float


class GfxObj
{
public:
	GfxObj(): mNumVertices(0),mNumIndices(0), mNumFaces(0), md3dDevice(0), mVB(0), mIB(0), scale(1,1,1),pos(0,0,0),theta(0,0,0), indexed(false)
	{

	}
	~GfxObj()
	{
		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
	}
	void setTrans(void)
	{
		D3DXMATRIX m;
		D3DXMatrixIdentity(&world);
		D3DXMatrixScaling(&m, scale.x, scale.y, scale.z);
		world*=m;
		D3DXMatrixRotationY(&m, theta.y);
		world*=m;
		D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);
		world*=m;
	}
	void Translate(float x, float y, float z){
		pos.x = x;
		pos.y = y;
		pos.z = z;
		setTrans();
	}
	D3DXMATRIX GetWorld(){
		return world;
	}
	// Virtual methods.  Derived client class overrides these methods to 
	// implement specific object requirements.

	virtual void init (ID3D10Device* device, DWORD nV=0, DWORD nF=0)
	{
		md3dDevice=		device;
		mNumVertices=	nV;
		mNumFaces=		nF;

	}
	//virtual void update(float dt);
	//virtual void draw()=0; 

	virtual void draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
		setTrans();
		D3DXMATRIX mWVP = world*mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&world);
		D3D10_TECHNIQUE_DESC techDesc;
		mTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex( p )->Apply(0);        
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
			if(indexed){
				md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
				md3dDevice->DrawIndexed(mNumIndices, 0,0);
			} else{
				md3dDevice->Draw(mNumVertices, 0);
			}
		}
	}

public: //should this be protected?
	D3DXMATRIX world;
	D3DXVECTOR3 pos, theta, scale;	

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	DWORD mNumIndices;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	bool indexed;

	typedef std::vector<Vertex> VertexList;
	typedef std::vector<DWORD> IndexList;

	void buildVertices(VertexList& vertices, IndexList& indices);
};

#endif // GFXOBJ_H