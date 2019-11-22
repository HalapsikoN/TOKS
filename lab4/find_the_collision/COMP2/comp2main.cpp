#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <ctime>

int delay(int min, int max)
{
	//srand(time(NULL));
	return (min + rand() % (max - min + 1));
}

using namespace std;

int main()
{
	cout << "Station 2 is working" << endl;
	LPSTR  SemaphoreName2 = "Semaphore2";
	HANDLE hSemaphore2 = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName2);
	if (hSemaphore2 == NULL)
	{
		MessageBox(NULL, "Open sem2 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	cout << "Ready to connect the pipe..." << endl;
	//��������� �������� ��� �������� ������
	WaitForSingleObject(hSemaphore2, INFINITE);

	HANDLE hNamedPipe2;		// ������������� ������ Pipe
	DWORD  cbWritten;		// ���������� ����, ���������� ����� �����
	DWORD  cbRead=0;			// ���������� ����, �������� ����� �����
	char   szBuf[BUFSIZ];		// ����� ��� �������� ������
	char   szPipeName[BUFSIZ];	// ����� ��� ����� ������ Pipe
	LPSTR  lpszPipeName = "\\\\.\\pipe\\Pipe2";

	hNamedPipe2 = CreateFile(
		lpszPipeName,
		GENERIC_ALL,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe2 == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Open pipe2 Failed", "Error", MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfuly conected the pipe2." << endl;
	}
	//������
	ReleaseSemaphore(hSemaphore2, 1, NULL);

	cout << "PRESS KEY TO START THE STATION 2!" << endl;
	char* startMessage = "MESSAGE FROM SECOND STATION";
	strcpy_s(szBuf, startMessage);
	rewind(stdin);
	_getch();

	for (int attempt = 0; attempt < 10; attempt++)
	{
		strcpy_s(szBuf, startMessage);
		int timeDelay = delay(100, 100 * pow(2, attempt));
		cout << timeDelay << endl;
		cout << endl;
		Sleep(timeDelay);
		WriteFile(hNamedPipe2, szBuf, BUFSIZ, &cbWritten, NULL);
		WaitForSingleObject(hSemaphore2, INFINITE);
		while (cbRead == 0)
		{
			ReadFile(hNamedPipe2, szBuf, BUFSIZ, &cbRead, NULL);
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
				cout << "Our message is sended. Wait for another message:" << endl;
				while (cbRead == 0)
				{
					ReadFile(hNamedPipe2, szBuf, BUFSIZ, &cbRead, NULL);
				}
				cout << szBuf << endl;
				system("pause");
				return 0;
			}
			else
			{
				cout << "Find a message:" << endl;
				cout << szBuf << endl;
				ReadFile(hNamedPipe2, szBuf, BUFSIZ, &cbRead, NULL);
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

	system("pause");
	return 0;
}