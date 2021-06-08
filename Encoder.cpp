#include "Encoder.h"

string Encoder::DecimalToString(int num)
{
	string binary = "";
	while (num != 0) { // num이 0일 때까지 반복
		binary += (num % 2 == 0 ? "0" : "1"); // 2로 나누었을 때 나머지가 0이면 0을 0이 아니면 1을 더함
		num /= 2; // num에 2로 나눈 몫을 저장함
	}
	if (binary.size() < 8) {
		// 8개의 bit를 맞추기 위해 zero extension을 함
		int rest_bit = 8 - binary.size(); 
		for (int i = 0; i < rest_bit; i++) {
			binary += "0";
		}
	}
	reverse(binary.begin(), binary.end()); // 결과를 뒤집는다.
	return binary; // 결과를 반환함
}

int Encoder::StringToDecimal(string binary)
{
	int num = 0; // num에 0을 저장
	for (int i = 0; i < binary.size(); i++)
		if (binary.c_str()[i] == '1') { // 해당 위치의 값이 1이면 
			num += pow(2, binary.size() - i - 1); // 해당 자릿수에 맞는 지수 값을 더함
		}
	return num; // 10진 변환 결과를 반환함
}

void Encoder::CreateFrequencyTable() {
	int s;
	int frequent[128] = { 0, }; // 각 변수들의 빈도를 저장하는 배열 변수를 저장하고 0으로 초기화함
	FILE* fin; // FIle 포인터 선언
	fopen_s(&fin, input, "r"); // 파일을 읽기 모드로 open
	if (fin == NULL) { // 파일을 열지 못하면 그냥 return 함
		cout << "Input File is not EXIST!!" << endl;
		return;
	}
	while ((s = fgetc(fin)) != EOF) { // 파일의 끝까지 1개씩 읽어옴
		++frequent[s]; // frequency에 s에 저정된 값의 인덱스의 값을 1증가
		total_char_count++; // total_char_count 변수를 1증가
	}
	fclose(fin); // fin 파일을 닫음

	/* 각 파일에서 문자가 나온 수를 queue에 저장하는 code */
	HuffmanNode* node = new HuffmanNode; // HuffmanNode를 동적할당
	node->SetSymbol(26); // symbol 26 (EOD)를 설정
	pq.push({ 1.0 / (double)total_char_count, node}); // 확률과 node를 priority queue에 push
	for (int i = 0; i < 128; i++) {
		if (frequent[i] != 0) { // frequency가 0이 아닌 경우 
			HuffmanNode* node = new HuffmanNode; // 새로운 node를 동적할당함
			node->SetSymbol((char)i); // symbol을 저장
			pq.push({ (double)frequent[i] / (double)total_char_count, node }); // priority queue에 push
		}
	}
}

void Encoder::CreateTree() {
	while (pq.size() != 1) { // priority queue의 size가 1이 아닐때까지 반복
		// 가장 위에 있는 원소 2개를 GET하고 POP함
		pair<double, HuffmanNode*> operand1 = pq.top(); 
		pq.pop();
		pair<double, HuffmanNode*> operand2 = pq.top();
		pq.pop();

		// 새로운 node를 동적할당하여 위의 2개 원소를 새로운 node의 child들로 Setting
		HuffmanNode* node = new HuffmanNode;
		node->SetLeft(operand1.second);
		node->SetRight(operand2.second);

		// 2개의 원소의 parent를 setting
		operand1.second->SetParent(node);
		operand2.second->SetParent(node);
		
		// priority queue에 2개의 확률을 더한뒤 새로운 node를 priority queue에 push
		pq.push({ operand1.first + operand2.first, node });
	}
	// 최종적으로 Root 변수에 tree 저장
	pair<double, HuffmanNode*> Top = pq.top();
	pq.pop();
	Root = Top.second;
	CreateCodeTable("", Root); // huffman tree를 사용해 code 생성
}

