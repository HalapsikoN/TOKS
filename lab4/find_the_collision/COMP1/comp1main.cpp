#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <ctime>

int delay(int min, int max)
{
	srand(time(NULL));
	return (min+rand()%(max-min+1));
}

using namespace std;

int main() 
{
	cout << "Station 1 is working" << endl;
	LPSTR  SemaphoreName1 = "Semaphore1";
	HANDLE hSemaphore1 = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName1);
	if (hSemaphore1 == NULL)
	{
		MessageBox(NULL, "Open sem1 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	cout << "Ready to connect the pipe..." << endl;
	//остановка процесса для открытия канала
	WaitForSingleObject(hSemaphore1, INFINITE);

	HANDLE hNamedPipe1;		// Идентификатор канала Pipe
	DWORD  cbWritten;		// Количество байт, переданных через канал
	DWORD  cbRead=0;			// Количество байт, принятых через канал
	char   szBuf[BUFSIZ];		// Буфер для передачи данных
	char   szPipeName[BUFSIZ];	// Буфер для имени канала Pipe
	LPSTR  lpszPipeName = "\\\\.\\pipe\\Pipe1";
	
	hNamedPipe1 = CreateFile(
		lpszPipeName,
		GENERIC_ALL,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe1 == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Open pipe1 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfuly conected the pipe1." << endl;
	}
	//запуск
	ReleaseSemaphore(hSemaphore1, 1, NULL);

	cout << "PRESS KEY TO START THE STATION 1!" << endl;
	char* startMessage = "MESSAGE FROM FIRST STATION";
	strcpy_s(szBuf, startMessage);
	rewind(stdin);
	_getch();

	for (int attempt = 0; attempt < 10; attempt++)
	{
		cout << endl;
		Sleep(1);
		strcpy_s(szBuf, startMessage);
		int timeDelay = delay(100, 100 * pow(2, attempt));
		cout << timeDelay<<endl;
		Sleep(timeDelay);
		WriteFile(hNamedPipe1, szBuf, BUFSIZ, &cbWritten, NULL);
		WaitForSingleObject(hSemaphore1, INFINITE);
		while (cbRead == 0)
		{
			ReadFile(hNamedPipe1, szBuf, BUFSIZ, &cbRead, NULL);
		}
		if (strcmp(szBuf, "collision") == 0)
		{
			cout << "collision" << endl;
			cbRead = 0;
			continue;
		}
		else
		{
			cbRead = 0;
			if (strcmp(startMessage, szBuf) == 0)
			{
				cout << "Our message is sended. Wait for another message:"<<endl;
				while (cbRead == 0)
				{
					ReadFile(hNamedPipe1, szBuf, BUFSIZ, &cbRead, NULL);
				}
				cout << szBuf << endl;
				system("pause");
				return 0;
			}else
			{
				cout << "Find a message:" << endl;
				cout << szBuf << endl;
				ReadFile(hNamedPipe1, szBuf, BUFSIZ, &cbRead, NULL);
				cbRead = 0;
				if (strcmp(startMessage, szBuf) == 0)
				{
					cout << "Our message is sended." << endl;
					system("pause");
					return 0;
				}
				attempt--;
				continue;
			}
		}
		cbRead = 0;
	}
	cout << endl << "All attempts were used. Can't send the message." << endl;
	system("pause");
	return 0;
}