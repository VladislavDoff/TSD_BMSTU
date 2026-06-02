#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
using namespace std;

const int INF = 1e9;
const double ALPHA = 1.0; // Важность феромонов
const double BETA = 2.0;  // Важность видимости
const double EVAPORATION = 0.5; // Коэффициент испарения феромона
const double Q = 100.0; // Количество феромона, отложенного муравьем
const int ANTS = 10; // Количество муравьев
const int ITERATIONS = 200; // Количество итераций

// Функция для вычисления общей длины маршрута
int calculatePathLength(const vector<int>& path, const vector<vector<int>>& graph) {
	int pathLength = 0;
	int n = path.size();
	for (int i = 0; i < n - 1; ++i) {
		if (graph[path[i]][path[i + 1]] == 0) { // Проверка наличия ребра между городами
			return INF;
		}
		pathLength += graph[path[i]][path[i + 1]];
	}
	if (graph[path[n - 1]][path[0]] != 0) { // Проверка наличия ребра между последним и первым городами
		pathLength += graph[path[n - 1]][path[0]];
		return pathLength;
	}
	return INF;
}

// Функция для поиска минимального пути методом полного перебора
void findOptimalPath(int n, vector<vector<int>>& graph) {
	vector<int> vertex(n);
	for (int i = 0; i < n; ++i) {
		vertex[i] = i;
	}

	int minPath = INF;
	vector<int> bestPath;

	do {
		int currentPath = calculatePathLength(vertex, graph);
		if (currentPath < minPath) {
			minPath = currentPath;
			bestPath = vertex;
		}
	} while (next_permutation(vertex.begin(), vertex.end()));

	if (minPath == INF) {
		cout << "Нет возможного маршрута" << endl;
	}
	else {
		cout << "Минимальный путь: " << minPath << endl;
		cout << "Лучший маршрут через полный перебор: ";
		for (int i = 0; i < n; ++i) {
			cout << bestPath[i] << " ";
		}
		cout << bestPath[0] << endl;
	}
}

// Функция для инициализации феромонов на ребрах
void initializePheromones(vector<vector<double>>& pheromones, int n) {
	pheromones.assign(n, vector<double>(n, 1.0)); // Начальное количество феромона
}

// Функция для обновления феромонов на ребрах
void updatePheromones(vector<vector<double>>& pheromones, const vector<vector<int>>& antsPaths) {
	int n = pheromones.size();
	// Испарение феромона на всех ребрах
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			pheromones[i][j] *= (1.0 - EVAPORATION);
		}
	}
	// Обновление феромона по маршрутам муравьев
	for (const auto& path : antsPaths) {
		int pathLength = calculatePathLength(path, antsPaths);
		for (int i = 0; i < n - 1; ++i) {
			pheromones[path[i]][path[i + 1]] += Q / pathLength;
		}
		pheromones[path[n - 1]][path[0]] += Q / pathLength;
	}
}

// Функция для выбора следующего города муравья
int selectNextCity(int currCity, const vector<vector<int>>& graph, const vector<double>& probabilities) {
	double sum = 0.0;
	for (size_t i = 0; i < graph.size(); ++i) {
		if (i != currCity && graph[currCity][i] != 0) {
			sum += probabilities[i];
		}
	}

	double randVal = static_cast<double>(rand()) / RAND_MAX;
	double cumulativeProb = 0.0;
	for (size_t i = 0; i < graph.size(); ++i) {
		if (i != currCity && graph[currCity][i] != 0) {
			cumulativeProb += probabilities[i] / sum;
			if (randVal <= cumulativeProb) {
				return i;
			}
		}
	}
	return -1;
}

// Функция для поиска маршрута муравьиной колонии
vector<int> antColonyOptimization(int n, vector<vector<int>>& graph, int maxDays) {
	vector<vector<double>> pheromones(n, vector<double>(n));
	initializePheromones(pheromones, n);

	vector<int> bestPath;
	int bestPathLength = INF;
	int days = 0;

	for (int iter = 0; iter < ITERATIONS && days < maxDays; ++iter) {
		vector<vector<int>> antsPaths(ANTS, vector<int>(n));
		for (int ant = 0; ant < ANTS; ++ant) {
			vector<int> visited(n, 0);
			vector<int> path(n);
			path[0] = rand() % n;
			visited[path[0]] = 1;

			for (int step = 1; step < n; ++step) {
				int currentCity = path[step - 1];
				vector<double> probabilities(n, 0.0);
				double probabilitiesSum = 0.0;

				for (int i = 0; i < n; ++i) {
					if (i != currentCity && visited[i] == 0 && graph[currentCity][i] != 0) {
						probabilities[i] = pow(pheromones[currentCity][i], ALPHA) * pow(1.0 / graph[currentCity][i], BETA);
						probabilitiesSum += probabilities[i];
					}
				}

				if (probabilitiesSum > 0.0) {
					for (int i = 0; i < n; ++i) {
						probabilities[i] /= probabilitiesSum;
					}
					int nextCity = selectNextCity(currentCity, graph, probabilities);
					path[step] = nextCity;
					visited[nextCity] = 1;
				}
			}
			antsPaths[ant] = path;
		}

		updatePheromones(pheromones, antsPaths);

		for (const auto& path : antsPaths) {
			int pathLength = calculatePathLength(path, graph);
			if (pathLength < bestPathLength) {
				bestPathLength = pathLength;
				bestPath = path;
			}
		}

		for (const auto& path : antsPaths) {
			int pathLength = calculatePathLength(path, graph);
			if (pathLength < bestPathLength) {
				bestPathLength = pathLength;
				bestPath = path;
			}
		}

		days++;
	}
	return bestPath;
}

int main() {
	setlocale(0, "");

	vector<vector<int>> graph = {
		{0, 0, 9, 0, 27, 0,  5, 20},
		{17, 0, 6, 0, 0, 2, 0, 0},
		{0, 14, 0, 3, 0, 7, 0, 8},
		{0, 0, 0, 0, 13, 0, 0, 32},
		{33, 0, 35, 0, 0, 38, 0, 0},
		{0, 0, 0, 0, 0, 0, 47, 0},
		{0, 50, 0, 52, 53, 0, 0, 56},
		{57, 0, 0, 0, 61, 0, 63, 0}
	};

	int maxDays;
	cout << "Введите максимальное количество суток на поиск муравьиным алгоритмом: ";
	cin >> maxDays;
	int n = graph.size();


	findOptimalPath(n, graph);
	cout << endl;

	srand(static_cast<unsigned int>(time(0)));
	vector<int> result = antColonyOptimization(n, graph, maxDays);

	cout << "Лучший маршрут через муравьиный алгоритм: ";
	for (int i = 0; i < n; ++i) {
		cout << result[i] << " ";
	}
	cout << result[0] << endl;

	return 0;
}

