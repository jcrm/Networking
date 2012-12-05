#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, 
		float cr, float cg, float cb, float ca)
		: pos(x,y,z), colour(cr,cg,cb,ca){}

	D3DXVECTOR3 pos;
	D3DXVECTOR4 colour;
};

#endif // VERTEX_H


