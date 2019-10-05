#include "COM.h"
#include "Exception.h"

#define TIMEOUT 100

void COM::Init(const string& port, int baudrate)
{
	//�������� �����
	m_Handle = CreateFile(
		port.c_str(), //������������ �����, �������� COM1
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_Handle == (HANDLE)-1) {
		m_Handle = 0;
		throw Exception();
	}
	//������������� ������� �������� � ��������� �������
	SetupComm(m_Handle, 1500, 1500);
	//����������� ����-���� ��� �������� ����������
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
	if (!SetCommTimeouts(m_Handle, &CommTimeOuts)) {
		m_Handle = 0;
		throw Exception();
	}
	//����������� ��������� ������ ��� �������� ����������
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(m_Handle, &ComDCM); // ������� ��������
	ComDCM.BaudRate = DWORD(baudrate); // �������� � �����
	ComDCM.ByteSize = 8; // ������ �����
	ComDCM.Parity = NOPARITY; // �������
	ComDCM.StopBits = ONESTOPBIT; // ���������� ���� ���
	ComDCM.fAbortOnError = TRUE;
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // ����� DTR ����
											  // ������������� RTS ����
	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;
	ComDCM.fBinary = TRUE; //�������� ����� ������
	ComDCM.fParity = FALSE; //�������
	ComDCM.fInX = ComDCM.fOutX = FALSE;
	ComDCM.XonChar = 0;
	ComDCM.XoffChar = uint8_t(0xff);
	ComDCM.fErrorChar = FALSE;
	ComDCM.fNull = FALSE;
	ComDCM.fOutxCtsFlow = FALSE;
	ComDCM.fOutxDsrFlow = FALSE;
	ComDCM.XonLim = 128;
	ComDCM.XoffLim = 128;
	//��������� ���������� ������ ��� �������� ����������
	if (!SetCommState(m_Handle, &ComDCM)) {
		CloseHandle(m_Handle);
		m_Handle = 0;
		throw Exception();
	}
}

void COM::Disconnect()
{
	if (m_Handle != 0) {
		CloseHandle(m_Handle);
		m_Handle = 0;
	}
}

void COM::Write(const uint8_t* data, int datasize)
{
	if (m_Handle == 0) { throw Exception(); }
	DWORD feedback;
	if (!WriteFile(m_Handle, data, datasize, &feedback, 0) ||
		feedback != datasize) {
		CloseHandle(m_Handle);
		m_Handle = 0;
		throw Exception("write");
	}
	//std::cout << datasize << " data:  " << data << " feedback:  " << feedback << std::endl;
}

void COM::WriteStr(uint8_t* data)
{
	int i;
	for (i = 0; *(data + i) != '\0'; ++i)
	{
		//std::cout << *(data + i) << std::endl;
		Write((data + i), 1);
	}
	Write((data + i), 1);
}

void COM::Read(uint8_t* data, int bufsize) {
	DWORD feedback = 0;
	int attempts = 5; //���������� �������� �� ������
	while (bufsize && attempts) {
		if (attempts) attempts--;
		if (!ReadFile(m_Handle, data, bufsize, &feedback, NULL))
		{
			CloseHandle(m_Handle);
			m_Handle = 0;
			throw Exception("read");
		}
		bufsize -= feedback;
		data += feedback;
		//std::cout << bufsize << "  data: " << data << " feedback:  "<<feedback << std::endl;
	}
	if (attempts == 0)
	{
		throw Exception("read2");
	}
}

void COM::ReadStr(uint8_t* data) {
	int i;
	uint8_t temp='a';
	for (i = 0; temp != '\0'; ++i)
	{
		Read(&temp, 1);
		//std::cout << (temp) << std::endl;
		*(data + i) = temp;
	}
	*(data + i) = '\0';
	//cout << data << endl;
}