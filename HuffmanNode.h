#pragma once

class HuffmanNode {
private:
	char symbol; // ascii 값을 저장하는 변수

	HuffmanNode* parent; // 부모 node를 가리키는 변수
	HuffmanNode* left; // 왼쪽 node를 가리키는 변수
	HuffmanNode* right; // 오른쪽 node를 가리키는 변수

public:
	HuffmanNode() { // 변수 초기화
		this->parent = nullptr; this->left = nullptr; this->right = nullptr;
		symbol = NULL;
	}
	~HuffmanNode() {};

	char GetSymbol() { // Node에 저장된 symbol을 반환하는 함수 
		return symbol;
	}

	void SetSymbol(char sb) { // symbol을 저장하는 함수
		symbol = sb;
	}

	void SetLeft(HuffmanNode* node) { // 왼쪽 node를 설정하는 함수 
		this->left = node;
	}
	void SetRight(HuffmanNode* node) { // 오른쪽 node를 설정하는 함수 
		this->right = node;
	}
	void SetParent(HuffmanNode* node) { // 부모 node를 설정하는 함수 
		this->parent = node;
	}

	HuffmanNode* GetLeft() { // 왼쪽 node를 반환하는 함수
		return this->left;
	}
	HuffmanNode* Getparent() { // 부모 node를 반환하는 함수 
		return this->parent;
	}
	HuffmanNode* Getright() { // 오른쪽 node를 반환하는 함수 
		return this->right;
	}
};