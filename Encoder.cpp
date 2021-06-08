#include "Encoder.h"

string Encoder::DecimalToString(int num)
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

int Encoder::StringToDecimal(string binary)
{
	int num = 0; // num�� 0�� ����
	for (int i = 0; i < binary.size(); i++)
		if (binary.c_str()[i] == '1') { // �ش� ��ġ�� ���� 1�̸� 
			num += pow(2, binary.size() - i - 1); // �ش� �ڸ����� �´� ���� ���� ����
		}
	return num; // 10�� ��ȯ ����� ��ȯ��
}

void Encoder::CreateFrequencyTable() {
	int s;
	int frequent[128] = { 0, }; // �� �������� �󵵸� �����ϴ� �迭 ������ �����ϰ� 0���� �ʱ�ȭ��
	FILE* fin; // FIle ������ ����
	fopen_s(&fin, input, "r"); // ������ �б� ���� open
	if (fin == NULL) { // ������ ���� ���ϸ� �׳� return ��
		cout << "Input File is not EXIST!!" << endl;
		return;
	}
	while ((s = fgetc(fin)) != EOF) { // ������ ������ 1���� �о��
		++frequent[s]; // frequency�� s�� ������ ���� �ε����� ���� 1����
		total_char_count++; // total_char_count ������ 1����
	}
	fclose(fin); // fin ������ ����

	/* �� ���Ͽ��� ���ڰ� ���� ���� queue�� �����ϴ� code */
	HuffmanNode* node = new HuffmanNode; // HuffmanNode�� �����Ҵ�
	node->SetSymbol(26); // symbol 26 (EOD)�� ����
	pq.push({ 1.0 / (double)total_char_count, node}); // Ȯ���� node�� priority queue�� push
	for (int i = 0; i < 128; i++) {
		if (frequent[i] != 0) { // frequency�� 0�� �ƴ� ��� 
			HuffmanNode* node = new HuffmanNode; // ���ο� node�� �����Ҵ���
			node->SetSymbol((char)i); // symbol�� ����
			pq.push({ (double)frequent[i] / (double)total_char_count, node }); // priority queue�� push
		}
	}
}

void Encoder::CreateTree() {
	while (pq.size() != 1) { // priority queue�� size�� 1�� �ƴҶ����� �ݺ�
		// ���� ���� �ִ� ���� 2���� GET�ϰ� POP��
		pair<double, HuffmanNode*> operand1 = pq.top(); 
		pq.pop();
		pair<double, HuffmanNode*> operand2 = pq.top();
		pq.pop();

		// ���ο� node�� �����Ҵ��Ͽ� ���� 2�� ���Ҹ� ���ο� node�� child��� Setting
		HuffmanNode* node = new HuffmanNode;
		node->SetLeft(operand1.second);
		node->SetRight(operand2.second);

		// 2���� ������ parent�� setting
		operand1.second->SetParent(node);
		operand2.second->SetParent(node);
		
		// priority queue�� 2���� Ȯ���� ���ѵ� ���ο� node�� priority queue�� push
		pq.push({ operand1.first + operand2.first, node });
	}
	// ���������� Root ������ tree ����
	pair<double, HuffmanNode*> Top = pq.top();
	pq.pop();
	Root = Top.second;
	CreateCodeTable("", Root); // huffman tree�� ����� code ����
}

void Encoder::CreateCodeTable(string code, HuffmanNode* root) {
	if (root != nullptr) {  // if root�� null�� �ƴϸ� 
		if (root->GetSymbol() != NULL) { // root�� symbol�� null�� �ƴϸ�
			code_table.insert({ root->GetSymbol(), code }); // code_table�� symbol�� code insert
		}
		if (root->GetLeft() != NULL) { // if ���� �ڽ��� ������ 
			code += "0"; // code���ڿ��� �� �ڿ� 0�� �߰���
			CreateCodeTable(code, root->GetLeft()); // ���� node�� �̵�
		}
		if (root->Getright() != NULL) { // if ������ �ڽ��� ������ 
			code = code.erase(code.size() - 1, code.size()); // code�� �� �� ���� ����
			code += "1"; // code�� ���� �ڿ� 1�� �߰���
			CreateCodeTable(code, root->Getright()); // ������ node�� �̵�
		}
	}
}

