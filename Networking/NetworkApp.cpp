//==================================================================================//
//	Network App based on Triangle App.cpp based on Frank Lunas Coloured Cube App	//
//																					//
//==================================================================================//

#include "d3dApp.h"
#include "Vertex.h"
#include "NetworkApp.h"
#include "PlayerList.h"

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
}

NetworkApp::~NetworkApp(){
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void NetworkApp::initApp(){
	D3DApp::initApp();
	PList.Init(md3dDevice);
	thisTriangle.init(md3dDevice);
	thatTriangle.init(md3dDevice);
	thisCube.init(md3dDevice);
	thisSphere.init(md3dDevice,(3.142/5),10,10);
	thisSphere.Translate(5.0f,0.0f,0.0f);
	mClearColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	buildFX();
	buildVertexLayouts();
}

void NetworkApp::onResize(){
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	//create projection matrix from field of view
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}
LRESULT NetworkApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_SOCKET:
		if (WSAGETSELECTERROR(lParam)){
			appSockets.Error(mhMainWnd);
			PostQuitMessage (0);
			return 0;
		}
		switch (WSAGETSELECTEVENT(lParam)){
			case FD_ACCEPT:{
					appSockets.Accept(wParam);
					break;
				}//end case FD_ACCePT
			case FD_CONNECT:{
					appSockets.SConnected();
					break;
				}//end case FD_CONNECT
			case FD_READ:{
					appSockets.ReadFrom(wParam);
					LocalRead();
					break;
				}//end case FD_READ
			case FD_CLOSE:{
					appSockets.Close(0);
					break;
				}//end case FD_CLOSE
		}
	}
	return D3DApp::msgProc(msg, wParam, lParam);
}
void NetworkApp::updateScene(float dt){

	if(GetAsyncKeyState(VK_ESCAPE)& 0x8000){
		//system("pause");
		PostQuitMessage(0);
	}else if(GetAsyncKeyState(VK_F1)& 0x8000){
		appSockets.SendTo();
		appSockets.RedrawText();
		cout << "F1 keypress\n";
	}
	if(GetAsyncKeyState('C')& 0x8000){
		appSockets.Connect();
		appSockets.RedrawText();
	}
	if(GetAsyncKeyState('P')& 0x8000){
		ShowWin32Console();
	}
	if(GetAsyncKeyState('T')& 0x8000){
		cout << "T keypress\n";
		appSockets.SendAll();
		appSockets.RedrawText();
	}
	if(GetAsyncKeyState('Y')& 0x8000){
		if(!appSockets.GetInit()){
			cout << "create server";
			appSockets.init(0);
			appSockets.SetAsync(mhMainWnd);
			appSockets.RedrawText();
			PList.AddToList(appSockets.MyPacket);
		}
	}
	if(GetAsyncKeyState('N')& 0x8000){
		if(!appSockets.GetInit()){
			cout << "create client";
			appSockets.init(1);
			appSockets.SetAsync(mhMainWnd);
			appSockets.RedrawText();
		}
	}
	Speed tSpeed;			
	tSpeed= thisCube.GetSpeed();
	if(GetAsyncKeyState(VK_DOWN)& 0x8000 ){
		tSpeed.UpdateDir(0,-0.1,0);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000){
		tSpeed.UpdateDir(0,0.1,0);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000){
		tSpeed.UpdateDir(-0.1,0,0);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000){
		tSpeed.UpdateDir(0.1,0,0);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('Q') & 0x8000){
		tSpeed.UpdateDir(0,0,0.1);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('E') & 0x8000){
		tSpeed.UpdateDir(0,0,-0.1);
		thisCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('M') & 0x8000){
		D3DXVECTOR3 temp(0,0,0);
		tSpeed.UpdateDir(temp);
		thisCube.ChangeSpeed(tSpeed);
	}	
	// Update angles based on input to orbit camera around scene.
	if(GetAsyncKeyState('A') & 0x8000)	thisCamera.MoveLeft();
	if(GetAsyncKeyState('D') & 0x8000)	thisCamera.MoveRight();
	if(GetAsyncKeyState('W') & 0x8000)	thisCamera.MoveForward();
	if(GetAsyncKeyState('S') & 0x8000)	thisCamera.MoveBack();
	if(GetAsyncKeyState('Z') & 0x8000)	thisCamera.MoveUp();
	if(GetAsyncKeyState('X') & 0x8000)	thisCamera.MoveDown();

	thisSphere.Move();
	if(PList.Move(thisCube)){
		appSockets.UpdatePacket(thisCube.GetPos(), thisCube.GetSpeed());
	}
	

	D3DApp::updateScene(dt);

	thisCamera.Update();
	thisCamera.Look(mView);
}

void NetworkApp::drawScene(){
	D3DApp::drawScene();
	//! Restore default states, input layout and primitive topology 
	//! because mFont->DrawText changes them.  Note that we can 
	//! restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   

	// Don't transform texture coordinates, so just use identity transformation.
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	//xthisTriangle.draw(mView,mProj,mfxWVPVar,mTech);
	//xthatTriangle.draw(mView,mProj,mfxWVPVar,mTech);
	//xthisCube.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
	thisSphere.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);


	PList.Draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);


	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	md3dDevice->RSSetState(0);
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	RECT NR = {5, mClientHeight-75,0,0};
	mFont->DrawText(0, appSockets.GetText().c_str(), -1, &NR, DT_NOCLIP, BLACK);
	
	mSwapChain->Present(0, 0);
}

void NetworkApp::buildFX(){
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

void NetworkApp::buildVertexLayouts(){
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] ={
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}
void NetworkApp::LocalRead(){
	if(appSockets.CheckType()){
		if(appSockets.GetNewConnection()){
			PList.AddToList(appSockets.MyPacket);
		}else if(appSockets.MyPacket.ReadyToRecv){
			appSockets.SendAllCubes(PList.GetList(),PList.GetIterator());
		}else{
			PList.UpdateList(appSockets.MyPacket);
			appSockets.SendAll(appSockets.MyPacket.CID);
		}
	}else{
		if(appSockets.GetLocalID()==appSockets.MyPacket.CID && !appSockets.initRead){
			appSockets.initRead = true;
			PList.SetLocalID(appSockets.GetLocalID());
			PList.AddToList(appSockets.MyPacket);
		}else if(PList.CheckList(appSockets.MyPacket.CID)){
			PList.UpdateList(appSockets.MyPacket);
		}else if(!PList.CheckList(appSockets.MyPacket.CID)){
			PList.AddToList(appSockets.MyPacket);
		}
	}
}