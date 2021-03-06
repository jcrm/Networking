//=============================================================================
// Triangle App.cpp based on Frank Lunas Colored Cube App
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

#include "d3dApp.h"
#include "Vertex.h"

bool move = true;

class NetworkApp : public D3DApp
{
public:
	NetworkApp(HINSTANCE hInstance);
	~NetworkApp();
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	int scale;

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();
 
private:	

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;

	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	NetworkApp theApp(hInstance);


	theApp.initApp();

	return theApp.run();
}

NetworkApp::NetworkApp(HINSTANCE hInstance)
: D3DApp(hInstance),mFX(0), mTech(0), mfxWVPVar(0), mVertexLayout(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
}

NetworkApp::~NetworkApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void NetworkApp::initApp()
{
	D3DApp::initApp();
	thisTriangle.init(md3dDevice);
	thatTriangle.init(md3dDevice);
	thisCube.init(md3dDevice);
	thisSphere.init(md3dDevice,(3.142/5),10,10);
	thisSphere.Translate(5.0f,0.0f,0.0f);
	mClearColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	buildFX();
	buildVertexLayouts();
}

void NetworkApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	//create projection matrix from field of view
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void NetworkApp::updateScene(float dt)
{
	D3DApp::updateScene(dt);

	thisCamera.Update();
	thisCamera.Look(mView);
}

void NetworkApp::drawScene()
{
	D3DApp::drawScene();
	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   

	// set constants
	

	// Don't transform texture coordinates, so just use identity transformation.
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	//thisTriangle.draw(mView,mProj,mfxWVPVar,mTech);
	//thatTriangle.draw(mView,mProj,mfxWVPVar,mTech);
	thisCube.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
	thisSphere.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);



	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	md3dDevice->RSSetState(0);
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	RECT NR = {5, mClientHeight-75,0,0};
	mFont->DrawText(0, appSockets.GetText().c_str(), -1, &NR, DT_NOCLIP, BLACK);
	
	mSwapChain->Present(0, 0);
}

void NetworkApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	

	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWVPVar			= mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar			= mFX->GetVariableByName("gWorld")->AsMatrix();

}

void NetworkApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}
 