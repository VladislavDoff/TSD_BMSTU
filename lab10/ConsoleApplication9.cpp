#include <iostream>
#include <vector>

using namespace std;

class LongTritNumber {
public:
	enum class Trit { False = 0, Unknown = 1, True = 2 };
	static const int Capacity = 9; // Увеличиваем емкость на 1 для хранения знака

	LongTritNumber() {
		trits.resize(Capacity, Trit::False);
	}

	const Trit& operator[](size_t index) const {
		return trits[index];
	}

	LongTritNumber& operator=(const LongTritNumber& other) {
		if (this != &other) {
			for (size_t i = 0; i < Capacity; ++i) {
				trits[i] = other.trits[i];
			}
		}
		return *this;
	}
		
	bool operator==(const LongTritNumber& other) const {
		if (trits[0] != other.trits[0]) {
			return false;
		}

		for (size_t i = 0; i < Capacity; ++i) {
			if (trits[i] != other.trits[i]) {
				return false;
			}
		}
		return true;
	}
		
	bool operator!=(const LongTritNumber& other) const {
		for (size_t i = 0; i < Capacity; ++i) {
			if (trits[i] != other.trits[i]) {
				return true;
			}
		}
		return false;
	}
		
	LongTritNumber operator+(const LongTritNumber& other) const {
		LongTritNumber result;
		Trit carry = Trit::False;

		for (int i = Capacity - 1; i >= 0; --i) {
			Trit sum = addTrits(trits[i], other.trits[i], carry);
			result.trits[i] = sum;
		}

		return result;
	}
		
	LongTritNumber operator-(const LongTritNumber& other) const {
		LongTritNumber result;
		Trit borrow = Trit::False;
		
		for (int i = Capacity - 1; i >= 0; --i) {
			Trit diff = subtractTrits(trits[i], other.trits[i], borrow);
			result.trits[i] = diff;
			borrow = calculateBorrow(trits[i], other.trits[i], borrow);
		}
		
		return result;
	}
		
	int operator<(const LongTritNumber& other) const {
		if (trits[0] != other.trits[0]) {
			return (trits[0] == Trit::True);
		}
		for (size_t i = 1; i < Capacity; ++i) {
			if (trits[i] < other.trits[i]) {
				return true;
			}
			else if (trits[i] > other.trits[i]) {
				return false;
			}
		}
		return 3;
	}

	int operator>(const LongTritNumber& other) const {
		if (trits[0] != other.trits[0]) {
			return (trits[0] == Trit::False);
		}
		for (size_t i = 1; i < Capacity; ++i) {
			if (trits[i] > other.trits[i]) {
				return true;
			}
			else if (trits[i] < other.trits[i]) {
				return false;
			}
		}
		return 3;
	}

	bool operator<=(const LongTritNumber& other) const {
		if (trits[0] != other.trits[0]) {
			return (trits[0] == Trit::True);
		}
		for (size_t i = 1; i < Capacity; ++i) {
			if (trits[i] <= other.trits[i]) {
				return true;
			}
			else if (trits[i] >= other.trits[i]) {
				return false;
			}
		}
		return false;
	}

	bool operator>=(const LongTritNumber& other) const {
		if (trits[0] != other.trits[0]) {
			return (trits[0] == Trit::False);
		}
		for (size_t i = 1; i < Capacity; ++i) {
			if (trits[i] >= other.trits[i]) {
				return true;
			}
			else if (trits[i] <= other.trits[i]) {
				return false;
			}
		}
		return false;
	}

	friend ostream& operator<<(ostream& os, const LongTritNumber& number) {
		if (number.trits[0] == Trit::True) {
			os << "Отрицательное число: ";
		}
		else {
			os << "Положительное число: ";
		}
		for (size_t i = 1; i < number.trits.size(); ++i) {
			switch (number.trits[i]) {
			case Trit::False:
				os << "False";
				break;
			case Trit::Unknown:
				os << "Unknown";
				break;
			case Trit::True:
				os << "True";
				break;
			}
			os << " ";
		}
		return os;
	}

	friend istream& operator>>(istream& is, LongTritNumber& number) {
		int sign = 0;
		cout << "Введите знак числа (0 - положительное / 1 - отрицательное): ";
		is >> sign;

		if (sign != 0 && sign != 1) {
			cout << "Некорректный ввод знака числа." << endl;
			return is;
		}

		number.trits[0] = (sign == 1) ? Trit::True : Trit::False;

		for (size_t i = 1; i < number.trits.size(); ++i) {
			int userInput;
			cout << "Введите значение трита (0 - False, 1 - Unknown, 2 - True): ";
			is >> userInput;

			if (userInput >= 0 && userInput <= 2) {
				number.trits[i] = static_cast<Trit>(userInput);
			}
			else {
				cout << "Некорректный ввод, используйте значения от 0 до 2." << endl;
				return is;
			}
		}
		return is;
	}

private:
		vector<Trit> trits;

	Trit addTrits(Trit a, Trit b, Trit& carry) const {
		int sum = static_cast<int>(a) + static_cast<int>(b) + static_cast<int>(carry);
		carry = static_cast<Trit>(sum / 3);
		return static_cast<Trit>(sum % 3);
	}

	Trit calculateCarry(Trit a, Trit b, Trit carry) const {
		int sum = static_cast<int>(a) + static_cast<int>(b) + static_cast<int>(carry);
		return static_cast<Trit>(sum / 3); // 1 если sum >= 3, иначе 0
	}

	Trit subtractTrits(Trit a, Trit b, Trit& borrow) const {
		int diff = static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(borrow);
		if (diff < 0) {
			diff += 3; 
			borrow = Trit::True;
		}
		else {
			borrow = Trit::False;
		}
		return static_cast<Trit>(diff);
	}

	Trit calculateBorrow(Trit a, Trit b, Trit borrow) const {
		int diff = static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(borrow);
		return (diff < 0) ? Trit::True : Trit::False;
	}
};

int main() {
	setlocale(0, "");
	LongTritNumber a, b;

	cout << "Введите числа:" << endl;
	cout << "Число a:" << endl;
	cin >> a;

	cout << "Число b:" << endl;
	cin >> b;

	LongTritNumber c = a + b;
	LongTritNumber d = a - b;

	cout << endl;
	cout << "Результат сложения: " << c << endl;
	cout << "Результат вычитания: " << d << endl;

	if (a == b) {
		cout << "a равно b" << endl;
	}
	else {
		cout << "a не равно b" << endl;
	}

	if (a != b) {
		cout << "a не равно b" << endl;
	}
	else {
		cout << "a равно b" << endl;
	}

	if (a < b) {
		if ((a < b) == 3) {
			cout << "Числа равны" << endl;

		}
		else {
			cout << "a меньше b" << endl;
		}
	}
	else {		
		cout << "a не меньше b" << endl;
	}

	if (a > b) {
		if ((a > b) == 3){
			cout << "Числа равны" << endl;
		}
		else {
			cout << "a больше b" << endl;
		}		
	}
	else {
		cout << "a не больше b" << endl;
	}

	if (a <= b) {
		cout << "a меньше или равно b" << endl;
	}
	else {
		cout << "a не меньше или равно b" << endl;
	}

	if (a >= b) {
		cout << "a больше или равно b" << endl;
	}
	else {
		cout << "a не больше или равно b" << endl;
	}

	return 0;
}
