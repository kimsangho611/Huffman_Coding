#include "Decoder.h"

string Decoder::DecimalToString(int num)
{
	string binary = "";
	while (num != 0) { // num�� 0�� ������ �ݺ�
		binary += (num % 2 == 0 ? "0" : "1"); // 2�� �������� �� �������� 0�̸� 0�� 0�� �ƴϸ� 1�� ����
		num /= 2; // num�� 2�� ���� ���� ������
	}
	if (binary.size() < 8) {
		// 8���� bit�� ���߱� ���� zero extension�� ��
		int rest_bit = 8 - binary.size();
		for (int i = 0; i < rest_bit; i++) {
			binary += "0";
		}
	}
	reverse(binary.begin(), binary.end()); // ����� �����´�.
	return binary; // ����� ��ȯ��
}

int Decoder::StringToDecimal(string binary)
{
	int num = 0; // num�� 0�� ����
	for (int i = 0; i < binary.size(); i++)
		if (binary.c_str()[i] == '1') { // �ش� ��ġ�� ���� 1�̸� 
			num += pow(2, binary.size() - i - 1); // �ش� �ڸ����� �´� ���� ���� ����
		}
	return num; // 10�� ��ȯ ����� ��ȯ��
}

int Decoder::Check_Code_Table(string code)
{
	if (code_table[code]) return code_table[code]; // huffman code�� ���� �����ϴ� ��� �ش� ���� ��ȯ��
	else return -1; // ������ -1�� ��ȯ��
}

void Decoder::DecodeTable()
{
	FILE* fp; // ���� �����͸� ������
	fopen_s(&fp, table, "rb"); // huffman table ������ binary�� open
	if (fp == NULL) { // ������ ������ �׳� return ��
		cout << "huffman_table.hbs File is not EXIST!!" << endl;
	}
	int code, ascii, len;
	string table_code = "";
	while (1) { // ���� �ݺ�
		if (!feof(fp)) { // ������ ���� �ƴϸ�
			code = fgetc(fp); // ���� 1���� ����
			table_code += DecimalToString(code); // ascii int ���� binary code�� ����
			ascii = StringToDecimal(table_code.substr(0, 8)); // �տ��� 8���� ���ڸ� 10������ ��ȯ
			table_code = table_code.erase(0, 8); // �տ��� 8���� ���ڸ� ����
		}
		else break; // ������ ���̸� break

		if (!feof(fp)) { // ������ ���� �ƴϸ�
			code = fgetc(fp); // ���� 1���� ����
			table_code += DecimalToString(code); // code�� 2������ ��ȯ
			len = StringToDecimal(table_code.substr(0, 8)); // �տ��� 8���� ���ڸ� 10������ ��ȯ
			table_code = table_code.erase(0, 8); // �տ��� 8�� ���ڸ� ����
		}
		else break; // ������ ���̸� break

		if (!feof(fp)) { // ������ ���� �ƴϸ�
			code = fgetc(fp); // ���� 1���� ����
			table_code += DecimalToString(code); // ���� ���ڸ� 2������ ��ȯ�Ͽ� code�� �� �ڿ� ����
			while (table_code.size() < len) { // code size�� �ռ� ���� ���̺��� ���� ���
				code = fgetc(fp); // ������ �ѹ��� ����
				table_code += DecimalToString(code); // 2������ ��ȯ�Ͽ� table_code ���ڿ� �ڿ� ����
			}
			code_table.insert({ table_code.substr(0, len), (char)ascii }); // huffman code�� �ش� symbol�� code_table�� ����
			table_code = table_code.erase(0, len); // huffman code�� ���̸�ŭ ���ڿ��� ����
		}
		else break; // ������ ���̸� break
	}
	code_table.insert({ table_code.substr(0, len), (char)ascii }); // huffman code�� ���� ��ŭ �߶� symbol�� code_table�� insert
	table_code = table_code.erase(0, len); // huffman code�� ���̸�ŭ ���ڿ����� ���� 

	ascii = StringToDecimal(table_code.substr(0, 8)); // ���ڿ����� 8���� �߶� 10������ ��ȯ
	table_code = table_code.erase(0, 8); // ���ڿ����� ���� 8�� ���� ����
	 
	len = StringToDecimal(table_code.substr(0, 8)); // �տ��� 8�� ���� �߶� 10������ ��ȯ
	table_code = table_code.erase(0, 8); // �տ��� 8���� ���ڿ����� ����

	code_table.insert({ table_code.substr(0, len), (char)ascii }); // ���ڿ����� huffman code�� ���̸�ŭ �߶� symbol�� �Բ� insert
	table_code = table_code.erase(0, len); // ���ڿ����� huffman code�� ���̸�ŭ ����
	fclose(fp); // ������ ����
}

void Decoder::decoder() {
	FILE* fp;
	fopen_s(&fp, "./huffman_code.hbs", "rb"); // ������ binary�� open
	if (fp == NULL) { // ������ ������ �׳� return
		cout << "huffman_code.hbs File is not EXIST!!" << endl;
		return;
	}
	std::ofstream fin(output); // output.txt ������ open

	string code = "";
	bool isEOD = 0; // ������ ������ Ȯ���ϴ� ����
	while (1) {
		if (!feof(fp)) // ������ ���� �ƴϸ� 
			code += DecimalToString(fgetc(fp)); // ���ڸ� �а� �̸� 2������ ��ȯ�Ͽ� code ���ڿ� �ڿ� ����

		for (int i = 0; i < code.size(); i++) {
			int Decode_value = Check_Code_Table(code.substr(0, i)); // �տ��� ���ڸ� 1���� �÷����鼭 huffman code�� �ִ��� Ȯ��
			if (Decode_value == 26) { // huffman code�� EOD�� 
				isEOD = 1; // isEOD�� 1�� �����ϰ� �ݺ��� ����
				break;
			}
			if (Decode_value >= 0) { // huffman code�� �����ϸ� 
				fin << (char)Decode_value; // int���� char������ ��ȯ�Ͽ� ���Ͽ� ����
				code = code.erase(0, i); // huffman code�� ���̸�ŭ ���ڿ� ����
				i = 0; // i�� 0���� �ʱ�ȭ
			}
		}
		if (isEOD) break; // EOD�� ���� �ݺ��� ����
	}
	fin.close(); // ������ ����
	fclose(fp); // ������ ����
}