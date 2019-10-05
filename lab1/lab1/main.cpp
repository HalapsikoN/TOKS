#include <stdio.h>
#include <stdlib.h>
#include "COM.h"
#include <thread>

#define COM_1_PORT_NAME "\\\\.\\COM1"
#define COM_2_PORT_NAME "\\\\.\\COM2"
//#define BAUD_RATE 9600

using namespace std;

int work = 1;
int firstWrite = 0, firstRead = 0, firstChangeBaydrate = 0;
int secondWrite = 0, secondRead = 0, secondChangeBaydrate = 0;
LPSTR  SemaphoreName1 = "Semaphore1";
LPSTR  SemaphoreName2 = "Semaphore2";

void portCOM1()
{
	HANDLE hSemaphore = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName1);

	if (hSemaphore == NULL)
	{
		std::cout << "CreateSemaphore error: " << GetLastError() << std::endl;
	}

	
	COM firstPort;
	firstPort.Init(COM_1_PORT_NAME, CBR_9600);
	while (work)
	{
		char str[200];
		WaitForSingleObject(hSemaphore, INFINITE);
		if (firstWrite)
		{
			cout << "writeCOM1" << endl;
			rewind(stdin);
			cin.getline(str, 200);
			firstPort.WriteStr((uint8_t*)& str);
			firstWrite = 0;
		}
		if (firstRead)
		{
			cout << "readCOM1" << endl;
			firstPort.ReadStr((uint8_t*)& str);
			cout << str << endl;
			firstRead = 0;
		}
		if (firstChangeBaydrate)
		{
			cout << "changeCOM1" << endl;
			firstPort.Disconnect();
			int newRate;
			cin >> newRate;
			newRate = (newRate < 0) ? (newRate*(-1)) : newRate;
			firstPort.Init(COM_1_PORT_NAME, newRate);
			firstChangeBaydrate = 0;
		}
		ReleaseSemaphore(hSemaphore, 1, NULL);
	}
}

void portCOM2()
{
	HANDLE hSemaphore = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		SemaphoreName2);

	if (hSemaphore == NULL)
	{
		std::cout << "CreateSemaphore error: " << GetLastError() << std::endl;
	}


	char str[200];
	COM secondPort;
	secondPort.Init(COM_2_PORT_NAME, CBR_9600);
	while (work)
	{
		char str[200];
		WaitForSingleObject(hSemaphore, INFINITE);
		if (secondWrite)
		{
			cout << "writeCOM2" << endl;
			rewind(stdin);
			cin.getline(str, 200);
			secondPort.WriteStr((uint8_t*)& str);
			secondWrite = 0;
		}
		if (secondRead)
		{
			cout << "readCOM2" << endl;
			secondPort.ReadStr((uint8_t*)& str);
			cout << str << endl;
			secondRead = 0;
		}
		if (secondChangeBaydrate)
		{
			cout << "changeCOM2" << endl;
			secondPort.Disconnect();
			int newRate;
			cin >> newRate;
			newRate = (newRate < 0) ? (newRate*(-1)) : newRate;
			secondPort.Init(COM_2_PORT_NAME, newRate);
			secondChangeBaydrate = 0;
		}
		ReleaseSemaphore(hSemaphore, 1, NULL);

	}
}

int main()
{	//==============================================================================//
	//								СОЗДАНИЕ СЕМАФОРОВ								//
	//==============================================================================//
	HANDLE hSemaphore1 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName1	// имя семафора
	);
	if (hSemaphore1 == NULL)
	{
		std::cout << "Create Semaphore Failed & Error NO - " << GetLastError() << std::endl;
		//_getch();
		return 0;
	}
	/*else
	{
		std::cout << "Create Semaphore Success" << std::endl;
	}*/


	HANDLE hSemaphore2 = CreateSemaphore(
		NULL,				// нет атрибута
		0,					// начальное состояние
		1,					// максимальное состояние
		SemaphoreName2	// имя семафора
	);
	if (hSemaphore2 == NULL)
	{
		std::cout << "Create Semaphore Failed & Error NO - " << GetLastError() << std::endl;
		//_getch();
		return 0;
	}
	/*else
	{
		std::cout << "Create Semaphore Success" << std::endl;
	}*/
	//==============================================================================//


	thread thread1(portCOM1);
	thread thread2(portCOM2);
	//char str[200];

	int choice;
	while (work)
	{
		do
		{
			rewind(stdin);
			cout << endl << "1) write from first to second" << endl;
			cout << "2) write from second to first" << endl;
			cout << "3) change first baudrate" << endl;
			cout << "4) change second baudrate" << endl;
			cout << "0) exit" << endl << endl;
		} while (!(scanf_s("%d", &choice)) || choice > 4 || choice < 0);

		switch (choice)
		{
		case 1:
		{
			firstWrite = 1;
			secondRead = 1;
			ReleaseSemaphore(hSemaphore1, 1, NULL);
			WaitForSingleObject(hSemaphore1, INFINITE);
			ReleaseSemaphore(hSemaphore2, 1, NULL);
			WaitForSingleObject(hSemaphore2, INFINITE);
			break;
		}
		case 2:
		{
			secondWrite = 1;
			firstRead = 1;
			ReleaseSemaphore(hSemaphore2, 1, NULL);
			WaitForSingleObject(hSemaphore2, INFINITE);
			ReleaseSemaphore(hSemaphore1, 1, NULL);
			WaitForSingleObject(hSemaphore1, INFINITE);
			break;
		}
		case 3:
		{
			firstChangeBaydrate = 1;
			ReleaseSemaphore(hSemaphore1, 1, NULL);
			WaitForSingleObject(hSemaphore1, INFINITE);
			break;
		}
		case 4:
		{
			secondChangeBaydrate = 1;
			ReleaseSemaphore(hSemaphore2, 1, NULL);
			WaitForSingleObject(hSemaphore2, INFINITE);
			break;
		}
		case 0:
		{
			work = 0;
			ReleaseSemaphore(hSemaphore1, 1, NULL);
			ReleaseSemaphore(hSemaphore2, 1, NULL);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	thread1.join();
	thread2.join();
	CloseHandle(hSemaphore1);
	CloseHandle(hSemaphore2);
	return 0;
}