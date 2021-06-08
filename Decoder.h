#pragma once
#include<iostream>
#include<cstring>
#include<cmath>
#include<fstream>
#include<map>
using namespace std;

class Decoder
{
private:
	const char* table, * output; // huffman_table.hbs ��ο� output.txt�� ��θ� �����ϴ� ����
	map<string, char> code_table; // code_table�� �����ϴ� ����

public:
	Decoder(const char* table_hbs, const char* output_url) {
		table = table_hbs; output = output_url; 
		DecodeTable(); // table�� Decode�� table�� ��ȯ�ϴ� �Լ� 
	}
	~Decoder() {};
	
	string DecimalToString(int num); // 10������ 8-bit 2������ ��ȯ�ϴ� �Լ� 
	int StringToDecimal(string binary); // 8-bit 2������ 10������ ��ȯ�ϴ� �Լ� 

	int Check_Code_Table(string code); // huffman code �� �ִ��� Ȯ���ϴ� �Լ�

	void DecodeTable(); // huffman_table.hbs�� ���� decode�ϴ� �Լ�
	void decoder(); // table�� ����� huffman_code.hbs�� ��ȯ�ϰ� output���Ϸ� �����ϴ� �Լ� 
};

