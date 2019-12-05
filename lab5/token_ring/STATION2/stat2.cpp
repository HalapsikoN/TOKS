#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <bitset>
#include <ctime>

using namespace std;

int countNumberOfWords(string str)
{
	int counter = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if ((str[i] != ' ' && str[i + 1] == ' ') || (str[i] != ' ' && str[i + 1] == '\0'))
		{
			counter++;
		}
	}
	return counter;
}

string getString(string str, int number)
{
	string result = "";
	if (number > countNumberOfWords(str))
	{
		return result;
	}

	for (int i = 0; i < str.length(); i++)
	{
		if ((str[i] != ' ' && i == 0) || (str[i] == ' ' && str[i + 1] != ' '))
		{
			if (i != 0)
			{
				i++;
			}
			number--;
			if (number == 0)
			{
				while (str[i] != ' ' && str[i] != '\0')
				{
					result += str[i++];
				}
			}
		}
	}

	return result;
}

string getAllPackage(string str, string delimeter)
{
	string result = "";
	int counter = 2;
	int i = 0;
	while (counter > 0)
	{
		if (str[i] == delimeter[0])
		{
			counter--;
		}
		result += str[i++];
	}
	return result;
}

string getFS(string str, string delimeter)
{
	string result = "";
	int counter = 2;
	int i = 0;
	while (counter > 0)
	{
		if (str[i++] == delimeter[0])
		{
			counter--;
		}
	}
	while (str[i] == ' ')
	{
		i++;
	}
	while (str[i] != ' '&& str[i] != '\0')
	{
		result += str[i++];
	}
	return result;
}

string getINFO(string str, string delimeter)
{
	string result = "";
	int counter = 4;
	int i = 0;
	while (counter > 0)
	{
		if (str[i++] == ' ')
		{
			counter--;
		}
	}
	while (str[i] != delimeter[0])
	{
		result += str[i++];
	}
	return result;
}

int main()
{
	cout << "STATION2 IS WORKING" << endl;
	//==============================================================================//
	//								����������� �������								//
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
	WaitForSingleObject(hSemaphore1, INFINITE);
		
	HANDLE hNamedPipe1;		// ������������� ������ Pipe
	DWORD  cbWritten1;		// ���������� ����, ���������� ����� �����
	DWORD  cbRead1 = 0;			// ���������� ����, �������� ����� �����
	char   szBuf1[BUFSIZ];		// ����� ��� �������� ������
	char   szPipeName1[BUFSIZ];	// ����� ��� ����� ������ Pipe
	LPSTR  lpszPipeName1 = "\\\\.\\pipe\\Pipe1";

	hNamedPipe1 = CreateFile(
		lpszPipeName1,
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
	//==============================================================================//
	//								�������� ������									//
	//==============================================================================//

	BOOL   fConnected2;									// ���� ��������� �������� ������
	HANDLE hNamedPipe2;									// ������������� ������ Pipe
	LPSTR  lpszPipeName2 = "\\\\.\\pipe\\Pipe2";			// ��� ������������ ������ Pipe
	char   szBuf2[BUFSIZ];								// ����� ��� �������� ������ ����� �����
	DWORD  cbWritten2;									// ���������� ���� ������, ���������� ����� �����
	DWORD  cbRead2 = 0;

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

	ReleaseSemaphore(hSemaphore2, 1, NULL);
	//������
	cout << "start" << endl;

	string EMPTY_TOKEN = "~ 0 ~";
	string DELIMETER = "~";
	string SPACE = " ";
	string AC = "1";
	string DA = "";
	string SA = "2";
	string MESSAGE = "MESSAGE FROM 2 STATION TO ";
	string FS = "";

	boolean wantToSend = false;

	while (TRUE)
	{
		ReadFile(hNamedPipe1, szBuf1, BUFSIZ, &cbRead1, NULL);
		cout << endl << "READ: " << szBuf1 << endl;

		if (!wantToSend)
		{
			srand(time(NULL));
			int num = rand() % 100;
			//cout << num<<endl;
			wantToSend = ((num>80) ? true : false);
			//cout << wantToSend << endl;
		}

		string str = (string)szBuf1;
		string toSend = "";

		if (getString(str, 2) == "0")
		{
			if (wantToSend)
			{
				//cout << "state 1" << endl;

				toSend += DELIMETER + SPACE;
				toSend += AC + SPACE;
				srand(time(NULL));
				DA = (rand() % 100 > 50) ? "1" : "3";
				toSend += DA + SPACE;
				toSend += SA + SPACE;
				toSend += MESSAGE + DA + SPACE;
				toSend += DELIMETER + SPACE;
				bitset<6> temp = 0;
				toSend += temp.to_string();
			}
			else
			{
				//cout << "state 2" << endl;

				toSend = EMPTY_TOKEN;
			}
		}
		else
		{
			if (getString(str, 3) == SA)
			{
				//cout << "state 3" << endl;

				string info = getINFO(str, DELIMETER);
				cout << "GOT MESSAGE   :   " << info << endl;
				toSend += getAllPackage(str, DELIMETER) + SPACE;
				FS = getFS(str, DELIMETER);
				bitset<6> temp(FS);
				temp[5] = 1; temp[4] = 1; temp[2] = 1; temp[1] = 1;
				toSend += temp.to_string();
			}
			else
			{
				if (getString(str, 4) == SA)
				{
					//cout << "state 4" << endl;

					FS = getFS(str, DELIMETER);
					bitset<6> temp(FS);
					if (temp.to_ulong() == 54)
					{
						//cout << "state 5" << endl;
						wantToSend = false;
						toSend = EMPTY_TOKEN;
					}
					else
					{
						//cout << "state 6" << endl;

						toSend += DELIMETER + SPACE;
						toSend += AC + SPACE;
						toSend += DA + SPACE;
						toSend += SA + SPACE;
						toSend += MESSAGE + DA + SPACE;
						toSend += DELIMETER + SPACE;
						bitset<6> temp = 0;
						toSend += temp.to_string();
					}
				}
				else
				{
					//cout << "state 7" << endl;

					toSend = str;
				}
			}
		}

		cout << "TOSEND:  " << toSend << endl;

		strcpy_s(szBuf2, toSend.c_str());

		Sleep(1000);
		WriteFile(hNamedPipe2, szBuf2, BUFSIZ, &cbWritten2, NULL);
	}

	CloseHandle(hNamedPipe2);
	system("pause");
	return 0;
}