void Encoder::CreateCodeTable(string code, HuffmanNode* root) {
	if (root != nullptr) {  // if root가 null이 아니면 
		if (root->GetSymbol() != NULL) { // root의 symbol이 null이 아니면
			code_table.insert({ root->GetSymbol(), code }); // code_table에 symbol과 code insert
		}
		if (root->GetLeft() != NULL) { // if 왼쪽 자식이 있으면 
			code += "0"; // code문자열의 맨 뒤에 0을 추가함
			CreateCodeTable(code, root->GetLeft()); // 왼쪽 node로 이동
		}
		if (root->Getright() != NULL) { // if 오른쪽 자식이 있으면 
			code = code.erase(code.size() - 1, code.size()); // code의 맨 뒤 문자 삭제
			code += "1"; // code의 제일 뒤에 1을 추가함
			CreateCodeTable(code, root->Getright()); // 오론쪽 node로 이동
		}
	}
}

void Encoder::encoder() {
	string zero = "00000000";
	string code = "";
	std::ofstream fin_table(out_table, ios::out | ios::binary); // huffman_table.hbs 파일을 binary로 open
	for (auto p = code_table.begin(); p != code_table.end(); p++) {
		string ascii = DecimalToString(p->first); // code_table의 symbol을 2진수로 변경
		code += ascii; // code 문자열의 맨뒤에 더함
		string code_len = DecimalToString(p->second.size()); // symbol의 huffman code의 길이를 2진수로 변환
		code += code_len; // code 문자열의 맨 뒤에 더함
		code += p->second; // symbol의 huffman code를 code 문자열 맨 뒤에 더함

		if (code.size() / 8 > 0) { // 코드의 길이가 8자리가 넘어가면 
			int count = code.size() / 8; // 몫을 구함
			for (int i = 0; i < count; i++) {
				int hex_code = StringToDecimal(code.substr(0, 8)); // 앞에서 부터 8개씩 쪼개서 10진수로 변환
				fin_table.write((char*)&hex_code, sizeof(char)); // 해당 10진수를 파일에 저장
				code = code.erase(0, 8); // 앞에서 8개의 문자를 제거
			}
		}
	}
	// Byte Align
	code += zero.substr(0, 8 - code.size()); // 코드 뒤에 byte align 하기 위해 8 - code.size()만큼 0을 붙임
	int hex_code = StringToDecimal(code); // code 값을 10진수로 변환
	fin_table.write((char*)&hex_code, sizeof(char)); // 파일에 10진수를 작성
	fin_table.close(); // 파일을 닫음

	char s;
	code = "";
	std::ofstream fin_code(out_code, ios::out | ios::binary); // huffman_code 파일을 binary로 open
	FILE* fp; // 파일 포인터 선언
	fopen_s(&fp, input, "r"); // input_data 파일을 open
	if (fp == NULL) { // 파일이 없으면 그냥 return 함
		cout << "Input File is not EXIST!!" << endl;
		return;
	}
	while ((s = fgetc(fp)) != EOF) { // input_data 파일의 끝에 도달할때까지 반복함
		code += code_table[s]; // symbol의 huffman code를 가져와서 code의 맨 뒤에 저장

		if (code.size() > 8) { // code의 길이가 8보다 크면 
			int count = code.size() / 8; // code 길이를 8로 나눈 몫 저장
			for (int i = 0; i < count; i++) {
				int hex_code = StringToDecimal(code.substr(0, 8)); // 앞에서 8개의 문자를 10진수로 변환
				fin_code.write((char*)&hex_code, sizeof(char)); // 파일에 10진수 값을 저장
				code = code.erase(0, 8); // 앞의 8개의 문자를 삭제
			}
		}
	}
	// EOD를 hbs 파일에 저장
	code += code_table[(char)26]; // code뒤에 EOD의 huffman code를 붙임
	code += zero.substr(0, 8 - (code.size() % 8)); // Byte align을 함
	int count = code.size() / 8; // 길이에 8을 나눈 몫을 저장
	for (int i = 0; i < count; i++) { 
		int hex_code = StringToDecimal(code.substr(0, 8)); // 앞에서 8개 문자를 10진수로 변환
		fin_code.write((char*)&hex_code, sizeof(char)); // 파일에 저장
		code = code.erase(0, 8); // 앞에서 8개의 문자를 삭제
	}
	fclose(fp); // 파일을 닫음
	fin_code.close(); // 파일을 닫음
}

