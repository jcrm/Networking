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
	mMainWndCaption = L"1000424 Network Application";
	md3dDriverType  = D3D10_DRIVER_TYPE_HARDWARE;
	mClearColor     = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	mClientWidth    = 800;
	mClientHeight   = 600;
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
	//initialise list 
	mPList.Init(md3dDevice);
	//initialise temp cube
	mCube.init(md3dDevice);
	mSphere.init(md3dDevice,(3.142/5),10,10);
	mSphere.Translate(5.0f,0.0f,0.0f);
	
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
			mAppSockets.Error(mhMainWnd);
			PostQuitMessage (0);
			return 0;
		}
		switch (WSAGETSELECTEVENT(lParam)){
		case FD_ACCEPT:
			mAppSockets.Accept(wParam);
			break;	//end case FD_ACCEPT
		case FD_CONNECT:
			mAppSockets.SConnected();
			break;//end case FD_CONNECT
		case FD_READ:
			mAppSockets.ReadFrom(wParam);
			LocalRead();
			break;//end case FD_READ
		case FD_CLOSE:
			mAppSockets.Close();
			break;//end case FD_CLOSE
		}
	}
	return D3DApp::msgProc(msg, wParam, lParam);
}
void NetworkApp::updateScene(float dt){
	static int packetCount = 0;

	NetworkKeyPresses();
	ObjectKeyPresses();
	CameraKeyPresses();
	
	if((packetCount++)%500==0){
		if(mAppSockets.GetInitialised()){
			if(!mAppSockets.GetServer()&& mAppSockets.GetConnected()){
				mAppSockets.SendTo();
			}else if(mAppSockets.GetServer()){
				mAppSockets.SendServer();
				mAppSockets.SendBall(mSphere.GetSpeed(),mSphere.GetPos());
			}
		}
	}
	D3DApp::updateScene(dt);

	mCamera.Update();
	mCamera.Look(mView);
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

	mPList.Draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
	mSphere.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	md3dDevice->RSSetState(0);
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	RECT NR = {5, mClientHeight-75,0,0};
	mFont->DrawText(0, mAppSockets.GetText().c_str(), -1, &NR, DT_NOCLIP, BLACK);
	
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
	
	mTech		= mFX->GetTechniqueByName("ColorTech");
	mfxWVPVar	= mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar	= mFX->GetVariableByName("gWorld")->AsMatrix();
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
	if(mAppSockets.GetInitialised()){
		if(mAppSockets.GetServer()){
			if(mAppSockets.GetNewConnection()){
				mPList.AddToList(mAppSockets.GetTempPacket());
				
			}else if(mAppSockets.GetTempPacket().GetReadyToRecv()){
				mAppSockets.SendAllCubes(mPList.GetList(),mPList.GetIterator());
				mAppSockets.SendBall(mSphere.GetSpeed(),mSphere.GetPos());
			}else{
				mPList.UpdateList(mAppSockets.GetTempPacket());
				mAppSockets.SendAll(mAppSockets.GetTempPacket().GetCID());
			}
		}else{
			if(mAppSockets.GetLocalID()==mAppSockets.GetTempPacket().GetCID() && !mAppSockets.GetInitRead()){
				mAppSockets.SetInitRead(true);
				mPList.SetLocalID(mAppSockets.GetLocalID());
				mPList.AddToList(mAppSockets.GetTempPacket());
			}else if(mPList.CheckList(mAppSockets.GetTempPacket().GetCID())){
				if(mAppSockets.GetTempPacket().GetBall()){
					mSphere.ChangeSpeed(mAppSockets.GetTempPacket().GetSpeed());
					D3DXVECTOR3 temp = mAppSockets.GetTempPacket().GetPos();
					mSphere.TranslateTo(temp.x,temp.y,temp.z);
				}else{
					mPList.UpdateList(mAppSockets.GetTempPacket());
				}
			}else if(!mPList.CheckList(mAppSockets.GetTempPacket().GetCID())){
				mPList.AddToList(mAppSockets.GetTempPacket());
			}
		}
	}
}
void NetworkApp::NetworkKeyPresses(){
	if(GetAsyncKeyState(VK_ESCAPE)& 0x8000){
		PostQuitMessage(0);
	}else if(GetAsyncKeyState(VK_F1)& 0x8000){
		if(mAppSockets.GetInitialised()){
			mAppSockets.SendTo();
			mAppSockets.RedrawText();
		}
	}else if(GetAsyncKeyState('C')& 0x8000){
		if(mAppSockets.GetInitialised()){
			mAppSockets.Connect();
			mAppSockets.RedrawText();
		}
	}else if(GetAsyncKeyState('P')& 0x8000){
		ShowWin32Console();
	}else if(GetAsyncKeyState('T')& 0x8000){
		if(mAppSockets.GetInitialised()){
			mAppSockets.SendAll();
			mAppSockets.RedrawText();
		}
	}else if(GetAsyncKeyState('Y')& 0x8000){
		if(!mAppSockets.GetInitialised()){
			cout << "create server\n";
			mAppSockets.Init(0);
			mAppSockets.SetAsync(mhMainWnd);
			mAppSockets.RedrawText();
			mPList.AddToList(mAppSockets.GetTempPacket());
		}
	}else if(GetAsyncKeyState('N')& 0x8000){
		if(!mAppSockets.GetInitialised()){
			cout << "create client\n";
			mAppSockets.Init(1);
			mAppSockets.SetAsync(mhMainWnd);
			mAppSockets.RedrawText();
		}
	}
}
void NetworkApp::ObjectKeyPresses(){
	Speed tSpeed;			
	tSpeed= mCube.GetSpeed();
	if(GetAsyncKeyState(VK_DOWN)& 0x8000 ){
		tSpeed.UpdateDir(0,-0.1,0);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000){
		tSpeed.UpdateDir(0,0.1,0);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000){
		tSpeed.UpdateDir(-0.1,0,0);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000){
		tSpeed.UpdateDir(0.1,0,0);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('Q') & 0x8000){
		tSpeed.UpdateDir(0,0,0.1);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('E') & 0x8000){
		tSpeed.UpdateDir(0,0,-0.1);
		mCube.ChangeSpeed(tSpeed);
	}
	if(GetAsyncKeyState('M') & 0x8000){
		D3DXVECTOR3 temp(0,0,0);
		tSpeed.UpdateDir(temp);
		mCube.ChangeSpeed(tSpeed);
	}	

	if(mAppSockets.GetInitialised()){
		if(mAppSockets.GetServer()){
			mSphere.RandDir();
		}
		mSphere.Move();
	}
	if(mPList.Move(mCube)){
		mAppSockets.UpdatePacket(mCube.GetPos(), mCube.GetSpeed());
	}
}
void NetworkApp::CameraKeyPresses(){
	//camera movement
	if(GetAsyncKeyState('A') & 0x8000)	mCamera.MoveLeft();
	if(GetAsyncKeyState('D') & 0x8000)	mCamera.MoveRight();
	if(GetAsyncKeyState('W') & 0x8000)	mCamera.MoveForward();
	if(GetAsyncKeyState('S') & 0x8000)	mCamera.MoveBack();
	if(GetAsyncKeyState('Z') & 0x8000)	mCamera.MoveUp();
	if(GetAsyncKeyState('X') & 0x8000)	mCamera.MoveDown();
	/*
		//look around the screen using the mouse
		if(mMousePos.x < (50)){
			mCamera.OnlyYaw(-2);
		}
		else if(mMousePos.x > (mClientWidth-50)){
			mCamera.OnlyYaw(2);
		}
		if(mMousePos.y < (50)){
			mCamera.OnlyPitch(2);
		}
		else if(mMousePos.y > (mClientHeight-50)){
			mCamera.OnlyPitch(-2);
		}
	*/
}