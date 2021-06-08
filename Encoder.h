#pragma once
#include<iostream>
#include<cstring>
#include<cmath>
#include<fstream>
#include<map>
#include<algorithm>
#include<queue>
#include "HuffmanNode.h"

using namespace std;

class Encoder
{
private:
	const char* input, *out_table, *out_code; // input_data, huffman_table, huffman_code파일에 관한 경로를 저장하는 변수
	priority_queue<pair<double, HuffmanNode*>, vector<pair<double, HuffmanNode*>>, greater<pair<double, HuffmanNode*>>> pq; // huffman tree를 생성하기 위한 prioity queue 변수
	map<char, string> code_table; // table을 저장하기 위한 변수
	int total_char_count = 0; // 전체 문자 개수를 저장하는 변수

	HuffmanNode* Root; // huffman tree의 root를 저장하기 위한 변수

public:
	Encoder(const char* input_text, const char* table_url, const char* code_url) {
		input = input_text; out_table = table_url; out_code = code_url; // 경로 저장
		CreateFrequencyTable(); 
		CreateTree();
	}
	~Encoder() {};

	string DecimalToString(int num); // 10진수를 8-bit 2진수로 변환하는 함수
	int StringToDecimal(string binary); // 8-bit 2진수를 10진수로 변환하는 함수

	void CreateFrequencyTable(); // input_data.txt 파일을 읽고 해당 정보를 priority queue에 저장하는 함수 
	void CreateTree(); // huffman tree 를 생성하는 함수
	void CreateCodeTable(string code, HuffmanNode* root); // huffman tree를 pre-order traversal을 사용해 code를 생성하는 함수

	void encoder(); // input_data.txt 파일을 읽고 huffman encoding 하는 변수
};

