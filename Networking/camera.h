#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "d3dUtil.h"

class Camera{
public:
	
	float Yaw, Pitch, Roll;
	bool yaw_on, pitch_on, roll_on, jump_on;
	int speed;
	float veloy, gravity;

	/*//used for storing forward, movement and position vectors
	struct cam{
		float x, y, z;
	}forward, up, movement, position;*/

	D3DXVECTOR3 forward;
	D3DXVECTOR3 up;
	D3DXVECTOR3 movement;
	D3DXVECTOR3 position;
	
	
	Camera();
	void CameraRightVector();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBack();
	void Look(D3DXMATRIX &mView);
	void Update(float roll, float pitch, float yaw);
	void Update();
	void OnlyPitch(float pitch);
	void OnlyRoll(float roll);
	void OnlyYaw(float yaw);
	void Jump();
	void CheckGround();

private:

};

#endif