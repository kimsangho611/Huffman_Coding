#include "Decoder.h"

string Decoder::DecimalToString(int num)
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

int Decoder::StringToDecimal(string binary)
{
	int num = 0; // num에 0을 저장
	for (int i = 0; i < binary.size(); i++)
		if (binary.c_str()[i] == '1') { // 해당 위치의 값이 1이면 
			num += pow(2, binary.size() - i - 1); // 해당 자릿수에 맞는 지수 값을 더함
		}
	return num; // 10진 변환 결과를 반환함
}

int Decoder::Check_Code_Table(string code)
{
	if (code_table[code]) return code_table[code]; // huffman code의 값이 존재하는 경우 해당 값을 반환함
	else return -1; // 없으면 -1을 반환함
}

void Decoder::DecodeTable()
{
	FILE* fp; // 파일 포인터를 선언함
	fopen_s(&fp, table, "rb"); // huffman table 파일을 binary로 open
	if (fp == NULL) { // 파일이 없으면 그냥 return 함
		cout << "huffman_table.hbs File is not EXIST!!" << endl;
	}
	int code, ascii, len;
	string table_code = "";
	while (1) { // 무한 반복
		if (!feof(fp)) { // 파일의 끝이 아니면
			code = fgetc(fp); // 문자 1개를 읽음
			table_code += DecimalToString(code); // ascii int 값을 binary code로 변경
			ascii = StringToDecimal(table_code.substr(0, 8)); // 앞에서 8개의 문자를 10진수로 변환
			table_code = table_code.erase(0, 8); // 앞에서 8개의 문자를 삭제
		}
		else break; // 파일의 끝이면 break

		if (!feof(fp)) { // 파일의 끝이 아니면
			code = fgetc(fp); // 문자 1개를 읽음
			table_code += DecimalToString(code); // code를 2진수로 변환
			len = StringToDecimal(table_code.substr(0, 8)); // 앞에서 8개의 문자를 10진수로 변환
			table_code = table_code.erase(0, 8); // 앞에서 8개 문자를 삭제
		}
		else break; // 파일의 끝이면 break

		if (!feof(fp)) { // 파일의 끝이 아니면
			code = fgetc(fp); // 문자 1개를 읽음
			table_code += DecimalToString(code); // 읽은 문자를 2진수로 변환하여 code의 맨 뒤에 더함
			while (table_code.size() < len) { // code size가 앞서 읽은 길이보다 작은 경우
				code = fgetc(fp); // 파일을 한번더 읽음
				table_code += DecimalToString(code); // 2진수로 변환하여 table_code 문자열 뒤에 더함
			}
			code_table.insert({ table_code.substr(0, len), (char)ascii }); // huffman code와 해당 symbol을 code_table에 저장
			table_code = table_code.erase(0, len); // huffman code의 길이만큼 문자열을 제거
		}
		else break; // 파일의 끝이면 break
	}
	code_table.insert({ table_code.substr(0, len), (char)ascii }); // huffman code의 길이 만큼 잘라서 symbol과 code_table에 insert
	table_code = table_code.erase(0, len); // huffman code의 길이만큼 문자열에서 삭제 

	ascii = StringToDecimal(table_code.substr(0, 8)); // 문자열에서 8개를 잘라서 10진수로 변환
	table_code = table_code.erase(0, 8); // 문자열에서 앞의 8개 문자 삭제
	 
	len = StringToDecimal(table_code.substr(0, 8)); // 앞에서 8개 문자 잘라서 10진수로 변환
	table_code = table_code.erase(0, 8); // 앞에서 8개를 문자열에서 삭제

	code_table.insert({ table_code.substr(0, len), (char)ascii }); // 문자열에서 huffman code의 길이만큼 잘라 symbol과 함께 insert
	table_code = table_code.erase(0, len); // 문자열에서 huffman code의 길이만큼 삭제
	fclose(fp); // 파일을 닫음
}

void Decoder::decoder() {
	FILE* fp;
	fopen_s(&fp, "./huffman_code.hbs", "rb"); // 파일을 binary로 open
	if (fp == NULL) { // 파일이 없으면 그냥 return
		cout << "huffman_code.hbs File is not EXIST!!" << endl;
		return;
	}
	std::ofstream fin(output); // output.txt 파일을 open

	string code = "";
	bool isEOD = 0; // 파일의 끝인지 확인하는 변수
	while (1) {
		if (!feof(fp)) // 파일의 끝이 아니면 
			code += DecimalToString(fgetc(fp)); // 문자를 읽고 이를 2진수로 변환하여 code 문자열 뒤에 저장

		for (int i = 0; i < code.size(); i++) {
			int Decode_value = Check_Code_Table(code.substr(0, i)); // 앞에서 문자를 1개씩 늘려가면서 huffman code가 있는지 확인
			if (Decode_value == 26) { // huffman code가 EOD면 
				isEOD = 1; // isEOD에 1을 저장하고 반복문 종료
				break;
			}
			if (Decode_value >= 0) { // huffman code가 존재하면 
				fin << (char)Decode_value; // int형을 char형으로 변환하여 파일에 저장
				code = code.erase(0, i); // huffman code의 길이만큼 문자열 삭제
				i = 0; // i를 0으로 초기화
			}
		}
		if (isEOD) break; // EOD면 무한 반복문 종료
	}
	fin.close(); // 파일을 닫음
	fclose(fp); // 파일을 닫음
}