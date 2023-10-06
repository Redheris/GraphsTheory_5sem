#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

#pragma once
class Graph
{
private:
	// Список смежности
	//vector<vector<int>> adjList;
	map<int, set<int>> adjList;
	// Характеристики рёбер(вес и метка),
	// по умолч. и при остутствии -- 0 и пустая строка соотв.
	map<pair<int, int>, pair<int, string>> edgeChars;
	bool isOriented;
public:
	// Добавление новой вершины
	// n -- количество новых вершин (по умолч. 1)
	void addNode(int n = 1);
	// Добавление нового ребра
	// x, y -- индексы вершин, weight -- вес, mark -- метка
	void addEdge(int x, int y, int weight = 0, string mark = "");
	// Удаление вершины
	// x -- индекс вершины
	void delNode(int x);
	// Удаление ребра
	// x, y -- индексы вершин
	void delEdge(int x, int y);
	// Удаление всех рёбер с меткой mark
	void delEdges(string mark);
	// Сохранение графа в файл
	// fileLoc -- путь к файлу
	void exportList(string fileLoc);
	// Вывод графа в консоль
	void printList();

	void setAdjList(map<int, set<int>> adjList);

	map<pair<int, int>, pair<int, string>> getEdgeChars();
	map<int, set<int>> getAdjList();
	bool getIsOriented();

	Graph(bool isOriented);
	Graph(const Graph& g);
	// Чтение графа из файла
	// fileLoc -- путь к файлу
	Graph(string fileLoc);
};

