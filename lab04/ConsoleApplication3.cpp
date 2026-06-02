#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <locale>

using namespace std;

const double PI = 3.141592653589793;

template<typename T>
class List {
public:
	List() {
		size = 0;
		head = nullptr;
	}

	~List() {}

	void push_back(T date) {
		if (head == nullptr) {
			head = new Node<T>(date);
		}
		else {
			Node<T> *current = this->head;
			while (current->pNext != nullptr) {
				current = current->pNext;
			}
			current->pNext = new Node<T>(date);
		}
		size++;
	}

	int GetSize() {
		return size;
	}

	void pop_back() {
		Node<T> *previous = this->head;
		for (int i = 0; i < GetSize() - 2; i++) {
			previous = previous->pNext;
		}
		if (size > 1) {
			Node<T> *toDelete = previous->pNext;
			previous->pNext = toDelete->pNext;
			delete toDelete;
			size--;
		}
		else {
			Node<T> *toDelete = head;
			head = nullptr;
			delete toDelete;
			size--;
		}
	}

	void check() {
		if (size == 0) {
			cout << "Стек пустой" << endl;
		}
		else {
			cout << "В стеке хранятся данные" << endl;
		}
	}

	void print() {
		Node<T> *current = this->head;
		while (current != nullptr) {
			cout << current->date;
			current = current->pNext;
		}
	}

	T& operator[](const int index)
	{
		int counter = 0;
		Node<T> *current = this->head;
		while (current != nullptr){
			if (counter == index){
				return current->date;
			}
			current = current->pNext;
			counter++;
		}
	}


private:

	template<typename T>
	class Node {
	public:
		Node * pNext;
		T date;

		Node(T date, Node * pNext = nullptr) {
			this->date = date;
			this->pNext = pNext;
		}
	};
	Node<T> *head;
	int size;
};

void read_file(string &inf) {
	string filename = "C:\\Users\\Kononova\\Desktop\\lab_3_2_inf.txt";
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Ошибка: Не удалось открыть файл." << endl;
		exit;
	}
	while (getline(file, inf)) {
		cout << "Прочитанная строка: " << inf << endl << endl;
	}
	file.close();
}

bool isNumber(const string& s) {
	for (char c : s) {
		if (!isdigit(c) && c != '.' && c != '-') {
			return false;
		}
	}
	return true;
}

bool isOperator(string &c) {
	return (c == "+" || c == "-" || c == "*" || c == "/" || c == "^" || c == "(" || c == ")");
}

bool isFunction(string& token) {
	return (token == "sin" || token == "ctg");
}

bool valid_inf(string &inf) {
	int brackets = 0;
	for (int i = 0; i < inf.size(); i++) {
		if (i == 0 && inf[i] == '-') {
			inf.insert(0, "0 ");
		}
		if (i > 1 && inf[i] == '-' && inf[i - 2] == '(') {
			inf.insert(i , "0 ");
		}
		if (i > 1 && (inf[i] == '-' || inf[i] == '+' || inf[i] == '*' || inf[i] == '/')
			&& (inf[i - 2] == '-' || inf[i - 2] == '+' || inf[i - 2] == '*' || inf[i - 2] == '/')) {
			cout << "Ошибка в валидности операторов" << endl << endl;
			return 0;
		}
		if (i > 1 && inf[i] == '-' && (inf[i - 2] == '/' || inf[i - 2] == '*')) {
			inf.insert(i + 4, ") ");
			inf.insert(i, "( 0 ");		
		}
		if (inf[i] == '(') {
			brackets++;
		}
		else if(inf[i] == ')'){
			brackets--;
			if (brackets < 0) {
				cout << "Неправильно расставлены скобки" << endl << endl;
				return false;
			}
		}
	}
	if (brackets != 0) {
		cout << "Неправильно расставлены скобки" << endl << endl;
		return false;
	}
	int operators = 0, operands = 0;
	for (int i = 0; i < inf.size(); i++) {
		string check;
		while (i < inf.size() && inf[i] != ' ') {
			check += inf[i];
			i++;
		}
		if (isOperator(check)) {
			operators++;
			continue;
		}
		else if (isFunction(check)) {
			operators++;
			continue;
		}
		else if (isNumber(check)) {
			operands++;
			continue;
		}
		else if (check == "x") {
			operands++;
			continue;
		}
		else {
			cout << "Ошибка в валидности токенов" << endl << endl;
			return 0;
		}		
	}
	if (operands - 1 != operators) {
		cout << "Ошибка в валидности операторов" << endl << endl;
		return 0;
	}
	return true;
}

int precedence(char op) {
	if (op == 's' || op == 'c')
		return 4;
	else if (op == '^')
		return 3;
	else if (op == '*' || op == '/')
		return 2;
	else if (op == '+' || op == '-')
		return 1;
	else
		return -1;
}

