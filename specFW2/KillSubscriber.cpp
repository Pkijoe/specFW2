
#include "stdafx.h"
#include "KillSubscriber.h"

DWORD SubscribeToBeKilled(char *applicationName, HWND hWnd)
{
	DWORD dwWritten;

	char fileString[512];

	sprintf(fileString,"%s:%08x\r\n",applicationName,hWnd);
		HANDLE hLogfile = CreateFile(	TEXT("\\My Documents\\KillThese.txt"),  //\\Storage Card
									GENERIC_WRITE,
									0,
									NULL,
									OPEN_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
		SetFilePointer( hLogfile, NULL,NULL,FILE_END);
		WriteFile(	hLogfile,(LPVOID) fileString,strlen(fileString),&dwWritten,NULL);
		CloseHandle(hLogfile);

		return S_OK;

}
