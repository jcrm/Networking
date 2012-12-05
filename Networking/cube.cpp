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

	v.colour = D3DXVECTOR4(RandF(),RandF(),RandF(),RandF());


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
	DrawVertices();
}






/*
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f)},		//A
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f)},		//D
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E

		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f)},		//E
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f)},		//H

		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f)},		//F
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f)},		//G
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f)},		//C
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f)},		//B*/





	/*Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f), WHITE, WHITE},		//A - 0
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXVECTOR3(-1.0f, +1.0f, -1.0f), WHITE, WHITE},		//B - 1
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR3(+1.0f, +1.0f, -1.0f), WHITE, WHITE},		//C - 2
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f), D3DXVECTOR3(+1.0f, -1.0f, -1.0f), WHITE, WHITE},		//D - 3

		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f), D3DXVECTOR3(-1.0f, -1.0f, +1.0f), WHITE, WHITE},		//E - 4
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR3(-1.0f, +1.0f, +1.0f), WHITE, WHITE},		//F - 5
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR3(+1.0f, +1.0f, +1.0f), WHITE, WHITE},		//G - 6
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f), D3DXVECTOR3(+1.0f, -1.0f, +1.0f), WHITE, WHITE},		//H - 7
		
    };*/


	/*// Create indices
    unsigned int indices[] = {	0, 1, 2, 2, 3, 0,
								1, 5, 6, 6, 2, 1,
								4, 0, 3, 3, 7, 4,
								4, 5, 1, 1, 0, 4,
								3, 2, 6, 6, 7, 3,
								7, 6, 5, 5, 4, 7};*/