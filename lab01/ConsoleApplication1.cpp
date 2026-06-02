#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

string RandomWord(int length) {
	static const char alphabet[] = "абвгдеёжзиклмнопрстуфхцчшщъыьэюя";
	string result;

	for (int i = 0; i < length; ++i) {
		int randomIndex = rand() % (sizeof(alphabet) - 1);
		result += alphabet[randomIndex];
	}

	return result;
}

int levenstein(string str_1, string str_2) {
	int n = str_1.length();
	int m = str_2.length();

	vector<int> current_row(n + 1);
	for (int i = 1; i <= n; i++) {
		current_row[i] = i;
	}

	for (int i = 1; i <= m; i++) {
		vector<int> previous_row = current_row;
		current_row[0] = i;
		for (int j = 1; j <= n; j++) {
			current_row[j] = min(previous_row[j] + 1, min(current_row[j - 1] + 1,
				(str_1[j - 1] != str_2[i - 1]) ? previous_row[j - 1] + 1 : previous_row[j - 1]));
		}
	}
	return current_row[n];
}

int main() {
    setlocale(0, "");
	string str1, str2;
	srand(time(0));
	int a, k, num;
	double medtime = 0;
	cout << "Ввести данные вручную (1) или сгенерировать (2) - ";
	cin >> a;
	if (a == 1) {
		cout << "Введите первое слово: ";
		cin >> str1;
		cout << "Введите второе слово: ";
		cin >> str2;
		clock_t start_time = clock();
		cout << "Расстояние левенштейна: " << levenstein(str1, str2) << endl;
		clock_t end_time = clock();
		double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
		cout << "Процессорное время: " << cpu_time << endl;
		cout << endl;
	}
	else {
		cout << "Введите длину слова: ";
		cin >> k;
		cout << "Количество слов: ";
		cin >> num;
		for (int i = 1; i <= num; i++) {
			cout << i << endl;
			str1 = RandomWord(k);
			str2 = RandomWord(k);
			cout << str1 << endl << str2 << endl;
			clock_t start_time = clock();
			cout << "Расстояние левенштейна: " << levenstein(str1, str2) << endl;
			clock_t end_time = clock();
			double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
			medtime += cpu_time;
			cout << "Процессорное время: " << cpu_time << endl;
			cout << endl;			
		}
		cout << "Среднее значение работы процессора: " << medtime / num;
	}
	return 0;
}











#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

string RandomWord(int length) {
	static const char alphabet[] = "абвгдеёжзиклмнопрстуфхцчшщъыьэюя";
	string result;

	for (int i = 0; i < length; ++i) {
		int randomIndex = rand() % (sizeof(alphabet) - 1);
		result += alphabet[randomIndex];
	}

	return result;
}

int DamerauLevenshteinDistance(const string& source, const string& target) {

	int n = source.length();
	int m = target.length();
	vector<vector<int>> distanceMatrix(n + 1, vector<int>(m + 1));

	// Инициализируем первую строку и первый столбец матрицы
	for (int i = 0; i <= n; ++i) {
		distanceMatrix[i][0] = i;
	}
	for (int j = 0; j <= m; ++j) {
		distanceMatrix[0][j] = j;
	}

	// Вычисляем расстояние Дамерау-Левенштейна
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {

			int substitutionCost = (source[i - 1] != target[j - 1]);

			int deletionCost = distanceMatrix[i - 1][j] + 1;
			int insertionCost = distanceMatrix[i][j - 1] + 1;
			int substitutionOrEqualityCost = distanceMatrix[i - 1][j - 1] + substitutionCost;

			//  транспозиция, если это возможно
			if (i > 1 && j > 1 && source[i - 1] == target[j - 2] && source[i - 2] == target[j - 1]) {
				distanceMatrix[i][j] = min({ deletionCost, insertionCost, substitutionOrEqualityCost, distanceMatrix[i - 2][j - 2] + substitutionCost });
			}
			else {
				distanceMatrix[i][j] = min({ deletionCost, insertionCost, substitutionOrEqualityCost });
			}
		}
	}

	for (int i = 0; i <= source.length(); ++i) {
		for (int j = 0; j <= target.length(); ++j) {
			cout << distanceMatrix[i][j] << " ";
		}
		cout << endl;
	}

	return distanceMatrix[n][m];
}

