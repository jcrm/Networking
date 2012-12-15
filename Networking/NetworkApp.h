#ifndef NETWORKAPP_H
#define NETWORKAPP_H

#include "d3dApp.h"
#include "Vertex.h"

class NetworkApp : public D3DApp{
public:
	NetworkApp(HINSTANCE hInstance);
	~NetworkApp();
	LRESULT NetworkApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
private:
	void buildFX();
	void buildVertexLayouts();
 
	void LocalRead();
	void CameraKeyPresses();
	void ObjectKeyPresses();
	void NetworkKeyPresses();
private:	

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
};

#endif