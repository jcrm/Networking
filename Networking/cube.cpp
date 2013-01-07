#include "cube.h"

Cube::Cube(){

}
void Cube::draw(){
}
void Cube::draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
	GfxObj::draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
}
float Cube::GetPosX(){
	return pos.x;
}
float Cube::GetPosY(){
	return pos.y;
}
float Cube::GetPosZ(){
	return pos.z;
}
Cube::~Cube(){

}
void Cube::buildVertices(VertexList& vertices, IndexList& indices){
	Vertex v;

	v.colour = D3DXVECTOR4(mColour.x,mColour.y,mColour.z,1.0f);

	//Front
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, -1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, -1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, -1.0f);
	vertices.push_back(v);

	//Back
	v.pos = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	vertices.push_back(v);
	v.pos = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);
	vertices.push_back(v);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(7);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(4);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(3);
}

void Cube::DrawVertices(){

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	buildVertices(vertices, indices);
	
	mNumVertices = (UINT)vertices.size();
	mNumFaces    = (UINT)indices.size()/3;
	mNumIndices  = (UINT)indices.size();

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
void Cube::init(ID3D10Device* device){
	md3dDevice = device;
	mNumVertices=	8;
	mNumFaces=		12;
	mNumIndices =	36;
	indexed = true;
	mColour = D3DXVECTOR3(RandF(),RandF(),RandF());
	DrawVertices();
}
void Cube::init(ID3D10Device* device, D3DXVECTOR3 c){
	md3dDevice = device;
	mNumVertices=	8;
	mNumFaces=		12;
	mNumIndices =	36;
	indexed = true;
	mColour = D3DXVECTOR3(c.x,c.y,c.z);
	DrawVertices();
}