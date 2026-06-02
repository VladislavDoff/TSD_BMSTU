#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct Node {
	char ch;
	int freq;
	Node *left, *right;
};

struct Compare {
	bool operator()(Node* l, Node* r) {
		return (l->freq > r->freq);
	}
};

Node* getNode(char ch, int freq, Node* left, Node* right) {
	Node* node = new Node();
	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;
	return node;
}

void encode(Node* root, string str, map<char, string> &huffmanCode) {
	if (root == nullptr) return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void writeHuffmanCodesToFile(const map<char, string>& huffmanCode, const string& encodedText, const map<char, int>& freq) {
	ofstream outputFile("C:\\Users\\Kononova\\Desktop\\huffman_codes.bin", ios::binary);

	if (outputFile.is_open()) {
		for (const auto& pair : huffmanCode) {
			outputFile.put(pair.first);

			int frequency_num = freq.at(pair.first);
			outputFile.write(reinterpret_cast<const char*>(&frequency_num), sizeof(frequency_num));

			char codeLength = static_cast<char>(pair.second.length());
			outputFile.put(codeLength);
		}

		for (char ch : encodedText) {
			outputFile.put(ch);
		}

		outputFile.close();
		cout << "Данные успешно записаны в бинарный файл 'huffman_codes.bin'" << endl;
	}
	else {
		cout << "Невозможно открыть файл для записи" << endl;
	}
}

void winnings(string text, const map<char, string>& huffmanCode, const map<char, int>& freq) {
	int bit_1 = text.size() * 8;
	int bit_2 = 0;
	
	for (auto &pair : huffmanCode) {
		bit_2 += pair.second.size() *  freq.at(pair.first);
	}
	double win = (bit_1 + 0.0) / bit_2;
	cout << "Сжатие в " << win << " раз" << endl;
}

void buildHuffmanTree(string text) {
	map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	priority_queue<Node*, vector<Node*>, Compare> pq;	

	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1) {
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top(); pq.pop();

		int sumFreq = left->freq + right->freq;
		pq.push(getNode('\0', sumFreq, left, right));
	}

	Node* root = pq.top();

	map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	string encodedText;
	for (char ch : text) {
		encodedText += huffmanCode[ch];
	}

	cout << "Частоты букв: " << endl;
	for (auto pair : freq) {
		cout << pair.first << " : " << pair.second << endl;
	}
	cout << endl;

	cout << "Код букв: " << endl;
	for (auto pair : huffmanCode) {
		cout << pair.first << " : " << pair.second << endl;
	}
	cout << endl;

	cout << "Закодированный текст по Хаффману: " << encodedText << endl;

	writeHuffmanCodesToFile(huffmanCode, encodedText, freq);
	cout << endl;
	winnings(text, huffmanCode, freq);
}

int main() {
	setlocale(0, "");
	ifstream file("C:\\Users\\Kononova\\Desktop\\huff.lisp");
	string text = "";
	if (file.is_open()) {
		string text_help = "";
		while (getline(file, text_help)){
			text += text_help;
			text_help = "";
		}
		file.close();
	}
	else {
		cout << "Невозможно открыть файл";
		return 0;
	}
	cout << "Прочитанная фраза: " <<text << endl;
	buildHuffmanTree(text);
	
	return 0;
}
