#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

int main()
{
	bitset<16> number;
	char a;
	//cin >> a;
	a = '[';
	number = a;
	cout << number <<endl;
	
	// найти размер слова 
	int realWord = 8;
	/*while (number[realWord] != 1)
	{
		realWord--;
	}
	realWord++;*/
	cout << "real:"<<realWord << endl;
	
	//подсчёт контрольной части
	int allWord = 0;
	while ((pow(2, allWord) / (allWord + 1)) < (pow(2, realWord)))
	{
		allWord++;
	}
	cout << "all:" << allWord << endl;
	int controlWord = allWord - realWord;
	cout << "control:" << controlWord << endl;

	//запись числа в код защищенный
	bitset<16> result;
	int k = realWord-1;
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
	}
	cout << result << endl;

	//формирование контрольных битов
	for (int j = 0; j < controlWord; ++j)
	{
		int controlSlot = pow(2, j);
		//cout << controlSlot<<endl;
		int controlBitValue = 0;
		bitset<8> check;
		int numberOfOne = 0;
		for(int i=controlSlot; i<=allWord; i++)
		{
			check = i;
			if (check[j]==1)
			{
				//cout << i << " ";
				numberOfOne += (result[i] == 1) ? 1 : 0;
						
			}
		}
		//cout << endl;
		result[controlSlot] = (numberOfOne % 2 == 0) ? 0 : 1;
	}
	cout << result << endl;

	//--------------------------------------------------------------------------------------------------//

	cout << endl << endl << endl;

	bitset<16> nNumber = result;
	//создание ошибки
	nNumber[5] = 1;
	//
	int nRealWord = 8;
	cout << "new real:" << nRealWord << endl;
	//подсчёт контрольной части
	int nAllWord = 0;
	while ((pow(2, nAllWord) / (nAllWord + 1)) < (pow(2, nRealWord)))
	{
		nAllWord++;
	}
	cout << "new all:" << nAllWord << endl;
	int nControlWord = nAllWord - nRealWord;
	cout << "new control:" << nControlWord << endl;
	cout << nNumber << endl;
	
	bitset<16> nResult = nNumber;
	//формирование контрольных битов
	for (int j = 0; j < nControlWord; ++j)
	{
		int nControlSlot = pow(2, j);
		//cout << nControlSlot<<endl;
		int nControlBitValue = 0;
		bitset<8> check;
		int numberOfOne = 0;
		for (int i = nControlSlot+1; i <= nAllWord; i++)
		{
			check = i;
			if (check[j] == 1)
			{
				//cout << i << " ";
				numberOfOne += (nResult[i] == 1) ? 1 : 0;

			}
		}
		//cout << endl;
		nResult[nControlSlot] = (numberOfOne % 2 == 0) ? 0 : 1;
	}
	cout << nResult << endl;

	//подсчёт несовпадений
	int numberOfDiffrentBits = 0;
	int positionMistake = 0;
	for (int j = 0; j < nControlWord; ++j)
	{
		int nControlSlot = pow(2, j);
		//numberOfDiffrentBits += (nResult[nControlSlot] != nNumber[nControlSlot]) ? 1: 0;
		if (nResult[nControlSlot] != nNumber[nControlSlot])
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
		nNumber.flip(positionMistake);
		cout << "Corrected number: " << nNumber << endl;
		break;
	}
	}

	nResult.reset();
	int nk = nRealWord - 1;
	for (int i = 1; i <= nAllWord; ++i)
	{
		bool isControlSlot = false;
		for (int j = 0; j < nControlWord; j++)
		{
			if (i == pow(2, j))
			{
				isControlSlot = true;
				break;
			}
		}

		if (!isControlSlot)
		{
			nResult[nk--]=nNumber[i];
		}
	}
	char b = nResult.to_ulong();
	uint16_t ss = result.to_ulong();
	cout << nResult << endl;
	cout << b << endl;
	cout << ss << endl;
	cout << result << endl;
	bitset<16> yy = ss;
	cout << yy << endl;

	return 0;
}