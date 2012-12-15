#include "PlayerList.h"

PlayerList::PlayerList(){
	LocalID = 0;
}
PlayerList::~PlayerList(){

}
void PlayerList::Init(ID3D10Device* Device){
	md3dDevice = Device;
}
void PlayerList::AddToList(MyPackets tempPacket){
	Players tPlayer;
	tPlayer.PlayerCube.init(md3dDevice);
	tPlayer.ID = tempPacket.CID;
	tPlayer.PlayerCube.TranslateTo(tempPacket.pos.x, tempPacket.pos.y, tempPacket.pos.z);
	tPlayer.PlayerCube.ChangeSpeed(tempPacket.PacketSpeed);
	LocalList.push_back(tPlayer);
}
void PlayerList::UpdateList(MyPackets tempPacket){
	for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
		if(PlayerListIT->ID == tempPacket.CID){
			PlayerListIT->PlayerCube.TranslateTo(tempPacket.pos.x, tempPacket.pos.y, tempPacket.pos.z);
			PlayerListIT->PlayerCube.ChangeSpeed(tempPacket.PacketSpeed);
		}
	}
}
bool PlayerList::CheckList(int tempID){
	for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
		if(PlayerListIT->ID==tempID){
			return true;
		}
	}
	return false;
}
bool PlayerList::Move(Cube &tempCube){
	if(LocalList.size()!=0){
		for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
			if(PlayerListIT->ID==LocalID){
				PlayerListIT->PlayerCube = tempCube;
				PlayerListIT->PlayerCube.Move();
				tempCube = PlayerListIT->PlayerCube;
				return true;
			}else{
				PlayerListIT->PlayerCube.Move();
				return false;
			}
		}
	}
	return false;
}
void PlayerList::Draw(D3DXMATRIX mView,D3DXMATRIX mProj,ID3D10EffectMatrixVariable* mfxWVPVar,ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* mTech){
	if(LocalList.size()!=0){
		for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
			PlayerListIT->PlayerCube.draw(mView,mProj,mfxWVPVar,mfxWorldVar,mTech);
		}
	}
}
void PlayerList::SetLocalID(int LID){
	LocalID = LID;
}
std::list<Players> PlayerList::GetList(){
	return LocalList;
}
std::list<Players>::iterator PlayerList::GetIterator(){
	return PlayerListIT;
}