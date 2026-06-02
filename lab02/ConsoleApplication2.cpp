#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void output_krm(vector<vector<int>>& matr, string txt, int &row, int &col) {
	string filename = "C:\\Users\\Kononova\\Desktop\\мгту им Н.Э.Баумана\\3 семестр\\Типы и структуры данных\\лаб_2\\" + txt + ".txt";
	ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		cout << "Не удалось открыть файл" << endl;
	}

	int rows_1 = 0;
	int cols_1 = 0;
	string line_1;
	while (getline(inputFile, line_1)) {
		vector<int> row_1;
		istringstream iss(line_1);
		int value;

		while (iss >> value) {
			row_1.push_back(value);
		}

		if (row_1.size() > 0) {
			matr.push_back(row_1);

			if (row_1.size() > cols_1) {
				cols_1 = row_1.size();
			}

			++rows_1;
		}
	}
	row = rows_1;
	col = cols_1;
	inputFile.close();
}

void djennings(vector<vector<int>>&matrix, vector<int> &AN, vector<int> &D) {
	for (int i = 0; i < matrix.size(); i++) {
		bool flagg = false;
		for (int j = 0; j <= i; j++) {
			if (matrix[i][j] != 0) {
				flagg = true;
			}
			if (flagg == true || i == j) {
				AN.push_back(matrix[i][j]);
				if (i == j) D.push_back(AN.size());
			}
		}
	}
}

