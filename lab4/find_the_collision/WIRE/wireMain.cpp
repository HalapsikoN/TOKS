#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>

using namespace std;

int main()
{
	cout << "WORKWIRE IS WORKING" << endl;
	//==============================================================================//
	//								СОЗДАНИЕ КАНАЛА									//
	//==============================================================================//

	BOOL   fConnected1;									// Флаг успешного создания канала
	HANDLE hNamedPipe1;									// Идентификатор канала Pipe
	LPSTR  lpszPipeName1 = "\\\\.\\pipe\\Pipe1";			// Имя создаваемого канала Pipe
	char   szBuf1[BUFSIZ];								// Буфер для передачи данных через канал
	DWORD  cbWritten1;									// Количество байт данных, переданных через канал
	DWORD  cbRead1=0;

	hNamedPipe1 = CreateNamedPipe(
		lpszPipeName1,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZ, BUFSIZ, 5000, NULL);

	if (hNamedPipe1 == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Create Pipe1 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	else
	{
		cout << "Created Pipe1" << endl;
	}

	BOOL   fConnected2;									// Флаг успешного создания канала
	HANDLE hNamedPipe2;									// Идентификатор канала Pipe
	LPSTR  lpszPipeName2 = "\\\\.\\pipe\\Pipe2";			// Имя создаваемого канала Pipe
	char   szBuf2[BUFSIZ];								// Буфер для передачи данных через канал
	DWORD  cbWritten2;									// Количество байт данных, переданных через канал
	DWORD  cbRead2=0;

	hNamedPipe2 = CreateNamedPipe(
		lpszPipeName2,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZ, BUFSIZ, 5000, NULL);

	if (hNamedPipe2 == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Create Pipe2 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	else
	{
		cout << "Created Pipe2" << endl;
	}

	//==============================================================================//
	//								ПОДКЛЮЧЕНИЕ СЕМАФОР								//
	//==============================================================================//
	LPSTR  SemaphoreName1 = "Semaphore1";
	LPSTR  SemaphoreName2 = "Semaphore2";
	HANDLE hSemaphore1 = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName1);
	if (hSemaphore1 == NULL)
	{
		MessageBox(NULL, "Open sem1 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	HANDLE hSemaphore2 = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName2);
	if (hSemaphore2 == NULL)
	{
		MessageBox(NULL, "Open sem2 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}



	cout << "Press key to connect the stations..." << endl;
	rewind(stdin);
	_getch();
	ReleaseSemaphore(hSemaphore1, 1, NULL);
	ReleaseSemaphore(hSemaphore2, 1, NULL);
	WaitForSingleObject(hSemaphore1, INFINITE);
	WaitForSingleObject(hSemaphore2, INFINITE);

	while (TRUE)
	{

		while (cbRead1 == 0 && cbRead2 == 0)
		{
			ReadFile(hNamedPipe1, szBuf1, BUFSIZ, &cbRead1, NULL);
			ReadFile(hNamedPipe2, szBuf2, BUFSIZ, &cbRead2, NULL);
		}
		int timeSleep = 1000;
		if (cbRead1 != 0)
		{
			cout << "open collision window (" << (double)timeSleep / 1000 << ") sec:" << endl;
			while (cbRead2 == 0 && timeSleep > 0)
			{
				if (timeSleep % 500 == 0)
				{
					cout << (double)timeSleep / 1000 << " sec..." << endl;
				}
				ReadFile(hNamedPipe2, szBuf2, BUFSIZ, &cbRead2, NULL);
				timeSleep--;
				Sleep(1);
			}
			cout << "close collision window" << endl;

			if (cbRead2 == 0)
			{
				WriteFile(hNamedPipe2, szBuf1, BUFSIZ, &cbWritten2, NULL);
				WriteFile(hNamedPipe1, szBuf1, BUFSIZ, &cbWritten1, NULL);
				ReleaseSemaphore(hSemaphore1, 1, NULL);
				ReleaseSemaphore(hSemaphore2, 1, NULL);
			}
			else
			{
				strcpy_s(szBuf1, "collision");
				WriteFile(hNamedPipe1, szBuf1, BUFSIZ, &cbWritten1, NULL);
				strcpy_s(szBuf2, "collision");
				WriteFile(hNamedPipe2, szBuf2, BUFSIZ, &cbWritten2, NULL);
				ReleaseSemaphore(hSemaphore1, 1, NULL);
				ReleaseSemaphore(hSemaphore2, 1, NULL);
			}
			cout << "otpravil" << endl;
		}
		else
		{
			cout << "open collision window (" << (double)timeSleep / 1000 << ") sec:" << endl;
			while (cbRead1 == 0 && timeSleep > 0)
			{
				if (timeSleep % 500 == 0)
				{
					cout << (double)timeSleep / 1000 << " sec..." << endl;
				}
				ReadFile(hNamedPipe1, szBuf1, BUFSIZ, &cbRead1, NULL);
				timeSleep--;
				Sleep(1);
			}
			cout << "close collision window" << endl;

			if (cbRead1 == 0)
			{
				WriteFile(hNamedPipe2, szBuf2, BUFSIZ, &cbWritten2, NULL);
				WriteFile(hNamedPipe1, szBuf2, BUFSIZ, &cbWritten1, NULL);
				ReleaseSemaphore(hSemaphore1, 1, NULL);
				ReleaseSemaphore(hSemaphore2, 1, NULL);
			}
			else
			{
				strcpy_s(szBuf1, "collision");
				WriteFile(hNamedPipe1, szBuf1, BUFSIZ, &cbWritten1, NULL);
				strcpy_s(szBuf2, "collision");
				WriteFile(hNamedPipe2, szBuf2, BUFSIZ, &cbWritten2, NULL);
				ReleaseSemaphore(hSemaphore1, 1, NULL);
				ReleaseSemaphore(hSemaphore2, 1, NULL);
			}
			cout << "otpravil" << endl;
		}
		cbRead1 = 0;
		cbRead2 = 0;

	}


	//WaitForSingleObject(hSemaphore1, INFINITE);
	
	



	CloseHandle(hNamedPipe1);
	CloseHandle(hNamedPipe2);
	system("pause");
	return 0;
}