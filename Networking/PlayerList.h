#ifndef _PLAYERLIST_H_
#define _PLAYERLIST_H_

#include "cube.h"
#include "Packet.h"
#include "d3dUtil.h"
#include <list>

typedef struct Players{
	int ID;
	Cube PlayerCube;

	Players(){
		ID = 0;
	}
}Players;

class PlayerList{
public:
	PlayerList();
	~PlayerList();
	void Init(ID3D10Device* Device);
	void AddToList(MyPackets tempPacket);
	void UpdateList(MyPackets tempPacket);
	bool Move(Cube &tempCube);
	bool CheckList();
	void SetLocalID(int LID);
	void Draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech);
private:
	std::list<Players> LocalList;
	std::list<Players>::iterator PlayerListIT;
	ID3D10Device* md3dDevice;
	int LocalID;
};

#endif