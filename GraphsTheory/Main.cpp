#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include "ConsolePanel.h"

#include "Graph.h"


using namespace std;

void task1a1_connectedTo(Graph& g, int x) {
	cout << "task 1a1: ";
	// Проход по всем вершинам
	for (auto i : g.getAdjList()) {

		// Проход по всем рёбрам из вершины
		for (auto y : i.second)
			if (get<0>(y) == x)
				cout << i.first << " ";
	}
	cout << endl;
}

void task1a2_degreeOfVerts(Graph& g) {
	cout << "task 1b: ";
	vector<int> nodesDeg(g.getAdjList().size());
	for (auto i : g.getAdjList()) {
		nodesDeg[i.first] += i.second.size();
		for (auto j : i.second) {
			nodesDeg[get<0>(j)]++;
		}
	}

	for (int i = 0; i < nodesDeg.size(); ++i)
		cout << i << " : " << nodesDeg[i] << "\n";
	cout << endl;
}

Graph task1b_upToNotOrient(Graph& g) {
	
	if (!g.getIsOriented())
		return g;
	Graph gNew(g);
	auto tempEdges = g.getAdjList();

	// Проход по всем вершинам
	for (auto x : g.getAdjList())
		// Проход по всем рёбрам
		for (auto y : x.second)
			gNew.addEdge(x.first, get<0>(y), get<1>(y), get<2>(y));

	return gNew;
}

void task2_1_DFS(Graph& g, int v, bool*& used, vector<int>& res) {
	used[v] = true;
	auto vAdj = g.getAdjList().at(v);
	for (auto i : vAdj)
		if (!used[get<0>(i)])
			task2_1_DFS(g, get<0>(i), used, res);
	res.push_back(v);
}

Graph task2_1(Graph g) {
	bool* used = new bool[g.getAdjList().size()] {0};
	vector<int> res;
	map<int, set<tuple<int, int, string>>> oldAdj = g.getAdjList();
	map<int, set<tuple<int, int, string>>> resAdj;

	for (int i = 0; i < g.getAdjList().size(); ++i)
		if (!used[i])
			task2_1_DFS(g, i, used, res);

	reverse(res.begin(), res.end());

	// Обновление значений вершин в графе
	for (int i : res) {
		set<tuple<int, int, string>> tempAdj;

		for (auto y : oldAdj.at(i))
			tempAdj.insert(make_tuple(res[get<0>(y)], get<1>(y), get<2>(y)));
		resAdj[res[i]] = tempAdj;
	}

	g.setAdjList(resAdj);

	return g;
}

vector<bool> task2_2_BFS(Graph g, int v) {
	vector<bool> used(g.getAdjList().size(), false);
	queue<int> q;
	q.push(v);
	used[v] = true;

	while (!q.empty()) {
		int u = q.front();
		q.pop();
		auto adjList = g.getAdjList().at(v);
		for (auto y : adjList) {
			if (!used[get<0>(y)]) {
				q.push(get<0>(y));
				used[get<0>(y)] = true;
			}
		}
	}
	return used;
}


int task2_2_countConnectedComponents(Graph g) {
	vector<bool> used(g.getAdjList().size(), false);
	int count = 0;

	for (auto x : g.getAdjList()) {
		if (!used[x.first]) {
			used = task2_2_BFS(g, x.first);
			count++;
		}
	}

	return count;
}

// Функция сравнния длины двух рёбер вида (x, y, weight)
bool isShorter(tuple<int, int, int, string> e1, tuple<int, int, int, string> e2) {
	return get<2>(e1) < get<2>(e2);
}

