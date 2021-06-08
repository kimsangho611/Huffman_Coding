#pragma once

class HuffmanNode {
private:
	char symbol; // ascii ���� �����ϴ� ����

	HuffmanNode* parent; // �θ� node�� ����Ű�� ����
	HuffmanNode* left; // ���� node�� ����Ű�� ����
	HuffmanNode* right; // ������ node�� ����Ű�� ����

public:
	HuffmanNode() { // ���� �ʱ�ȭ
		this->parent = nullptr; this->left = nullptr; this->right = nullptr;
		symbol = NULL;
	}
	~HuffmanNode() {};

	char GetSymbol() { // Node�� ����� symbol�� ��ȯ�ϴ� �Լ� 
		return symbol;
	}

	void SetSymbol(char sb) { // symbol�� �����ϴ� �Լ�
		symbol = sb;
	}

	void SetLeft(HuffmanNode* node) { // ���� node�� �����ϴ� �Լ� 
		this->left = node;
	}
	void SetRight(HuffmanNode* node) { // ������ node�� �����ϴ� �Լ� 
		this->right = node;
	}
	void SetParent(HuffmanNode* node) { // �θ� node�� �����ϴ� �Լ� 
		this->parent = node;
	}

	HuffmanNode* GetLeft() { // ���� node�� ��ȯ�ϴ� �Լ�
		return this->left;
	}
	HuffmanNode* Getparent() { // �θ� node�� ��ȯ�ϴ� �Լ� 
		return this->parent;
	}
	HuffmanNode* Getright() { // ������ node�� ��ȯ�ϴ� �Լ� 
		return this->right;
	}
};