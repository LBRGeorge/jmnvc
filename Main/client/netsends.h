#ifndef _NETSENDS_H
#define _NETSENDS_H

class CNetSends
{
public:
	static void						ServerCall					( BOOL bSequenced, char* szCallName, RakNet::BitStream bsSend );
	static void						Death						( BYTE byteDeathReason, BYTE byteWhoWasResponsible );
	static void						RequestClass				( BYTE byteClass );
	static void						Spawn						( void );
	static void						Chat						( PCHAR szText );
	static void						EnterVehicle				( BYTE byteVehicleID, BYTE bytePassenger );
	static void						ExitVehicle					( BYTE byteVehicleID );
	static void						KickPlayer					( BYTE bytePlayerID );
	static void						BanIP						( PCHAR szIP );
	static void						AdminLogin					( PCHAR szPassword );
};

#endif
