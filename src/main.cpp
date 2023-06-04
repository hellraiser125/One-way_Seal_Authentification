#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cmath>
#include "SEAL.h"
#include <random>
#include <string>
#include <cstring>
using namespace std;



int main(int argc, char* argv[]) {
	
	setlocale(LC_ALL, "Ukrainian");
	int variant = 0;
	int BobKey[5],BobID;
	SEAL seal;
	char a[1024];
	int i = 0, L = 0, n;
	int* enc = 0;
	int* dec = 0;
	int* text = 0;
	char* decoded = new char[L / 8];;
	cout << "\nMenu:\n"
		<< "1.Key and ID generation\n"
		<< "2.Create message\n"
		<< "3.Encode message\n"
		<< "4.Decode message\n"
		<< "5.Check authorization\n"
		<< "6.Exit\n";
	do {
		cout<<"\n>>>>>>>>>  ";
		cin >> variant;
		switch (variant)
		{
		case 1:
		{

			random_device rd;
			mt19937 gen(rd() + (unsigned)time(nullptr));
			uniform_int_distribution<int> uid(0,9);
			cout << "Key: ";
			for (int i = 0; i < 5; i++) {
				BobKey[i] = uid(gen);
				
				cout<< BobKey[i];
				//cout << "str: " << to_string(BobKey[i]).c_str()<<endl;
			}
			BobID = uid(gen);
			cout << "\nID: " << BobID << endl;
			cout << "N: ";//кількість раундів???
			cin >> n;
			break;
		}
		case 2:
		{
			const int SIZE = sizeof(BobKey) / sizeof(int);
			string id = to_string(BobID);
			cout << "Message: ";
			for (i; i < SIZE; i++) {
				string str = to_string(BobKey[i]);
				//cout << "\nSTR: " << str << endl;
				strcpy(&a[i], str.c_str()); // копируем строку в новый элемент массива
				a[i + 1] = '\0'; // добавляем завершающий нулевой символ
				//cout << a << endl;
			}
			//cout << "I " << i<<endl;
			//strcpy(&a[i], id.c_str());
			//a[i + 1] = '\0';
			for (size_t j = 0; j < (i + 1); j++)
			{
				cout << a[j];
			}
			
			//cout << "\nIteration: " << 5 * (i + 1) << endl;
			//a[5*(i+1)] = '\0';
			
			L = (i + 1) * 8;//i char'ов, т.е. 8*i бит
			text = new int[ceil((float)((i + 1)) / 4)];//его кодируем, размер в 4 раза меньше

			//вот здесь упаковать чары в инт без лишних нулей
			for (int j = 0; j < ceil((float)((i + 1)) / 4); j++) {
				int tmp = 0;
				for (int k = 0; k < 4; k++) {
					int num = 4 * j + k;
					tmp = tmp << 8;
					if (num < i)//если текст ещё не кончился
						tmp += a[num];
				}
				text[j] = tmp;
			}
			break;
		}
		case 3:
		{

			enc = seal.coding(text, L, BobKey, n);
			cout << "Encode message: \n" << enc <<endl;
			break;
		}
		case 4:
		{
			dec = seal.coding(enc, L, BobKey, n);

			//bob
			decoded = new char[L / 8];
			int d = 0;//индекс по decoded
			for (int i = 0; i < ceil((float)L / 32); i++) {
				for (int s = 3; s >= 0; s--) {
					decoded[d++] = dec[i] >> (8 * s);
				}
			}

			cout << "\nDecoded message:" << endl;
			for (int j = 0; j < L / 8; j++)
				cout << decoded[j];
			cout << endl;
			break;
		}
		case 5:
		{	
			cout << "Try with invalid key?\n1.Yes\n2.No" << endl;
			int v = 0;
			cin >> v;
			string checkKey = "";
			for (int i = 0; i < sizeof(BobKey) / sizeof(int); i++) {
				checkKey += to_string(BobKey[i]);
			}
			if (v == 1) {
				cout << "Key: 12345" << endl;
				if (strcmp(decoded, "12345") != 0) {
					cout << "Invalid key!Authorization failed" << endl;
					break;
				}
			}
			else if (v == 2) {
				if (strcmp(decoded, checkKey.c_str()) != 0) {
					cout << "Invalid key!Authorization failed" << endl;
				}
				cout << "Authorization completed" << endl;
			}
			break;
		}
		case 6:
		{
			EXIT_SUCCESS;
		}
		default:
			break;
		}
	} while (variant != 6);

	return 0;
}