#pragma once
class CNetStats
{
private:

	DWORD m_dwLastTotalBytesSent;
	DWORD m_dwLastTotalBytesRecv;
	DWORD m_dwLastUpdateTick;
	DWORD m_dwBPSUpload;
	DWORD m_dwBPSDownload;

public:
	CNetStats();
	~CNetStats() {};

	void Draw();
};