Graph minOstovTree_Kruskal(Graph g) {
	auto oldAdjList = g.getAdjList();
	// Результирующий граф
	Graph newG(false);
	newG.addNode(oldAdjList.size());
	int edgeCount = 0;
	// Список компонент связности со списком вершин в ней
	map<int, set<int>> linkComps;
	// Список вершин с номером компоненты
	map<int, int> nodeComps;
	// Список рёбер: x, y, weight
	vector<tuple<int, int, int, string>> sortEdges;
	int compNum = 0;
	for (auto x : oldAdjList) {
		nodeComps[x.first] = compNum;
		linkComps[compNum++].insert(x.first);
		for (auto y : x.second)
			sortEdges.push_back(make_tuple(x.first, get<0>(y), get<1>(y), get<2>(y)));
	}
	// Сортировка рёбер в списке по неубыванию
	sort(sortEdges.begin(), sortEdges.end(), isShorter);

	for (auto edge : sortEdges) {
		if (edgeCount >= oldAdjList.size()) break;
		int x = get<0>(edge);
		if (nodeComps[x] != nodeComps[get<1>(edge)]) {
			// Максимльный и минимальный номера компоненты
			int maxComp = max(nodeComps[x], nodeComps[get<1>(edge)]);
			int minComp = min(nodeComps[x], nodeComps[get<1>(edge)]);
			// Добавление ребра в результат
			newG.addEdge(x, get<1>(edge), get<2>(edge), get<3>(edge));
			edgeCount++;
			// Объединение компонент
			for (int ns : linkComps[maxComp]) {
				nodeComps[ns] = minComp;
				linkComps[minComp].insert(ns);
			}
			// Отбрасывание компоненты после объединения
			linkComps.erase(maxComp);
		}
	}
	return newG;
}

map<int, int> shortestPath_Dijkstra(Graph g, int s) {
	map<int, int> pathsLen;
	vector<bool> used;
	auto adjList = g.getAdjList();
	for (auto v : adjList) {
		pathsLen[get<0>(v)] = MAXINT;
		used.push_back(false);
	}
	pathsLen[s] = 0;
	for (auto i : adjList) {
		int v = -1;
		for (auto j : adjList)
			if (!used[j.first] && (v == -1 || pathsLen[j.first] < pathsLen[v]))
				v = j.first;
		if (pathsLen[v] == MAXINT)
			return pathsLen;
		used[v] = true;
		for (auto e : adjList.at(v))
			if (pathsLen[v] + get<1>(e) < pathsLen[get<0>(e)])
				pathsLen[get<0>(e)] = pathsLen[v] + get<1>(e);
	}

	return pathsLen;
}

set<int> task4a_findGraphCenter(Graph g) {
	map<int, int> radiusMap; // Карта для хранения эксцентриситетов вершин
	int graphRadius = MAXINT; // Инициализация радиуса графа

	for (auto x : g.getAdjList()) {
		int sourceVertex = x.first;
		map<int, int> shortestPaths = shortestPath_Dijkstra(g, sourceVertex);

		// Находим эксцентриситет для каждой вершины
		int maxPath = 0;
		cout << "Кратчайшие пути из " << sourceVertex << ":\n";
		for (auto edge : shortestPaths) {
			cout << sourceVertex << " -> " << x.first << " := " << edge.second << endl;
			maxPath = max(maxPath, edge.second);
		}


		// Сохраняем эксцентриситет в карту
		radiusMap[sourceVertex] = maxPath;

		// Обновляем общий радиус графа
		graphRadius = min(graphRadius, maxPath);
	}

	for (auto x : radiusMap)
		cout << "Эксцентриситет в-ны " << x.first << " = " << x.second << endl;

	cout << "Радиус графа: " << graphRadius << endl;

	// Находим вершины с эксцентриситетом, равным радиусу графа
	set<int> centerVertices;
	for (auto x : radiusMap)
		if (x.second == graphRadius)
			centerVertices.insert(x.first);

	return centerVertices;
}

map<int, int> shortestPath_FordBellman(Graph g, int source)
{
	map<int, int> distance; // Карта для хранения длин кратчайших путей
	vector<int> vertices;   // Вектор для хранения вершин графа

	// Инициализация расстояний: из начальной вершины до всех остальных расстояние равно бесконечности
	for (auto const& entry : g.getAdjList())
	{
		int vertex = entry.first;
		distance[vertex] = MAXINT;
		vertices.push_back(vertex);
	}

	// Расстояние до начальной вершины устанавливаем равным 0
	distance[source] = 0;

	// Релаксация рёбер (|V| - 1) раз, где |V| - количество вершин в графе
	for (int i = 1; i < vertices.size(); ++i)
	{
		for (auto entry : g.getAdjList())
		{
			int u = entry.first;

			for (auto edge : entry.second)
			{
				int v = get<0>(edge);
				int weight = get<1>(edge);

				if (distance[u] != MAXINT && distance[u] + weight < distance[v])
				{
					distance[v] = distance[u] + weight;
				}
			}
		}
	}

	// Проверка наличия отрицательных циклов
	// Проход по всем вершинам
	for (auto entry : g.getAdjList())
	{
		int u = entry.first;
		// Проход по всем рёбрам от вершины u
		for (auto& edge : entry.second)
		{
			int v = get<0>(edge);		// Конец ребра
			int weight = get<1>(edge);	// Вес ребра

			// Проверка, что расстояние до точки u известно,
			// а расстояние до точки v изменено на значение текущего ребра,
			// то есть алгоритм попал в цикл
			if (distance[u] != MAXINT && distance[u] + weight < distance[v])
			{
				cerr << "Граф содержит отрицательный цикл" << endl;
				return map<int, int>();
			}
		}
	}

	return distance;
}


