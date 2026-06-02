#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

class AVLTree {
private:
	struct Node {
		int key;
		Node* left;
		Node* right;
		int height;

		Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
	};

	Node* root;

	int height(Node* node) {
		if (node == nullptr) return 0;
		return node->height;
	}

	int balanceFactor(Node* node) {
		if (node == nullptr) return 0;
		return height(node->left) - height(node->right);
	}

	Node* rightRotate(Node* root) {
		Node* x = root->left;
		Node* T2 = x->right;
		x->right = root;
		root->left = T2;

		root->height = max(height(root->left), height(root->right)) + 1;
		x->height = max(height(x->left), height(x->right)) + 1;

		return x;
	}

	Node* leftRotate(Node* root) {
		Node* y = root->right;
		Node* T2 = y->left;
		y->left = root;
		root->right = T2;

		root->height = max(height(root->left), height(root->right)) + 1;
		y->height = max(height(y->left), height(y->right)) + 1;

		return y;
	}

	Node* insert(Node* node, int key) {
		if (node == nullptr) return new Node(key);

		if (key < node->key) {
			node->left = insert(node->left, key);
		}
		else if (key > node->key) {
			node->right = insert(node->right, key);
		}
		else {
			return node;
		}
		
		node->height = 1 + max(height(node->left), height(node->right));

		int balance = balanceFactor(node);

		if (balance > 1 && key < node->left->key) {
			return rightRotate(node);
		}
		if (balance < -1 && key > node->right->key) {
			return leftRotate(node);
		}
		if (balance > 1 && key > node->left->key) {
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}
		if (balance < -1 && key < node->right->key) {
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node;
	}

	Node* minValueNode(Node* node) {
		Node* current = node;

		while (current->left != nullptr) {
			current = current->left;
		}

		return current;
	}

	Node* remove(Node* root, int key) {
		if (root == nullptr) return root;

		if (key < root->key) {
			root->left = remove(root->left, key);
		}
		else if (key > root->key) {
			root->right = remove(root->right, key);
		}
		else {
			if (root->left == nullptr || root->right == nullptr) {
				Node* temp = root->left ? root->left : root->right;

				if (temp == nullptr) {
					temp = root;
					root = nullptr;
				}
				else{ 
					*root = *temp; 
				}

				delete temp;
			}
			else {
				Node* temp = minValueNode(root->right);
				root->key = temp->key;
				root->right = remove(root->right, temp->key);
			}
		}

		if (root == nullptr) return root;

		root->height = 1 + max(height(root->left), height(root->right));

		int balance = balanceFactor(root);

		if (balance > 1 && balanceFactor(root->left) >= 0) {
			return rightRotate(root);
		}
		if (balance > 1 && balanceFactor(root->left) < 0) {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
		if (balance < -1 && balanceFactor(root->right) <= 0) {
			return leftRotate(root);
		}
		if (balance < -1 && balanceFactor(root->right) > 0) {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	bool search(Node* root, int key) {
		if (root == nullptr) return false;

		if (key == root->key) {
			return true;
		}
		else if (key < root->key) {
			return search(root->left, key);
		}
		else {
			return search(root->right, key);
		}
	}

public:
	AVLTree() : root(nullptr) {}

	void insert(int key) {
		root = insert(root, key);
	}

	void remove(int key) {
		root = remove(root, key);
	}

	bool search(int key) {
		return search(root, key);
	}

	void printTreeLevelOrder() {
		if (root == nullptr) {
			cout << "Дерево пусто." << endl;
			return;
		}

		cout << "Дерево (по уровням):" << endl;
		queue<Node*> levelQueue;
		levelQueue.push(root);

		while (!levelQueue.empty()) {
			int levelSize = levelQueue.size();

			while (levelSize > 0) {
				Node* current = levelQueue.front();
				levelQueue.pop();

				cout << current->key << " ";

				if (current->left != nullptr) {
					levelQueue.push(current->left);
				}
				if (current->right != nullptr) {
					levelQueue.push(current->right);
				}

				levelSize--;
			}

			cout << endl;
		}
	}
};

int main() {
	setlocale(0, "");
	AVLTree tree;

	int choice, key;
	bool running = true;

	while (running) {
		cout << "Выберите операцию:" << endl;
		cout << "1. Добавить элемент" << endl;
		cout << "2. Удалить элемент" << endl;
		cout << "3. Поиск элемента" << endl;
		cout << "4. Вывести дерево (по уровням)" << endl;
		cout << "5. Выйти из программы" << endl;
		cout << "Введите выбор: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "Введите ключ для добавления: ";
			cin >> key;
			tree.insert(key);
			cout << endl;
			break;
		case 2:
			cout << "Введите ключ для удаления: ";
			cin >> key;
			tree.remove(key);
			cout << endl;
			break;
		case 3:
			cout << "Введите ключ для поиска: ";
			cin >> key;
			if (tree.search(key))
				cout << "Элемент найден." << endl;
			else
				cout << "Элемент не найден." << endl;
			cout << endl;
			break;
		case 4:
			tree.printTreeLevelOrder();
			break;
		case 5:
			running = false;
			break;
		default:
			cout << "Неверный выбор! Попробуйте еще раз." << endl;
		}
	}

	return 0;
}
