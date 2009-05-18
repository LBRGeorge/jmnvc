#include "anticheat.h"

extern CChatWindow *pChatWindow;
extern BOOL			bAntiCheat;
extern BOOL			bGameInited;

BOOL MemoryModificationCheck()
{
	return TRUE;
}

BOOL FileCheckSum()
{
	if(bAntiCheat)
	{		
		mdFile* mdCheckSumFile;

		mdCheckSumFile = new mdFile;

		sprintf(mdCheckSumFile->szFileLocation, "data/default.ide");
		mdCheckSumFile->iCorrectDigestArray[0] = 0;
		mdCheckSumFile->iCorrectDigestArray[1] = 1981298251;
		mdCheckSumFile->iCorrectDigestArray[2] = 1376643526;
		mdCheckSumFile->iCorrectDigestArray[3] = 3101585329;

		if (!CheckFile(mdCheckSumFile))
		{
			bGameInited = TRUE;
			return FALSE;
		}
		
		delete mdCheckSumFile;

		mdCheckSumFile = new mdFile;

		sprintf(mdCheckSumFile->szFileLocation, "data/handling.cfg");
		mdCheckSumFile->iCorrectDigestArray[0] = 0;
		mdCheckSumFile->iCorrectDigestArray[1] = 3465663929;
		mdCheckSumFile->iCorrectDigestArray[2] = 1196138439;
		mdCheckSumFile->iCorrectDigestArray[3] = 3152470125;

		if (!CheckFile(mdCheckSumFile))
		{
			bGameInited = TRUE;
			return FALSE;
		}
		
		delete mdCheckSumFile;

		mdCheckSumFile = new mdFile;

		sprintf(mdCheckSumFile->szFileLocation, "data/weapon.dat");
		mdCheckSumFile->iCorrectDigestArray[0] = 0;
		mdCheckSumFile->iCorrectDigestArray[1] = 1559786538;
		mdCheckSumFile->iCorrectDigestArray[2] = 4059666828;
		mdCheckSumFile->iCorrectDigestArray[3] = 1862227152;

		if (!CheckFile(mdCheckSumFile))
		{
			bGameInited = TRUE;
			return FALSE;
		}
		
		delete mdCheckSumFile;

		mdCheckSumFile = new mdFile;

		sprintf(mdCheckSumFile->szFileLocation, "data/jmnvc.SCM");
		mdCheckSumFile->iCorrectDigestArray[0] = 0;
		mdCheckSumFile->iCorrectDigestArray[1] = 1330870614;
		mdCheckSumFile->iCorrectDigestArray[2] = 2852418798;
		mdCheckSumFile->iCorrectDigestArray[3] = 2757762407;

		if (!CheckFile(mdCheckSumFile))
		{
			//bGameInited = TRUE;
			//return FALSE;
		}
		
		delete mdCheckSumFile;

		mdCheckSumFile = new mdFile;

		sprintf(mdCheckSumFile->szFileLocation, "data/pedstats.dat");
		mdCheckSumFile->iCorrectDigestArray[0] = 0;
		mdCheckSumFile->iCorrectDigestArray[1] = 1903937073;
		mdCheckSumFile->iCorrectDigestArray[2] = 2707443107;
		mdCheckSumFile->iCorrectDigestArray[3] = 385086949;

		if (!CheckFile(mdCheckSumFile))
		{
			bGameInited = TRUE;
			return FALSE;
		}
		
		delete mdCheckSumFile;
	}
	pChatWindow->AddDebugMessage("ModBot->No mods installed!");
	return TRUE;
}

BOOL CheckFile(mdFile* mdChkFile)
{
	FILE* fLocalFile;
	fLocalFile = fopen(mdChkFile->szFileLocation, "r");
	int x;
	unsigned char szBuffer[1024];

	if(!fLocalFile)
	{
		pChatWindow->AddDebugMessage("Error trying to load %s", mdChkFile->szFileLocation);
		return FALSE;
	}
	else
	{
		MD5_CTX mdCTX;
		MD5Init(&mdCTX);
		while((x = fread(szBuffer, 1, sizeof(szBuffer), fLocalFile) > 0)) MD5Update(&mdCTX, szBuffer, x);
		MD5Final(mdChkFile->szRawLocalDigest, &mdCTX);

		if(ferror(fLocalFile))
		{
			pChatWindow->AddDebugMessage("Kicked for cheating: \'%s\'.", mdChkFile->szFileLocation);
			fclose(fLocalFile);
			return FALSE;
		}
		
		// use following output to retreive md5 uint32 array's in md5sums.html
		//
		if (mdChkFile->szFileLocation == "data/jmnvc.SCM"){
		FILE* md5sums = fopen("md5sums.html", "a");
		fprintf(md5sums, "[%s]: mdCTX UINT32 buf[]: %u %u %u %u<br>\n", mdChkFile->szFileLocation, mdCTX.buf[0], mdCTX.buf[1], mdCTX.buf[2], mdCTX.buf[3]);
		// fprintf(md5sums, "[%s]: mdChkFile->iCorrectDigestArray[]: %u %u %u %u<br>\n", mdChkFile->szFileLocation, mdChkFile->iCorrectDigestArray[0], mdChkFile->iCorrectDigestArray[1], mdChkFile->iCorrectDigestArray[2], mdChkFile->iCorrectDigestArray[3]);
		fclose(md5sums);
		}
		//

		if((mdChkFile->iCorrectDigestArray[0] != mdCTX.buf[0]) || (mdChkFile->iCorrectDigestArray[1] != mdCTX.buf[1]) || (mdChkFile->iCorrectDigestArray[2] != mdCTX.buf[2]) || (mdChkFile->iCorrectDigestArray[3] != mdCTX.buf[3]))
		{
			pChatWindow->AddDebugMessage("Kicked for cheating: \'%s\'.", mdChkFile->szFileLocation);
			fclose(fLocalFile);
			return FALSE;
		}
	}

	fclose(fLocalFile);
	//pChatWindow->AddDebugMessage("Loaded \'%s\'!", mdChkFile->szFileLocation);
	return TRUE;
}
		
