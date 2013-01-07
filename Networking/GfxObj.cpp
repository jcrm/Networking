#include "GfxObj.h"

GfxObj::GfxObj()
: mNumVertices(0),mNumIndices(0), mNumFaces(0), 
md3dDevice(0), mVB(0), mIB(0), scale(1,1,1),pos(0,0,0),
theta(0,0,0), indexed(true){
	D3DXMatrixIdentity(&world);
}

GfxObj::~GfxObj(){
}
D3DXVECTOR3 GfxObj::GetPos(){
	return pos;
}
D3DXVECTOR3 GfxObj::GetColour(){
	return mColour;
}
void GfxObj::ChangeSpeed(Speed tempSpeed){
	mySpeed = tempSpeed;
}
Speed GfxObj::GetSpeed(){
	return mySpeed;
}
void GfxObj::Move(){
	float dx, dy, dz;
	dx = mySpeed.mDir.x * mySpeed.mVelo;
	dy = mySpeed.mDir.y * mySpeed.mVelo;
	dz = mySpeed.mDir.z * mySpeed.mVelo;
	Translate(dx,dy,dz);
}
void GfxObj::setTrans(void){
	D3DXMATRIX m;
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&m, scale.x, scale.y, scale.z);
	world*=m;
	D3DXMatrixRotationY(&m, theta.x);
	world*=m;
	D3DXMatrixRotationY(&m, theta.y);
	world*=m;
	D3DXMatrixRotationY(&m, theta.z);
	world*=m;
	D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);
	world*=m;
}
void GfxObj::TranslateTo(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;
	setTrans();
}
void GfxObj::Translate(float x, float y, float z){
	pos.x += x;
	pos.y += y;
	pos.z += z;
	setTrans();
}
void GfxObj::Translate(D3DXVECTOR3 temp){
	pos += temp;
	setTrans();
}
void GfxObj::Scale(float x, float y, float z){
	scale.x = x;
	scale.y = y;
	scale.z = z;
	setTrans();
}
D3DXMATRIX GfxObj::GetWorld(){
	return world;
}
// Virtual methods.  Derived client class overrides these methods to 
// implement specific object requirements.

void GfxObj::init (ID3D10Device* device){
	md3dDevice=		device;
}
void GfxObj::init (ID3D10Device* device, D3DXVECTOR3 c){
	md3dDevice=		device;
	mColour = D3DXVECTOR3(c.x,c.y,c.z);
}
void GfxObj::buildVB(VertexList& vertices){
	// Create vertex buffer
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}
void GfxObj::buildIB(IndexList& indices){
	// Create index buffer
	D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.Usage           = D3D10_USAGE_DEFAULT;
    bufferDesc.ByteWidth       = sizeof( unsigned int ) * mNumIndices;
    bufferDesc.BindFlags       = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags  = 0;
    bufferDesc.MiscFlags       = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &indices[0];
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    HR(md3dDevice->CreateBuffer( &bufferDesc, &InitData, &mIB));
}
void GfxObj::draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
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
void GfxObj::CaclNewDir(D3DXVECTOR3 tempPackPos, D3DXVECTOR3 tempPackDir){
	const static float time = 500.0f;
	float velo = mySpeed.mVelo;
	float curPosX, curPosY, curPosZ;
	curPosX = pos.x;
	curPosY = pos.y;
	curPosZ = pos.z;
//!-------------------------------------
	float pacVeloX, pacVeloY, pacVeloZ;
	pacVeloX = tempPackDir.x;
	pacVeloY = tempPackDir.y;
	pacVeloZ = tempPackDir.z;

	float pacPosX, pacPosY, pacPosZ;
	pacPosX = tempPackPos.x;
	pacPosY = tempPackPos.y;
	pacPosZ = tempPackPos.z;

	float actPosX, actPosY, actPosZ;
	actPosX = actPosY = actPosZ = 0.0f;

	float tempVeloX, tempVeloY, tempVeloZ;
	tempVeloX = tempVeloY = tempVeloZ = 0.0f;
//!-------------------------------------
	actPosX = pacPosX + (pacVeloX * time * velo);
	actPosY = pacPosY + (pacVeloY * time * velo);
	actPosZ = pacPosZ + (pacVeloZ * time * velo);

	tempVeloX = (actPosX-curPosX)/(time * velo);
	tempVeloY = (actPosY-curPosY)/(time * velo);
	tempVeloZ = (actPosZ-curPosZ)/(time * velo);

	mySpeed.mDir = D3DXVECTOR3(tempVeloX,tempVeloY, tempVeloZ);
}