void print_matrix(vector<vector<int>>& matrix) {
	for (const auto &row : matrix) {
		for (int num : row) {
			cout << num << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void inputt_file(const vector<vector<int>>& matr, string txt) {
	string filename = "C:\\Users\\Kononova\\Desktop\\intput" + txt + ".txt";
	ofstream outputFile(filename);

	if (outputFile.is_open()) {
		for (const vector<int>& row : matr) {
			for (int value : row) {
				outputFile << value << ' ';
			}
			outputFile << '\n';
		}

		outputFile.close();
		cout << "Данные успешно записаны в файл 'intput" + txt + ".txt'" << std::endl;
	}
	else {
		cerr << "Ошибка открытия файла для записи" << std::endl;
	}
}

void compression_krm(vector<vector<int>>&matrix, vector<int> &AN_krm, vector<int> &NR_krm, vector<int> &NC_krm, vector<int> &JR_krm, vector<int> &JC_krm, int &row, int &col) {
	int counter = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (matrix[i][j] != 0) {
				if (JR_krm[i] == -1) {
					JR_krm[i] = counter;
					NR_krm.push_back(counter);
				}
				else {
					int k = JR_krm[i];
					while (NR_krm[k] != JR_krm[i]) {
						k = NR_krm[k];
					}
					NR_krm[k] = counter;
					NR_krm.push_back(JR_krm[i]);
				}
				if (JC_krm[j] == -1) {
					JC_krm[j] = counter;
					NC_krm.push_back(counter);
				}
				else {
					int k = JC_krm[j];
					while (NC_krm[k] != JC_krm[j]) {
						k = NC_krm[k];
					}
					NC_krm[k] = counter;
					NC_krm.push_back(JC_krm[j]);
				}
				AN_krm.push_back(matrix[i][j]);
				counter++;
			}
		}
	}
}

int num_col_el(vector<int>&an, vector<int>&nc, vector<int>&jc, int coord) {
	for (int i = 0; i < jc.size(); i++) {
		if (jc[i] == coord) {
			return i;
		}
	}
	while (true) {
		if (coord > nc.size() - 1) {
			return 1;
		}
		coord = nc[coord];
		for (int i = 0; i < jc.size(); i++) {
			if (jc[i] == coord) {
				return i;
			}
		}
	}
}

int num_row_el(vector<int>&an, vector<int>&nr, vector<int>&jr, int coord) {
	for (int i = 0; i < jr.size(); i++) {
		if (jr[i] == coord) {
			return i;
		}
	}
	while (true) {
		if (coord > nr.size() - 1) {
			return 1;
		}
		coord = nr[coord];
		for (int i = 0; i < jr.size(); i++) {
			if (jr[i] == coord) {
				return i;
			}
		}
	}
}

void unpack_ring_RM(vector<vector<int>>&a, vector<int>&an, vector<int>&nr, vector<int>&nc, vector<int>&jr, vector<int>&jc) {
	for (int i = 0; i < an.size(); i++) {
		int row = num_row_el(an, nr, jr, i);
		int col = num_col_el(an, nc, jc, i);
		a[row][col] = an[i];
	}
}

void print_arrow(vector<int>&arr) {
	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}


void summ_krm(vector<vector<int>>&matrix1, vector<vector<int>>&matrix2, vector<int> &AN_krm, vector<int> &NR_krm, vector<int> &NC_krm, vector<int> &JR_krm, vector<int> &JC_krm, int &row_1, int &col_1, int &row_2, int &col_2) {
	if (row_1 == row_2 && col_1 == col_2) {
		int counter = 0;
		for (int i = 0; i < row_1; i++) {
			for (int j = 0; j < col_1; j++) {
				if (matrix1[i][j] + matrix2[i][j] != 0) {
					if (JR_krm[i] == -1) {
						JR_krm[i] = counter;
						NR_krm.push_back(counter);
					}
					else {
						int k = JR_krm[i];
						while (NR_krm[k] != JR_krm[i]) {
							k = NR_krm[k];
						}
						NR_krm[k] = counter;
						NR_krm.push_back(JR_krm[i]);
					}
					if (JC_krm[j] == -1) {
						JC_krm[j] = counter;
						NC_krm.push_back(counter);
					}
					else {
						int k = JC_krm[j];
						while (NC_krm[k] != JC_krm[j]) {
							k = NC_krm[k];
						}
						NC_krm[k] = counter;
						NC_krm.push_back(JC_krm[j]);
					}
					AN_krm.push_back(matrix1[i][j] + matrix2[i][j]);
					counter++;
				}
			}
		}
		vector<vector<int>> matrix_krm_summ(row_1, vector<int>(col_1));
		unpack_ring_RM(matrix_krm_summ, AN_krm, NR_krm, NC_krm, JR_krm, JC_krm);
		cout << "Матрица суммы" << endl;
		print_matrix(matrix_krm_summ);
		cout << "AN суммы матриц: ";
		print_arrow(AN_krm);
		cout << "NR суммы матриц: ";
		print_arrow(NR_krm);
		cout << "NC суммы матриц: ";
		print_arrow(NC_krm);
		cout << "JR суммы матриц: ";
		print_arrow(JR_krm);
		cout << "JC суммы матриц: ";
		print_arrow(JC_krm);
		cout << endl << endl;
		inputt_file(matrix_krm_summ, "2");
	}
	else {
		cout << "Невозможно сложить" << endl;
	}
}

void multiplic_krm(vector<vector<int>>&matrix1, vector<vector<int>>&matrix2, vector<int> &AN_krm, vector<int> &NR_krm, vector<int> &NC_krm, vector<int> &JR_krm, vector<int> &JC_krm, int &row_1, int &col_1, int &row_2, int &col_2) {
	if (row_1 == col_2 || row_2 == col_1) {
		vector<vector<int>>mult(row_1, vector<int>(col_2));
		for (int i = 0; i < row_1; i++) {
			for (int j = 0; j < col_2; j++) {
				for (int k = 0; k < col_1; k++) {
					mult[i][j] += matrix1[i][k] * matrix2[k][j];
				}
			}
		}
		compression_krm(mult, AN_krm, NR_krm, NC_krm, JR_krm, JC_krm, row_1, col_2);
		vector<vector<int>> matrix_krm(row_1, vector<int>(col_2));
		unpack_ring_RM(matrix_krm, AN_krm, NR_krm, NC_krm, JR_krm, JC_krm);
		cout << "Матрица произведения" << endl;
		print_matrix(matrix_krm);
		cout << "AN произведения матриц: ";
		print_arrow(AN_krm);
		cout << "NR произведения матриц: ";
		print_arrow(NR_krm);
		cout << "NC произведения матриц: ";
		print_arrow(NC_krm);
		cout << "JR произведения матриц: ";
		print_arrow(JR_krm);
		cout << "JC произведения матриц: ";
		print_arrow(JC_krm);
		inputt_file(matrix_krm, "3");
	}
	else {
		cout << "Умножение невозможно" << endl;
	}
}


int main() {
	setlocale(0, "");
	int choose = 0;
	cout << "Алгоритм Дженнингса - 1, кольцевая схема - 2: ";
	cin >> choose;
	if (choose == 1) {
		vector<vector<int>> matrix_1;
		int row_1 = 0, col_1 = 0;
		output_krm(matrix_1, "matr_1", row_1, col_1);
		cout << "Первая матрица" << endl;
		print_matrix(matrix_1);
		vector<int> AN_1;
		vector<int> D_1;
		djennings(matrix_1, AN_1, D_1);
		cout << "AN первой матрицы: ";
		print_arrow(AN_1);
		cout << "D первой матрицы: ";
		print_arrow(D_1);
		cout << endl;

		vector<vector<int>> matrix_2;
		int row_2 = 0, col_2 = 0;
		output_krm(matrix_2, "matr_2", row_2, col_2);
		cout << "Вторая матрица" << endl;
		print_matrix(matrix_2);
		vector<int> AN_2;
		vector<int> D_2;
		djennings(matrix_2, AN_2, D_2);
		cout << "AN второй матрицы: ";
		print_arrow(AN_2);
		cout << "AN второй матрицы: ";
		print_arrow(D_2);
		cout << endl;

		vector<int> summ_matr;
		vector<int> D_summ_matr;
		if (D_1.size() == D_2.size()) {
			for (int i = 0; i < D_1.size(); i++) {
				if (i == 0) {
					summ_matr.push_back(AN_1[i] + AN_2[i]);
					D_summ_matr.push_back(1);
				}
				else {
					int razn_1 = D_1[i] - D_1[i - 1];
					int razn_2 = D_2[i] - D_2[i - 1];
					if (razn_1 < razn_2) {
						for (int k = 0; k < razn_2 - razn_1; k++) {
							summ_matr.push_back(AN_2[D_2[i - 1] + k]);
						}
						if (razn_1 > 2) {
							for (int h = 0; h < razn_1 - 1; h++) {
								int check = AN_1[D_1[i - 1] + h] + AN_2[D_1[i - 1] + h];
								if (check != 0) summ_matr.push_back(check);
							}
						}
					}
					if (razn_1 > razn_2) {
						for (int k = 0; k < razn_1 - razn_2; k++) {
							summ_matr.push_back(AN_1[D_1[i - 1] + k]);
						}
						if (razn_2 > 2) {
							for (int h = 0; h < razn_2 - 1; h++) {
								int check = AN_1[D_2[i - 1] + h] + AN_2[D_2[i - 1] + h];
								if (check != 0) summ_matr.push_back(check);
							}
						}
					}
					summ_matr.push_back(AN_1[D_1[i] - 1] + AN_2[D_2[i] - 1]);
					D_summ_matr.push_back(summ_matr.size());
				}
			}
			vector<vector<int>> matrix(D_summ_matr.size(), vector<int>(D_summ_matr.size()));
			for (int i = 0; i < D_summ_matr.size(); i++) {
				matrix[i][i] = summ_matr[D_summ_matr[i] - 1];
				if (i != 0) {
					int null_count = 0;
					int h = D_summ_matr[i - 1] + 1;
					while (h < D_summ_matr[i]) {
						matrix[null_count][i] = summ_matr[D_summ_matr[i - 1] + null_count];
						matrix[i][null_count] = summ_matr[D_summ_matr[i - 1] + null_count];
						null_count++;
						h++;
					}
				}
			}
			cout << "Сумма матриц: " << endl;
			print_matrix(matrix);
			cout << "AN суммы матриц: ";
			print_arrow(summ_matr);
			cout << "D суммы матриц";
			print_arrow(D_summ_matr);
			cout << endl;
			inputt_file(matrix, "1");
			cout << endl;
		}
		else {
			cout << "Сложение матриц невозможно" << endl;
		}
	}
	else {
		vector<vector<int>> matrix_3;
		int row_3 = 0, col_3 = 0;
		output_krm(matrix_3, "test1", row_3, col_3);
		cout << "Первая матрица" << endl;
		print_matrix(matrix_3);
		vector<int> AN_krm_1;
		vector<int> NR_krm_1;
		vector<int> NC_krm_1;
		vector<int> JR_krm_1(row_3, -1);
		vector<int> JC_krm_1(col_3, -1);
		compression_krm(matrix_3, AN_krm_1, NR_krm_1, NC_krm_1, JR_krm_1, JC_krm_1, row_3, col_3);
		cout << "AN первой матрицы: ";
		print_arrow(AN_krm_1);
		cout << "NR первой матрицы: ";
		print_arrow(NR_krm_1);
		cout << "NC первой матрицы: ";
		print_arrow(NC_krm_1);
		cout << "JR первой матрицы: ";
		print_arrow(JR_krm_1);
		cout << "JC первой матрицы: ";
		print_arrow(JC_krm_1);
		cout << endl << endl;

		vector<vector<int>> matrix_4;
		int row_4 = 0, col_4 = 0;
		output_krm(matrix_4, "test2", row_4, col_4);
		cout << "Вторая матрица" << endl;
		print_matrix(matrix_4);
		vector<int> AN_krm_2;
		vector<int> NR_krm_2;
		vector<int> NC_krm_2;
		vector<int> JR_krm_2(row_4, -1);
		vector<int> JC_krm_2(col_4, -1);
		compression_krm(matrix_4, AN_krm_2, NR_krm_2, NC_krm_2, JR_krm_2, JC_krm_2, row_4, col_4);
		cout << "AN второй матрицы: ";
		print_arrow(AN_krm_2);
		cout << "NR второй матрицы: ";
		print_arrow(NR_krm_2);
		cout << "NC второй матрицы: ";
		print_arrow(NC_krm_2);
		cout << "JR второй матрицы: ";
		print_arrow(JR_krm_2);
		cout << "JC второй матрицы: ";
		print_arrow(JC_krm_2);
		cout << endl << endl;

		vector<int> AN_krm_sum;
		vector<int> NR_krm_sum;
		vector<int> NC_krm_sum;
		vector<int> JR_krm_sum(row_3, -1);
		vector<int> JC_krm_sum(col_3, -1);
		summ_krm(matrix_3, matrix_4, AN_krm_sum, NR_krm_sum, NC_krm_sum, JR_krm_sum, JC_krm_sum, row_3, col_3, row_4, col_4);	
		

		vector<int> AN_krm_mult;
		vector<int> NR_krm_mult;
		vector<int> NC_krm_mult;
		vector<int> JR_krm_mult(row_3, -1);
		vector<int> JC_krm_mult(col_4, -1);
		multiplic_krm(matrix_3, matrix_4, AN_krm_mult, NR_krm_mult, NC_krm_mult, JR_krm_mult, JC_krm_mult, row_3, col_3, row_4, col_4);
		
	}
	return 0;
}