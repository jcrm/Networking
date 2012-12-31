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
	tPlayer.ID = tempPacket.GetCID();
	tPlayer.PlayerCube.TranslateTo(tempPacket.GetPos().x, tempPacket.GetPos().y, tempPacket.GetPos().z);
	tPlayer.PlayerCube.ChangeSpeed(tempPacket.GetSpeed());
	LocalList.push_back(tPlayer);
}
void PlayerList::UpdateList(MyPackets tempPacket){
	for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
		if(PlayerListIT->ID == tempPacket.GetCID()){
			PlayerListIT->PlayerCube.CaclNewDir(tempPacket.GetPos(),tempPacket.GetSpeed().mDir);
			//PlayerListIT->PlayerCube.TranslateTo(tempPacket.GetPos().x, tempPacket.GetPos().y, tempPacket.GetPos().z);
			//PlayerListIT->PlayerCube.ChangeSpeed(tempPacket.GetSpeed());
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
	bool temp= false;
	if(LocalList.size()!=0){
		for(PlayerListIT=LocalList.begin(); PlayerListIT!=LocalList.end();PlayerListIT++){
			if(PlayerListIT->ID==LocalID){
				PlayerListIT->PlayerCube = tempCube;
				PlayerListIT->PlayerCube.Move();
				tempCube = PlayerListIT->PlayerCube;
				temp = true;
			}else{
				PlayerListIT->PlayerCube.Move();
			}
		}
	}
	return temp;
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