#include "triangle.h"

Triangle::Triangle(){

}
void Triangle::draw(){
}
void Triangle::draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
	GfxObj::draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
}
float Triangle::GetPosX(){
	return pos.x;
}
float Triangle::GetPosY(){
	return pos.y;
}
float Triangle::GetPosZ(){
	return pos.z;
}
Triangle::~Triangle(){

}
void Triangle::DrawVertices(){
	/*Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f+pos.x, -1.0f+pos.y, 0.0f+pos.z)},
		{D3DXVECTOR3(-1.0f+pos.x, +1.0f+pos.y, 0.0f+pos.z)},
		{D3DXVECTOR3(+1.0f+pos.x, +1.0f+pos.y, 0.0f+pos.z)},		
    };

	for(DWORD i = 0; i < mNumVertices; ++i){
		vertices[i].pos.x *= scale.x;
		vertices[i].pos.y *= scale.y;
		vertices[i].pos.z *= scale.z;
	}
	// Create vertex buffer
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));*/
}
void Triangle::init(ID3D10Device* device){
	md3dDevice = device;
	mNumVertices=	3;
	mNumFaces=		1;
	DrawVertices();
}
