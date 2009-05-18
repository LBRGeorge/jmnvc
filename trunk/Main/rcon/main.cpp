#include "main.h"

void logprintf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char buffer[512];
	vsprintf(buffer, format, ap);
	va_end(ap);
	puts(buffer);
}

CRconClient* pRconClient = NULL;
bool bQuitApp = false;

BOOL WINAPI CtrlHandler(DWORD dwType)
{
	switch (dwType)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			bQuitApp = true;
			return TRUE;
	}
	return FALSE;
}

HANDLE hConsoleExecuteEvent;

DWORD WINAPI ConsoleInputThread(void* pParam)
{
	char buf[512];
	while (true)
	{
		DWORD dwRead;
		ReadConsole(GetStdHandle(STD_INPUT_HANDLE), buf, 255, &dwRead, NULL);
		if (dwRead > 2)
		{
			buf[dwRead-2] = 0;
			WaitForSingleObject(hConsoleExecuteEvent, INFINITE);
			pRconClient->Command(buf);
			SetEvent(hConsoleExecuteEvent);
		}
	}
}

int main (int argc, char** argv)
{
	puts("\n JVC:MP Command Line Remote Console Client");
	puts(" ----------------------------------------");
	puts(" v1.0, 26th Aug 2005\n");

	if (argc < 3)
	{
		puts(" Usage:");
		puts("   rcon <ip/host> <port> <password>");
		getc(stdin);
		exit(0);
	}

	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	hConsoleExecuteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleInputThread, NULL, 0, &dwThreadId);
	pRconClient = new CRconClient(argv[1], atoi(argv[2]), argv[3]);

	while (!bQuitApp)
	{
		pRconClient->Process();
		SetEvent(hConsoleExecuteEvent);
		WaitForSingleObject(hConsoleExecuteEvent, INFINITE);
		Sleep(100);
	}

	delete pRconClient;
	TerminateThread(hThread, 0);
	CloseHandle(hConsoleExecuteEvent);
	return 0;
}