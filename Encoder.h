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
	const char* input, *out_table, *out_code; // input_data, huffman_table, huffman_code���Ͽ� ���� ��θ� �����ϴ� ����
	priority_queue<pair<double, HuffmanNode*>, vector<pair<double, HuffmanNode*>>, greater<pair<double, HuffmanNode*>>> pq; // huffman tree�� �����ϱ� ���� prioity queue ����
	map<char, string> code_table; // table�� �����ϱ� ���� ����
	int total_char_count = 0; // ��ü ���� ������ �����ϴ� ����

	HuffmanNode* Root; // huffman tree�� root�� �����ϱ� ���� ����

public:
	Encoder(const char* input_text, const char* table_url, const char* code_url) {
		input = input_text; out_table = table_url; out_code = code_url; // ��� ����
		CreateFrequencyTable(); 
		CreateTree();
	}
	~Encoder() {};

	string DecimalToString(int num); // 10������ 8-bit 2������ ��ȯ�ϴ� �Լ�
	int StringToDecimal(string binary); // 8-bit 2������ 10������ ��ȯ�ϴ� �Լ�

	void CreateFrequencyTable(); // input_data.txt ������ �а� �ش� ������ priority queue�� �����ϴ� �Լ� 
	void CreateTree(); // huffman tree �� �����ϴ� �Լ�
	void CreateCodeTable(string code, HuffmanNode* root); // huffman tree�� pre-order traversal�� ����� code�� �����ϴ� �Լ�

	void encoder(); // input_data.txt ������ �а� huffman encoding �ϴ� ����
};

