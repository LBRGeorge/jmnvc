#pragma once

#include "game.h"

//-----------------------------------------------------------

class CCamera
{
public:
	void SetBehindPlayer();
	void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);	// tested
	void LookAtPoint(float fX, float fY, float fZ, int iType);
	void Restore();
	
	CCamera(){};
	~CCamera(){};
};

//-----------------------------------------------------------
