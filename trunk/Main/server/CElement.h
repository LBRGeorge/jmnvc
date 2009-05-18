
/*
	**Still trying to make the CElement work**
*/

#pragma once

class CElement
{
private:
	int m_Type;
	bool m_bDestroyed;
	//CVehicle m_pVehicle;
public:

	CElement(int iType, void* pVoid);
	~CElement(){};

	BOOL IsDestroyed() { return m_bDestroyed; };

	void Destroy() {
		m_bDestroyed = true;
		if (m_Type==1){
			//delete m_pVehicle;
		}
		delete this;
	};

	int GetType() {
		return m_Type;
	};
};