#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>

using namespace std;

void startProcess(STARTUPINFO& clientStartupInfo, PROCESS_INFORMATION& clientProcessInformation, LPSTR lpClientProcessName, LPSTR lpCommandLine);

int main(int argc, char* argv[])
{
	LPSTR  SemaphoreName1 = "Semaphore1";
	LPSTR  SemaphoreName2 = "Semaphore2";
	//==============================================================================//
	//								�������� ���������								//
	//==============================================================================//
	HANDLE hSemaphore1 = CreateSemaphore(
		NULL,				// ��� ��������
		0,					// ��������� ���������
		1,					// ������������ ���������
		SemaphoreName1	// ��� ��������
	);
	HANDLE hSemaphore2 = CreateSemaphore(
		NULL,				// ��� ��������
		0,					// ��������� ���������
		1,					// ������������ ���������
		SemaphoreName2	// ��� ��������
	);
	if (hSemaphore1 == NULL || hSemaphore2 == NULL)
	{
		MessageBox(NULL, "Create Semaphore Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}


	//==============================================================================//
	//								���� ��������									//
	//==============================================================================//
	STARTUPINFO clientStartupInfo1;
	PROCESS_INFORMATION clientProcessInformation1;
	LPSTR lpClientProcessName1 =  "..\\Debug\\COMP1.exe";
	startProcess(clientStartupInfo1, clientProcessInformation1, lpClientProcessName1, NULL);
	STARTUPINFO clientStartupInfo2;
	PROCESS_INFORMATION clientProcessInformation2;
	LPSTR lpClientProcessName2 ="..\\Debug\\COMP2.exe";
	startProcess(clientStartupInfo2, clientProcessInformation2, lpClientProcessName2, NULL);
	STARTUPINFO clientStartupInfo3;
	PROCESS_INFORMATION clientProcessInformation3;
	LPSTR lpClientProcessName3 = "..\\Debug\\WIRE.exe";
	startProcess(clientStartupInfo3, clientProcessInformation3, lpClientProcessName3, NULL);






	_getch();
	CloseHandle(clientProcessInformation1.hThread);
	CloseHandle(clientProcessInformation1.hProcess);
	CloseHandle(clientProcessInformation2.hThread);
	CloseHandle(clientProcessInformation2.hProcess);
	CloseHandle(clientProcessInformation3.hThread);
	CloseHandle(clientProcessInformation3.hProcess);
	CloseHandle(hSemaphore1);
	CloseHandle(hSemaphore2);
	return 0;
}


void startProcess(STARTUPINFO& clientStartupInfo, PROCESS_INFORMATION& clientProcessInformation, LPSTR lpClientProcessName, LPSTR lpCommandLine)
{
	ZeroMemory(&clientProcessInformation, sizeof(clientProcessInformation));
	ZeroMemory(&clientStartupInfo, sizeof(clientStartupInfo));
	clientStartupInfo.cb = sizeof(clientStartupInfo);
	BOOL bCreateProcess = CreateProcess(
		lpClientProcessName,
		lpCommandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&clientStartupInfo,
		&clientProcessInformation
	);
	if (bCreateProcess == FALSE)
	{
		MessageBox(NULL, "Create Process Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}