int main() {
	setlocale(0, "");
	string str1, str2;
	srand(time(0));
	int a, k, num;
	double medtime = 0;
	cout << "Ввести данные вручную (1) или сгенерировать (2) - ";
	cin >> a;
	if (a == 1) {
		cout << "Введите первое слово: ";
		cin >> str1;
		cout << "Введите второе слово: ";
		cin >> str2;
		clock_t start_time = clock();
		cout << "Расстояние Дамерау-Левенштейна: " << DamerauLevenshteinDistance(str1, str2) << endl;
		clock_t end_time = clock();
		double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
		cout << "Процессорное время: " << cpu_time << endl;
		cout << endl;
	}
	else {
		cout << "Введите длину слова: ";
		cin >> k;
		cout << "Количество слов: ";
		cin >> num;
		for (int i = 1; i <= num; i++) {
			cout << i << endl;
			str1 = RandomWord(k);
			str2 = RandomWord(k);
			cout << str1 << endl << str2 << endl;
			clock_t start_time = clock();
			cout << "Расстояние Дамерау-Левенштейна: " << DamerauLevenshteinDistance(str1, str2) << endl;
			clock_t end_time = clock();
			double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
			medtime += cpu_time;
			cout << "Процессорное время: " << cpu_time << endl;
			cout << endl;		
		}
		cout << "Среднее значение работы процессора: " << medtime / num;
	}
	return 0;
}








#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

string RandomWord(int length) {
	static const char alphabet[] = "абвгдеёжзиклмнопрстуфхцчшщъыьэюя";
	string result;

	for (int i = 0; i < length; ++i) {
		int randomIndex = rand() % (sizeof(alphabet) - 1);
		result += alphabet[randomIndex];
	}

	return result;
}

int damerauLevenshteinDistance(const string& str1, const string& str2, int i, int j,vector<vector<int>>& memo) {
	if (i == 0)
		return j;

	if (j == 0)
		return i;

	if (memo[i][j] != -1)
		return memo[i][j];

	int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

	int deletee = damerauLevenshteinDistance(str1, str2, i - 1, j, memo) + 1;
	int insert = damerauLevenshteinDistance(str1, str2, i, j - 1, memo) + 1;
	int substitution = damerauLevenshteinDistance(str1, str2, i - 1, j - 1, memo) + cost;

	int distance = std::min({ deletee, insert, substitution });

	if (i > 1 && j > 1 && str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1]) {
		int transposition = damerauLevenshteinDistance(str1, str2, i - 2, j - 2, memo) + cost;
		distance = std::min(distance, transposition);
	}

	memo[i][j] = distance;
	return distance;
}

int damerauLevenshteinDistance(const std::string& str1, const std::string& str2) {
	int m = str1.length();
	int n = str2.length();

	std::vector<std::vector<int>> memo(m + 1, std::vector<int>(n + 1, -1));

	return damerauLevenshteinDistance(str1, str2, m, n, memo);
}

int main() {
	setlocale(0, "");
	string str1, str2;
	srand(time(0));
	int a, k, num;
	double medtime = 0;
	cout << "Ввести данные вручную (1) или сгенерировать (2) - ";
	cin >> a;
	if (a == 1) {
		cout << "Введите первое слово: ";
		cin >> str1;
		cout << "Введите второе слово: ";
		cin >> str2;
		clock_t start_time = clock();
		cout << "Расстояние Дамерау-Левенштейна: " << damerauLevenshteinDistance(str1, str2) << endl;
		clock_t end_time = clock();
		double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
		cout << "Процессорное время: " << cpu_time << endl;
		cout << endl;
	}
	else {
		cout << "Введите длину слова: ";
		cin >> k;
		cout << "Количество слов: ";
		cin >> num;
		for (int i = 1; i <= num; i++) {
			cout << i << endl;
			str1 = RandomWord(k);
			str2 = RandomWord(k);
			cout << str1 << endl << str2 << endl;
			clock_t start_time = clock();
			cout << "Расстояние Дамерау-Левенштейна: " << damerauLevenshteinDistance(str1, str2) << endl;
			clock_t end_time = clock();
			double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
			medtime += cpu_time;
			cout << "Процессорное время: " << cpu_time << endl;
			cout << endl;
		}
		cout << "Среднее значение работы процессора: " << medtime / num;
	}



	return 0;
}


