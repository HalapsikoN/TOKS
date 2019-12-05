#include <iostream>
#include <bitset>
#include <string>

using namespace std;

int countSymbol(string str, char c)
{
	int counter = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == c)
		{
			counter++;
		}
	}
	return counter;
}

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
		if ((str[i] != ' ' && i==0) || (str[i] == ' ' && str[i + 1] != ' '))
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

int main()
{
	bitset<4> a;
	a = 3;

	string str="";

	str += "ddd sadad sada asdasdsa dasdsadsad asdasdsad";
	//str = a.to_string();

	cout << countSymbol(str, ' ')<<endl;
	cout << countNumberOfWords(str) << endl;
	cout << getString(str, 3) << endl;

	bitset<6> temp=0;
	temp[5] = 1; temp[4] = 1; temp[2] = 1; temp[1] = 1;
	cout << a << endl;
	cout << "string: " << str << endl;
	//cout << b.to_ulong() << endl;

	cout << getString(str, 1) << endl;

	return 0;
}