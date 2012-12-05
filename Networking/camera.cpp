#include"camera.h"
#include "d3dUtil.h"
#include <math.h>
//yaw around y 
//pitch around x
//roll around z
//initialize all variables
Camera::Camera(){

	forward = 	D3DXVECTOR3( 0.0f, 0.0f,  0.0f);
	up = 		D3DXVECTOR3( 0.0f, 1.0f,  0.0f);
	movement = 	D3DXVECTOR3( 0.0f, 0.0f,  0.0f);
	position = 	D3DXVECTOR3( 0.0f, 0.0f,-10.0f);
	Yaw = 0;
	Pitch = 0;
	Roll = 0;
	/*
	forward.x = 0;
	forward.y = 0;
	forward.z = 0;

	up.x = 0;
	up.y = 1;
	up.z = 0;

	movement.x = 0;
	movement.y = 0;
	movement.z = 0;

	position.x = 0;
	position.y = 0;
	position.z = 6;*/

	speed = 16000;

	//can only rotate horizontally, and vertically
	yaw_on = true;
	pitch_on=true;
	roll_on=false;
	jump_on =false;
	veloy = 0;
	gravity = 0.2;
}
//used for calculating the movement vector it is a cross product of up and forward vector
void Camera::CameraRightVector(){
	Update();
	movement.x = ((forward.y * up.z) - (forward.z * up.y))/speed;
	movement.y = (0-(forward.x * up.z) - (forward.z * up.x))/speed;
	movement.z = ((forward.x * up.y) - (forward.y * up.z))/speed;
}
//move the vertically up by adding the up vector to the position vector
void Camera::MoveUp(){
	CameraRightVector();
	position.x += up.x/(speed/100);
	position.y += up.y/(speed/100);
	position.z += up.z/(speed/100);
}
//move the vertically down by subtracting the up vector from the position vector
void Camera::MoveDown(){
	CameraRightVector();
	position.x -= up.x/(speed/100);
	position.y -= up.y/(speed/100);
	position.z -= up.z/(speed/100);
}
//move the camera horizontally left subtracting the movement vector from the position vector
void Camera::MoveLeft(){
	CameraRightVector();
	position.x -= movement.x;
	position.y -= movement.y;
	position.z -= movement.z;
}
//move the camera horizontally right adding the movement vector to the position vector
void Camera::MoveRight(){
	CameraRightVector();
	position.x += movement.x;
	position.y += movement.y;
	position.z += movement.z;
}
//move the camera forward by adding the forward vector to the position vector then increasing the forward vector
void Camera::MoveForward(){
	CameraRightVector();
	position.x += forward.x/speed;
	position.z += forward.z/speed;

	forward.x+=forward.x/speed;
	forward.z+=forward.z/speed;
}
//move the camera forward by subtracting the forward vector from the position vector then decreasing the forward vector
void Camera::MoveBack(){
	CameraRightVector();
	position.x -= forward.x/speed;
	position.z -= forward.z/speed;

	forward.x-=forward.x/speed;
	forward.z-=forward.z/speed;
}
//pitch between a predetermined range
void Camera::OnlyPitch(float pitch)
{
	if(pitch_on){
		if(Pitch <=45 && Pitch >= -45){
			Pitch+=pitch;
		}
		if(Pitch > 45){
			Pitch = 45;
		} else if(Pitch < -45){
			Pitch = -45;
		}
	}
}
//roll by a given amount
void Camera::OnlyRoll(float roll)
{
	if(roll_on){
		Roll+=roll;
	}
}
//yaw by a given amount
void Camera::OnlyYaw(float yaw)
{
	if(yaw_on){
		Yaw+=yaw;
	}
}
//update the forward and up vector based upon roll, pitch and yaw
void Camera::Update(float roll, float pitch, float yaw){
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	Yaw = yaw;
	Pitch = pitch;
	Roll = roll;

	cosY = cosf(Yaw*3.1415/180);
	cosP = cosf(Pitch*3.1415/180);
	cosR = cosf(Roll*3.1415/180);

	sinY = sinf(Yaw*3.1415/180);
	sinP = sinf(Pitch*3.1415/180);
	sinR = sinf(Roll*3.1415/180);

	forward.x = sinY * cosP * 360;
	forward.y = sinP * 360;
	forward.z = cosP * -cosY * 360;

	up.x = -cosY*sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY*sinR - sinP * cosR *-cosY ;
	
	CheckGround();
}
void Camera::Look(D3DXMATRIX &mView){
	D3DXMatrixLookAtLH(&mView, &position, &forward, &up);
}
//update the up and forward vectors
void Camera::Update(){
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	cosY = cosf(Yaw*3.1415/180);
	cosP = cosf(Pitch*3.1415/180);
	cosR = cosf(Roll*3.1415/180);

	sinY = sinf(Yaw*3.1415/180);
	sinP = sinf(Pitch*3.1415/180);
	sinR = sinf(Roll*3.1415/180);

	forward.x = sinY * cosP * 360;
	forward.y = sinP * 360;
	forward.z = cosP * cosY * 360;

	up.x = -cosY*sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY*sinR - sinP * cosR *-cosY ;

	//CheckGround();
	
}
//make sure that the camera is not below ground level and if above ground level decrease until it returns to ground level
//used for jumping
void Camera::CheckGround(){
	int static timer = 0;
	int const max_time = 1;

	if(timer == max_time){
		timer = 0;
		position.y += veloy;
		veloy -= gravity;
		if(position.y <=0){
			position.y = 0;
			veloy = 0;
			jump_on = false;
		}
	}
	timer++;
}
//if jump was pressed start the camera jump process 
void Camera::Jump(){
	if(!jump_on){
		veloy=1;
		jump_on = true;
	}
}