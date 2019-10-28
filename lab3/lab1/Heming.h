#pragma once
#include <iostream>
#include <bitset>

using namespace std;

class Heming
{
public:
	Heming(int bit);
	Heming(int bit, char number);
	Heming(int bit, uint16_t number);
	void setNumber(char number);
	void setNumber(uint16_t number);
	char getNumber();
	void flipNumberAt(int);
	uint16_t code();
	char decode();
private:
	int numberBits;
	bitset<16> number;
	bitset<16> result;
	int realWord;
	int allWord;
	int controlWord;
	void countWords();
	void writeNumberToCode();
	void generateControlBits();
	void generateControlBitsToCheck();
	void checkForMistakes();
	void readNumberFromCode();
};