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
	const char* table, * output; // huffman_table.hbs 경로와 output.txt의 경로를 저장하는 변수
	map<string, char> code_table; // code_table을 저장하는 변수

public:
	Decoder(const char* table_hbs, const char* output_url) {
		table = table_hbs; output = output_url; 
		DecodeTable(); // table을 Decode해 table로 변환하는 함수 
	}
	~Decoder() {};
	
	string DecimalToString(int num); // 10진수를 8-bit 2진수로 변환하는 함수 
	int StringToDecimal(string binary); // 8-bit 2진수를 10진수로 변환하는 함수 

	int Check_Code_Table(string code); // huffman code 가 있는지 확인하는 함수

	void DecodeTable(); // huffman_table.hbs를 통해 decode하는 함수
	void decoder(); // table을 사용해 huffman_code.hbs를 변환하고 output파일로 저장하는 함수 
};