// Алгоритм Флойда
map<int, std::map<int, int>> floydWarshall(Graph g) {
	auto adjList = g.getAdjList();

	// Инициализация матрицы расстояний
	map<int, map<int, int>> dist;
	for (auto x : adjList)
		for (auto y : adjList)
			dist[x.first][y.first] = MAXINT;

	// Заполнение матрицы расстояний из списка смежности
	for (auto x : adjList) {
		for (auto edge : x.second) {
			int v = get<0>(edge);
			int weight = get<1>(edge);
			dist[x.first][v] = weight;
		}
		dist[x.first][x.first] = 0;  // Расстояние от вершины до самой себя равно 0
	}

	// Алгоритм Флойда
	for (auto m_entry : adjList) {
		int m = m_entry.first;
		for (auto x_entry : adjList) {
			int x = x_entry.first;
			for (auto y_entry : adjList) {
				int y = y_entry.first;
				// Проверка на бесконечность и обновление расстояния
				if (dist[x][m] != MAXINT && dist[m][y] != MAXINT)
					dist[x][y] = min(dist[x][y], dist[x][m] + dist[m][y]);
			}
		}
	}

	for (auto i_entry : adjList) {
		int i = i_entry.first;
		for (auto j_entry : adjList) {
			int j = j_entry.first;
			for (auto t_entry : adjList) {
				int t = t_entry.first;
				if (dist[i][t] < MAXINT && dist[t][t] < 0 && dist[t][j] < MAXINT)
					dist[i][j] = -MAXINT;
			}
		}
	}
	

	return dist;
}

