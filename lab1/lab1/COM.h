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
private:
	HANDLE m_Handle;

};