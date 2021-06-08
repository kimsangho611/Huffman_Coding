#include "Encoder.h"
#include "Decoder.h"

using namespace std;
int main(void) {
	Encoder encode("./input_data.txt", "./huffman_table.hbs", "./huffman_code.hbs");
	encode.encoder(); // ������ ���ڵ�

	Decoder decode("./huffman_table.hbs", "./output.txt"); // Decoder ��ü ����
	decode.decoder(); // ������ ���ڵ�

	return 0; // 0�� ��ȯ��
}