string infixToPostfix(const string& expression) {
	List<char> operators;
	string postfix;

	for (char c : expression) {
		if (c == ' ' || c == 'i' || c == 'n' || c == 't'  || c == 'g') {
			continue;
		}
		if (c == '.') {
			postfix += c;
			continue;
		}
		if (c != 's' && c != 'c' && isalnum(c)) {
			postfix += c;
		}
		else if (c == '(') {
			operators.push_back(c);
		}
		else if (c == ')') {
			while (operators.GetSize() > 0 && operators[operators.GetSize() - 1] != '(') {
				postfix += ' ';
				postfix += operators[operators.GetSize() - 1];
				if (operators[operators.GetSize() - 1] == 's') {
					postfix += "in";
				}
				if (operators[operators.GetSize() - 1] == 'c') {
					postfix += "tg";
				}
				operators.pop_back();
			}
			if (operators.GetSize() > 0 && operators[operators.GetSize() - 1] == '(') {
				operators.pop_back();
			}
		}
		else {
			while (operators.GetSize() > 0 && precedence(c) <= precedence(operators[operators.GetSize() - 1])) {
				postfix += ' ';
				postfix += operators[operators.GetSize() - 1];
				if (operators[operators.GetSize() - 1] == 's') {
					postfix += "in";
				}
				if (operators[operators.GetSize() - 1] == 'c') {
					postfix += "tg";
				}
				operators.pop_back();
			}
			if (c != 's' && c != 'c') {
				postfix += ' ';
			}
			operators.push_back(c);
		}
	}

	while (operators.GetSize() > 0) {
		postfix += ' ';
		postfix += operators[operators.GetSize() - 1];
		operators.pop_back();
	}

	return postfix;
}

double evaluatePostfix(const string& expression) {
	setlocale(0, "");
	List<double> operands;

	for(int i = 0; i < expression.size(); i++){
		if (expression[i] == ' ') {
			continue; 
		}
		if (expression[i] == '-' && isdigit(expression[i + 1])) {
			int num = expression[i + 1] - '0';
			operands.push_back(-num);
			i = i + 2;
			continue;
		}
		if (isdigit(expression[i])) {
			if (expression[i + 1] == '.') {
				string numm;
				while (expression[i] != ' ') {
					numm += expression[i];
					i++;
				}
				locale::global(std::locale("en_US.UTF-8"));
				double num = std::stof(numm);
				operands.push_back(num);
				continue;
			}
			operands.push_back(expression[i] - '0');
		}
		else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '^') {
			double operand2 = operands[operands.GetSize() - 1];
			operands.pop_back();
			double operand1 = operands[operands.GetSize() - 1];
			operands.pop_back();
			double result = 0.0;

			switch (expression[i]) {
			case '+':
				result = operand1 + operand2;
				break;
			case '-':
				result = operand1 - operand2;
				break;
			case '*':
				result = operand1 * operand2;
				break;
			case '/':
				if (operand2 == 0) {
					return std::numeric_limits<float>::infinity();
				}
				result = operand1 / operand2;
				break;
			case '^':
				if (operand1 < 0 && (operand2 < 1 && operand2 > 0)) {
					cout << "Square root of a negative number" << endl;
					return std::numeric_limits<float>::infinity();
					break;
				}
				result = pow(operand1, operand2);
				break;
			}

			operands.push_back(result);
		}
		else if (expression[i] == 's') {
			double operand = operands[operands.GetSize() - 1];
			operands.pop_back();
			operands.push_back(sin(operand));
		}
		else if (expression[i] == 'c') {
			double operand = operands[operands.GetSize() - 1];
			operands.pop_back();
			if (PI / 2 == operand) {
				return std::numeric_limits<float>::infinity();
				break;
			}
			if (operand == 0) {
				return std::numeric_limits<float>::infinity();
				break;
			}
			operands.push_back(1.0 / tan(operand));
		}
	}
	return operands[operands.GetSize() - 1];
}

int main() {
	setlocale(0, "");
	string infix = "";
	read_file(infix);
	if (infix == "") {
		cout << "Пустая строка";
		return 0;
	}
	if (!valid_inf(infix)) {
		return 0;
	}
	cout << "Введённая строка с проверкой: " << infix << endl;
	
	string postfixExpression = infixToPostfix(infix);
	cout << "Постфиксная запись: " << postfixExpression << endl << endl;

	int xmin, xmax, h;
	cout << "Введите xmin: ";
	cin >> xmin;
	cout << "Введите xmax: ";
	cin >> xmax;
	cout << "Введите h: ";
	cin >> h;
	cout << endl;
	
	for (int i = xmin; i <= xmax; i = i + h) {
		string dubler = postfixExpression;
		for (int j = 0; j < postfixExpression.size(); j++) {
			if (postfixExpression[j] == 'x') {
				dubler.replace(j, 1, to_string(i));
			}
		}
		if (evaluatePostfix(dubler) == std::numeric_limits<float>::infinity()) {
			cout << "x = " << i << "   f(x) = " << "---" << endl;
		}
		else {
			cout << "x = " << i << "   f(x) = " << evaluatePostfix(dubler) << endl;
		}
		if (i + h > xmax && i < xmax) {
			for (int j = 0; j < postfixExpression.size(); j++) {
				if (postfixExpression[j] == 'x') {
					dubler.replace(j, 1, to_string(xmax));
				}
			}
			if (evaluatePostfix(dubler) == std::numeric_limits<float>::infinity()) {
				cout << "x = " << xmax << "   f(x) = " << "---" << endl;
			}
			else {
				cout << "x = " << xmax << "   f(x) = " << evaluatePostfix(dubler) << endl;
			}
		}
	}

	return 0;
}