void Encoder::encoder() {
	string zero = "00000000";
	string code = "";
	std::ofstream fin_table(out_table, ios::out | ios::binary); // huffman_table.hbs ������ binary�� open
	for (auto p = code_table.begin(); p != code_table.end(); p++) {
		string ascii = DecimalToString(p->first); // code_table�� symbol�� 2������ ����
		code += ascii; // code ���ڿ��� �ǵڿ� ����
		string code_len = DecimalToString(p->second.size()); // symbol�� huffman code�� ���̸� 2������ ��ȯ
		code += code_len; // code ���ڿ��� �� �ڿ� ����
		code += p->second; // symbol�� huffman code�� code ���ڿ� �� �ڿ� ����

		if (code.size() / 8 > 0) { // �ڵ��� ���̰� 8�ڸ��� �Ѿ�� 
			int count = code.size() / 8; // ���� ����
			for (int i = 0; i < count; i++) {
				int hex_code = StringToDecimal(code.substr(0, 8)); // �տ��� ���� 8���� �ɰ��� 10������ ��ȯ
				fin_table.write((char*)&hex_code, sizeof(char)); // �ش� 10������ ���Ͽ� ����
				code = code.erase(0, 8); // �տ��� 8���� ���ڸ� ����
			}
		}
	}
	// Byte Align
	code += zero.substr(0, 8 - code.size()); // �ڵ� �ڿ� byte align �ϱ� ���� 8 - code.size()��ŭ 0�� ����
	int hex_code = StringToDecimal(code); // code ���� 10������ ��ȯ
	fin_table.write((char*)&hex_code, sizeof(char)); // ���Ͽ� 10������ �ۼ�
	fin_table.close(); // ������ ����

	char s;
	code = "";
	std::ofstream fin_code(out_code, ios::out | ios::binary); // huffman_code ������ binary�� open
	FILE* fp; // ���� ������ ����
	fopen_s(&fp, input, "r"); // input_data ������ open
	if (fp == NULL) { // ������ ������ �׳� return ��
		cout << "Input File is not EXIST!!" << endl;
		return;
	}
	while ((s = fgetc(fp)) != EOF) { // input_data ������ ���� �����Ҷ����� �ݺ���
		code += code_table[s]; // symbol�� huffman code�� �����ͼ� code�� �� �ڿ� ����

		if (code.size() > 8) { // code�� ���̰� 8���� ũ�� 
			int count = code.size() / 8; // code ���̸� 8�� ���� �� ����
			for (int i = 0; i < count; i++) {
				int hex_code = StringToDecimal(code.substr(0, 8)); // �տ��� 8���� ���ڸ� 10������ ��ȯ
				fin_code.write((char*)&hex_code, sizeof(char)); // ���Ͽ� 10���� ���� ����
				code = code.erase(0, 8); // ���� 8���� ���ڸ� ����
			}
		}
	}
	// EOD�� hbs ���Ͽ� ����
	code += code_table[(char)26]; // code�ڿ� EOD�� huffman code�� ����
	code += zero.substr(0, 8 - (code.size() % 8)); // Byte align�� ��
	int count = code.size() / 8; // ���̿� 8�� ���� ���� ����
	for (int i = 0; i < count; i++) { 
		int hex_code = StringToDecimal(code.substr(0, 8)); // �տ��� 8�� ���ڸ� 10������ ��ȯ
		fin_code.write((char*)&hex_code, sizeof(char)); // ���Ͽ� ����
		code = code.erase(0, 8); // �տ��� 8���� ���ڸ� ����
	}
	fclose(fp); // ������ ����
	fin_code.close(); // ������ ����
}

