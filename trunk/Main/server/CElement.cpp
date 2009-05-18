#include "netgame.h"
#include <math.h>

extern CNetGame *pNetGame;

CElement::CElement(int iType, void* pVoid)
{
	if (iType==1){
		//m_pVehicle = (CVehicle*)pVoid;
		m_bDestroyed = false;
	}
}