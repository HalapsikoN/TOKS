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
	LPSTR  SemaphoreName3 = "Semaphore3";
	LPSTR  SemaphoreName4 = "Semaphore4";
	//==============================================================================//
	//								СОЗДАНИЕ СЕМАФОРОВ								//
	//==============================================================================//
	HANDLE hSemaphore1 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName1	// имя семафора
	);
	HANDLE hSemaphore2 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName2	// имя семафора
	);
	HANDLE hSemaphore3 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName3	// имя семафора
	);
	HANDLE hSemaphore4 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName4	// имя семафора
	);
	if (hSemaphore1 == NULL || hSemaphore2 == NULL || hSemaphore3 == NULL || hSemaphore4 == NULL)
	{
		MessageBox(NULL, "Create Semaphore Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}


	//==============================================================================//
	//								САМИ ПРОЦЕССЫ									//
	//==============================================================================//
	STARTUPINFO clientStartupInfo1;
	PROCESS_INFORMATION clientProcessInformation1;
	LPSTR lpClientProcessName1 = "..\\Debug\\STATION1.exe";
	startProcess(clientStartupInfo1, clientProcessInformation1, lpClientProcessName1, NULL);
	STARTUPINFO clientStartupInfo2;
	PROCESS_INFORMATION clientProcessInformation2;
	LPSTR lpClientProcessName2 = "..\\Debug\\STATION2.exe";
	startProcess(clientStartupInfo2, clientProcessInformation2, lpClientProcessName2, NULL);
	STARTUPINFO clientStartupInfo3;
	PROCESS_INFORMATION clientProcessInformation3;
	LPSTR lpClientProcessName3 = "..\\Debug\\STATION3.exe";
	startProcess(clientStartupInfo3, clientProcessInformation3, lpClientProcessName3, NULL);

	HWND hWnd = GetConsoleWindow();
	cout << "Press to connect..." << endl;
	rewind(stdin);
	_getch();
	ReleaseSemaphore(hSemaphore4, 1, NULL);
	ShowWindow(hWnd, SW_HIDE);
	WaitForSingleObject(hSemaphore4, INFINITE);
	cout << "Connected." << endl<<endl;
	Sleep(10000);
	ShowWindow(hWnd, SW_SHOW);

	bool isPause = false;
	cout << "Press any key to pause and (q) to stop programm..." << endl;
	
	rewind(stdin);
	while (_getch() != 'q')
	{
		if (isPause)
		{
			isPause = false;
			cout << "Unpaused..." << endl;
			ResumeThread(clientProcessInformation1.hThread);
			ResumeThread(clientProcessInformation2.hThread);
			ResumeThread(clientProcessInformation3.hThread);
		}
		else
		{
			isPause = true;
			cout << "Paused..." << endl;
			SuspendThread(clientProcessInformation1.hThread);
			SuspendThread(clientProcessInformation2.hThread);
			SuspendThread(clientProcessInformation3.hThread);
		}
		rewind(stdin);
	}
	TerminateProcess(clientProcessInformation1.hProcess, 0);
	TerminateProcess(clientProcessInformation2.hProcess, 0);
	TerminateProcess(clientProcessInformation3.hProcess, 0);
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