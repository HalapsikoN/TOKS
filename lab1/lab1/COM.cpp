#include "COM.h"
#include "Exception.h"

#define TIMEOUT 100

void COM::Init(const string& port, int baudrate)
{
	//открытие порта
	m_Handle = CreateFile(
		port.c_str(), //наименование порта, например COM1
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
	//устанавливаем размеры входного и выходного буферов
	SetupComm(m_Handle, 1500, 1500);
	//настраиваем тайм-ауты для текущего устройства
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
	//настраиваем параметры работы для текущего устройства
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(m_Handle, &ComDCM); // текущие значения
	ComDCM.BaudRate = DWORD(baudrate); // скорость в бодах
	ComDCM.ByteSize = 8; // размер байта
	ComDCM.Parity = NOPARITY; // паритет
	ComDCM.StopBits = ONESTOPBIT; // количество стоп бит
	ComDCM.fAbortOnError = TRUE;
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // сброс DTR бита
											  // автоустановка RTS бита
	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;
	ComDCM.fBinary = TRUE; //бинарный режим всегда
	ComDCM.fParity = FALSE; //паритет
	ComDCM.fInX = ComDCM.fOutX = FALSE;
	ComDCM.XonChar = 0;
	ComDCM.XoffChar = uint8_t(0xff);
	ComDCM.fErrorChar = FALSE;
	ComDCM.fNull = FALSE;
	ComDCM.fOutxCtsFlow = FALSE;
	ComDCM.fOutxDsrFlow = FALSE;
	ComDCM.XonLim = 128;
	ComDCM.XoffLim = 128;
	//установка параметров работы для текущего устройства
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
	int attempts = 5; //количество запросов на чтение
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