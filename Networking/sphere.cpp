#include "Sphere.h"
#include "GfxObj.h"
Sphere::Sphere()
: mRadius(0.0f), mNumSlices(0), mNumStacks(0)
{
}

Sphere::~Sphere()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sphere::init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks)
{
	md3dDevice = device;
	mRadius    = radius;
	mNumSlices = numSlices;
	mNumStacks = numStacks;
	indexed = true;
	DrawVertices();

}
void Sphere::DrawVertices()
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	buildStacks(vertices, indices);
	
	mNumVertices = (UINT)vertices.size();
	mNumFaces    = (UINT)indices.size()/3;
	mNumIndices  = (UINT)indices.size();

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &InitData, &mIB));
}

void Sphere::draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
	GfxObj::draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
}
void Sphere::buildStacks(VertexList& vertices, IndexList& indices)
{
	float phiStep = PI/mNumStacks;

	// do not count the poles as rings
	UINT numRings = mNumStacks-1;

	// Compute vertices for each stack ring.
	for(UINT i = 1; i <= numRings; ++i)
	{
		float phi = i*phiStep;

		// vertices of ring
		float thetaStep = 2.0f*PI/mNumSlices;
		for(UINT j = 0; j <= mNumSlices; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;
			v.colour = D3DXVECTOR4(RandF(),RandF(),RandF(),RandF());
			// spherical to cartesian
			v.pos.x = mRadius*sinf(phi)*cosf(theta);
			v.pos.y = mRadius*cosf(phi);
			v.pos.z = mRadius*sinf(phi)*sinf(theta);

			vertices.push_back( v );
		}
	}

	// poles: note that there will be texture coordinate distortion
	vertices.push_back( Vertex(0.0f, -mRadius, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f) );
	vertices.push_back( Vertex(0.0f, mRadius, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f) );

	UINT northPoleIndex = (UINT)vertices.size()-1;
	UINT southPoleIndex = (UINT)vertices.size()-2;

	UINT numRingVertices = mNumSlices+1;

	// Compute indices for inner stacks (not connected to poles).
	for(UINT i = 0; i < mNumStacks-2; ++i)
	{
		for(UINT j = 0; j < mNumSlices; ++j)
		{
			indices.push_back(i*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j);

			indices.push_back((i+1)*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j+1);
		}
	}

	// Compute indices for top stack.  The top stack was written 
	// first to the vertex buffer.
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(northPoleIndex);
		indices.push_back(i+1);
		indices.push_back(i);
	}

	// Compute indices for bottom stack.  The bottom stack was written
	// last to the vertex buffer, so we need to offset to the index
	// of first vertex in the last ring.
	UINT baseIndex = (numRings-1)*numRingVertices;
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex+i);
		indices.push_back(baseIndex+i+1);
	}
}
 
