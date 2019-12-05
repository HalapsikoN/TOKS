#include "Heming.h"


Heming::Heming(int bit)
{
	this->numberBits = bit;
	countWords();
}

Heming::Heming(int bit, char number)
{
	this->numberBits = bit;
	countWords();
	//cout << "check:  " << number<<" : "<<bit << endl;
	this->number = number;
	//cout << "check: " <<this->number<< endl;
}

Heming::Heming(int bit, uint16_t number)
{
	this->numberBits = bit;
	countWords();
	this->number = number;
}

void Heming::setNumber(char number)
{
	this->number = number;
}

void Heming::setNumber(uint16_t number)
{
	this->number = number;
}

char Heming::getNumber()
{
	return number.to_ulong();
}

void Heming::flipNumberAt(int i)
{
	this->number.flip(i);
}

void Heming::countWords()
{
	// найти размер слова 
	realWord = numberBits;
	/*while (number[realWord] != 1)
	{
	realWord--;
	}
	realWord++;*/
	//cout << "real:" << realWord << endl;

	//подсчёт контрольной части
	allWord = 0;
	while ((pow(2, allWord) / (allWord + 1)) < (pow(2, realWord)))
	{
		allWord++;
	}
	//cout << "all:" << allWord << endl;
	controlWord = allWord - realWord;
	//cout << "control:" << controlWord << endl;
}

void Heming::writeNumberToCode()
{
	//запись числа в код защищенный
	int k = realWord - 1;
	for (int i = 1; i <= allWord; ++i)
	{
		bool isControlSlot = false;
		for (int j = 0; j < controlWord; j++)
		{
			if (i == pow(2, j))
			{
				isControlSlot = true;
				break;
			}
		}

		if (!isControlSlot)
		{
			result[i] = number[k--];
		}
		//cout << "TYT:" << result << endl;
	}
	cout <<"TYT:"<< result << endl;
}

void Heming::generateControlBits()
{
	//формирование контрольных битов
	for (int j = 0; j < controlWord; ++j)
	{
		int controlSlot = pow(2, j);
		//cout << controlSlot<<endl;
		int controlBitValue = 0;
		bitset<8> check;
		int numberOfOne = 0;
		for (int i = controlSlot; i <= allWord; i++)
		{
			check = i;
			if (check[j] == 1)
			{
				//cout << i << " ";
				numberOfOne += (result[i] == 1) ? 1 : 0;

			}
		}
		//cout << endl;
		result[controlSlot] = (numberOfOne % 2 == 0) ? 0 : 1;
	}
	cout << "Before sending: "<<result << endl;
}

uint16_t Heming::code()
{
	writeNumberToCode();
	generateControlBits();
	return result.to_ulong();
}

void Heming::generateControlBitsToCheck()
{
	result = number;
	//формирование контрольных битов
	for (int j = 0; j < controlWord; ++j)
	{
		int nControlSlot = pow(2, j);
		//cout << nControlSlot<<endl;
		int nControlBitValue = 0;
		bitset<8> check;
		int numberOfOne = 0;
		for (int i = nControlSlot + 1; i <= allWord; i++)
		{
			check = i;
			if (check[j] == 1)
			{
				//cout << i << " ";
				numberOfOne += (result[i] == 1) ? 1 : 0;

			}
		}
		//cout << endl;
		result[nControlSlot] = (numberOfOne % 2 == 0) ? 0 : 1;
	}
	cout << "After sending: "<< result << endl;
}

void Heming::checkForMistakes()
{
	//подсчёт несовпадений
	int numberOfDiffrentBits = 0;
	int positionMistake = 0;
	for (int j = 0; j < controlWord; ++j)
	{
		int nControlSlot = pow(2, j);
		//numberOfDiffrentBits += (nResult[nControlSlot] != nNumber[nControlSlot]) ? 1: 0;
		if (result[nControlSlot] != number[nControlSlot])
		{
			numberOfDiffrentBits++;
			positionMistake += nControlSlot;
		}
	}

	switch (numberOfDiffrentBits)
	{
	case 0:
	{
		cout << "No mistakes" << endl;
		break;
	}
	case 1:
	{
		cout << "No mistakes in real number(only in control word)" << endl;
		break;
	}
	default:
	{
		cout << "There is a mistake at " << positionMistake << " position " << endl;
		number.flip(positionMistake);
		cout << "Corrected number: " << number << endl;
		break;
	}
	}
}

void Heming::readNumberFromCode()
{
	result.reset();
	int nk = realWord - 1;
	for (int i = 1; i <= allWord; ++i)
	{
		bool isControlSlot = false;
		for (int j = 0; j < controlWord; j++)
		{
			if (i == pow(2, j))
			{
				isControlSlot = true;
				break;
			}
		}

		if (!isControlSlot)
		{
			result[nk--] = number[i];
		}
	}
	//cout << result << endl;
	//cout << result.to_ulong() << endl;
}

char Heming::decode()
{
	countWords();
	generateControlBitsToCheck();
	checkForMistakes();
	readNumberFromCode();
	return result.to_ulong();
}