// Внешняя функция для алгоритма Форда-Фалкерсона
int FordFulkerson(Graph& graph, int source, int sink) {
	// Создаем остаточную сеть и устанавливаем начальные значения
	Graph residualGraph = graph;
	int maxFlow = 0;

	// Пока существует увеличивающий путь
	vector<bool> augmentingPath;
	map<int, int> parent;

	augmentingPath = task2_2_BFS(residualGraph, source);
	while (!augmentingPath.empty()) {
		// Найдем минимальную пропускную способность ребер на пути
		int minCapacity = MAXINT;

		// Увеличим поток вдоль пути на минимальную пропускную способность
		maxFlow += minCapacity;
		// Сбросим структуры данных для поиска следующего увеличивающего пути
		parent.clear();

		// Повторно получаем увеличивающий путь
		augmentingPath = task2_2_BFS(residualGraph, source);
	}

	// Вернем максимальный поток
	return maxFlow;
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
	//setlocale(LC_ALL, "RUS");

	string path = "./test1.txt";

	/*g.exportList("./test1.txt");
	g.delNode(2);
	g.printList();*/

	/*Graph gNew = Graph((char*)"./test1.txt");
	gNew.printList();*/

	Graph g = Graph(true);
	g.addNode(4);
	g.addEdge(0, 2, 0, "test");
	g.addEdge(1, 2);
	g.addEdge(3, 3);
	g.addEdge(1, 3);
	g.addEdge(1, 3);
	g.addEdge(2, 2);
	g.printList();

	//g.exportList(path);

	Graph gImport = Graph(path);
	//gImport.printList();

	/*gImport.exportList("./test2.txt");

	Graph f = Graph("./test2.txt");
	f.printList();*/

	Graph task1 = Graph(true);
	task1.addNode(7);
	task1.addEdge(0, 1);
	task1.addEdge(0, 0);
	task1.addEdge(1, 2);
	task1.addEdge(2, 3);
	task1.addEdge(2, 4);
	task1.addEdge(3, 4);
	task1.addEdge(2, 1);
	task1.addEdge(4, 2);
	task1.addEdge(4, 5);
	task1.addEdge(5, 4);

	task1.printList();

	task1a1_connectedTo(task1, 1);
	task1a2_degreeOfVerts(task1);

	cout << "\nTask 1b:\n";
	task1.printList();
	cout << "\nResult 1b:\n";
	cout << "task 1a2:\n";
	task1b_upToNotOrient(task1).printList();

	//============================================================================

	Graph task21(true);
	task21.addNode(4);
	task21.addEdge(0, 3);
	task21.addEdge(3, 1);
	task21.addEdge(3, 2);
	task21.addEdge(2, 1);

	cout << "\nTask 5.2_1\n";

	cout << "\nИсходный граф:\n";
	task21.printList();

	Graph task21Result = task2_1(task21);

	cout << "\nИсходный граф:\n";
	task21Result.printList();

	//============================================================================

	cout << "\nTask 6.2_2\n";

	Graph task22(false);
	task22.addNode(6);
	task22.addEdge(0,1);
	task22.addEdge(0,2);
	task22.addEdge(1,2);
	task22.addEdge(3,4);

	Graph task22_2(false);
	task22_2.addNode(3);
	task22_2.addEdge(0,1);
	task22_2.addEdge(0,2);
	task22_2.addEdge(1,2);

	cout << "\nКоличество связных компонент task22: " <<
		task2_2_countConnectedComponents(task22);

	cout << "\nGraph task21:\n"; task21.printList();

	cout << "\nКоличество связных компонент task21: " <<
		task2_2_countConnectedComponents(task1b_upToNotOrient(task21));

	cout << "\nGraph task22_2:\n"; task22_2.printList();

	cout << "\nКоличество связных компонент task21: " <<
		task2_2_countConnectedComponents(task1b_upToNotOrient(task22_2)) << endl;

	//============================================================================

	// Алгоритм Краскала работает только со связными неориентированными графами
	Graph task3(false);
	task3.addNode(7);
	task3.addEdge(0, 1, 7);
	task3.addEdge(0, 3, 5);
	task3.addEdge(1, 2, 8);
	task3.addEdge(1, 3, 9);
	task3.addEdge(1, 4, 7);
	task3.addEdge(3, 4, 15);
	task3.addEdge(4, 6, 9);
	task3.addEdge(4, 2, 5);
	task3.addEdge(5, 3, 6);
	task3.addEdge(5, 4, 8);
	task3.addEdge(5, 6, 11);

	minOstovTree_Kruskal(task3).printList();
	
	//============================================================================

	cout << "\nGraph task4:\n";

	Graph task4(true);
	task4.addNode(7);
	task4.addEdge(0, 1, 7);
	task4.addEdge(0, 2, 14);
	task4.addEdge(0, 5, 7);
	task4.addEdge(1, 3, 4);
	task4.addEdge(1, 6, 10);
	task4.addEdge(2, 3, 4);
	task4.addEdge(3, 2, 4);
	task4.addEdge(3, 4, 4);
	task4.addEdge(3, 6, 15);
	task4.addEdge(5, 2, 2);
	task4.addEdge(5, 4, 9);

	task4.printList();

	// Пример использования
	Graph task4a(false);

	// Создаем звезду с центром в вершине 1
	task4a.addNode(4);
	task4a.addEdge(0, 1, 3);
	task4a.addEdge(0, 3, 15);
	task4a.addEdge(1, 3, 4);
	task4a.addEdge(2, 3, 7);

	cout << "\nЗадание 8 (IVa)\n\n";

	// Вывод всех кратчайших путей из 0 в графе task4
	map<int,int> shortPaths = shortestPath_Dijkstra(task4, 0);
	cout << "Кратчайшие пути из 0:\n";
	for (pair<int, int> x : shortPaths)
		cout << 0 << " -> " << x.first << " := " << x.second << endl;
	cout << "----------------\n";

	// Вывод всех кратчайших путей из 0 в графе task4a
	cout << "Кратчайшие пути из 0:\n";
	shortPaths = shortestPath_Dijkstra(task4a, 0);
	for (pair<int, int> x : shortPaths)
		cout << 0 << " -> " << x.first << " := " << x.second << endl;
	cout << endl;
	
	// Вывод центра графа
	set<int> center = task4a_findGraphCenter(task4a);
	cout << "Центр графа: [ ";
	for (int x : center) cout << x << " ";
	cout << "]" << endl;

	task4a.addEdge(0, 2, 7);
	// Вывод центра графа
	center = task4a_findGraphCenter(task4a);
	cout << "Центр графа: [ ";
	for (int x : center) cout << x << " ";
	cout << "]" << endl;

	cout << "----------------\n";

	//============================================================================

	cout << "\nЗадание 9 (IVb)\n\n";

	Graph task4c(true);
	task4c.addNode(6);
	task4c.addEdge(0, 1, 2);
	task4c.addEdge(1, 2, 3);
	task4c.addEdge(2, 3, 1);
	task4c.addEdge(3, 4, 4);
	task4c.addEdge(4, 5, 2);
	task4c.addEdge(5, 0, 5);
	task4c.addEdge(2, 0, -1);

	task4c.printList();
	
	int u  = 0,
		v1 = 3,
		v2 = 4;
	cout << "Переменные из условия задания: u = " << u << ", v1 = " << v1 << ", v2 = " << v2 << endl;

	shortPaths = shortestPath_FordBellman(task4c, 0);
	cout << "Кратчайшие пути из " << u << ":\n";
	for (pair<int, int> x : shortPaths)
		cout << 0 << " -> " << x.first << " := " << x.second << endl;

	cout << "\nДлины кратчайших путей из " << u << " до v1 и v2:" << endl;
	cout << u << " -> " << v1 << " := " << shortPaths[v1] << endl;
	cout << u << " -> " << v2 << " := " << shortPaths[v2] << endl;

	//============================================================================

	cout << "\nЗадание 10 (IVc)\n\n";

	Graph task4c1(true);
	task4c1.addNode(8);

	task4c1.addEdge(0, 1, 2);
	task4c1.addEdge(1, 2, 3);
	task4c1.addEdge(2, 7, 3);
	task4c1.addEdge(2, 5, 2);
	task4c1.addEdge(3, 4, 4);
	task4c1.addEdge(3, 6, 0);
	task4c1.addEdge(4, 7, -6);
	task4c1.addEdge(5, 0, 5);
	task4c1.addEdge(7, 3, 1);

	// Вывод графа в консоль
	task4c1.printList();

	set<pair<int, int>> mininf_paths; // Структура для сохранения бесконено малых путей

	cout << "\nМатрица кратчайших путей:\n";

	// Применение алгоритма Флойда
	map<int, map<int, int>> allPairsShortestPaths = floydWarshall(task4c1);
	// Названия столбцов
	cout << " ";
	for (auto x : allPairsShortestPaths)
		cout << setw(7) << x.first;
	// Вывод строк матрицы
	for (auto x : allPairsShortestPaths) {
		cout << "\n\n";
		// Названия строки
		cout << x.first;
		// Веса путей
		for (auto y : x.second) {
			if (y.second == MAXINT)
				cout << setw(7) << "-" << setw(3);
			else if (y.second == -MAXINT) {
				cout << setw(7) << "-INF" << setw(3);
				// Добавление пути в список бесконечно малых
				mininf_paths.insert(make_pair(x.first, y.first));
			}
			else
				cout << setw(7) << y.second << setw(3);
		}
	}

	cout << "\n\nПары вершин, между которыми существует путь бесконечно малого веса:\n";
	for (auto p : mininf_paths)
		cout << "(" << p.first << " " << p.second << "), ";
	cout << endl;

	//============================================================================

	cout << "\nЗадание 11 (V)\n\n";

	Graph task5(true);  // Ориентированный граф
	task5.addNode(7);
	// Добавление рёбер
	task5.addEdge(0, 1);
	task5.addEdge(1, 2);
	task5.addEdge(2, 3);
	task5.addEdge(0, 4);
	task5.addEdge(4, 5);
	task5.addEdge(5, 2);
	task5.addEdge(5, 6);

	int source = 0;
	int sink = 5;

	/*int maxFlow = FordFulkerson(task10, source, sink);
	cout << "Максимальный поток: " << maxFlow << endl;*/

	setupPanel();
}

