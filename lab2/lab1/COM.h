#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

class COM
{
public:
	void Init(const string&, int);
	void Disconnect();
	void Write(const uint8_t*, int);
	void WriteStr(uint8_t*);
	void Read(uint8_t*, int);
	void ReadStr(uint8_t*);
	void WritePackage(const uint8_t*);
	void ReadPackage(uint8_t*);
private:
	void paste(uint8_t*, char, int, int&);
	void cut(uint8_t*, int, int&);
	HANDLE m_Handle;
	const uint8_t flag=0x7E;
	const uint8_t changeSymbol = 0x7D;
	const uint8_t checkChange = 0x5E;
	const uint8_t checkUnchange = 0x5D;
};