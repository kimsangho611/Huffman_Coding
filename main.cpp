#include "Encoder.h"
#include "Decoder.h"

using namespace std;
int main(void) {
	Encoder encode("./input_data.txt", "./huffman_table.hbs", "./huffman_code.hbs");
	encode.encoder(); // 파일을 인코딩

	Decoder decode("./huffman_table.hbs", "./output.txt"); // Decoder 객체 생성
	decode.decoder(); // 파일을 디코딩

	return 0; // 0을 반환함
}