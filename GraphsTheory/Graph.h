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
	// ������ ���������
	//vector<vector<int>> adjList;
	map<int, set<int>> adjList;
	// �������������� ����(��� � �����),
	// �� �����. � ��� ���������� -- 0 � ������ ������ �����.
	map<pair<int, int>, pair<int, string>> edgeChars;
	bool isOriented;
public:
	// ���������� ����� �������
	// n -- ���������� ����� ������ (�� �����. 1)
	void addNode(int n = 1);
	// ���������� ������ �����
	// x, y -- ������� ������, weight -- ���, mark -- �����
	void addEdge(int x, int y, int weight = 0, string mark = "");
	// �������� �������
	// x -- ������ �������
	void delNode(int x);
	// �������� �����
	// x, y -- ������� ������
	void delEdge(int x, int y);
	// �������� ���� ���� � ������ mark
	void delEdges(string mark);
	// ���������� ����� � ����
	// fileLoc -- ���� � �����
	void exportList(string fileLoc);
	// ����� ����� � �������
	void printList();

	void setAdjList(map<int, set<int>> adjList);

	map<pair<int, int>, pair<int, string>> getEdgeChars();
	map<int, set<int>> getAdjList();
	bool getIsOriented();

	Graph(bool isOriented);
	Graph(const Graph& g);
	// ������ ����� �� �����
	// fileLoc -- ���� � �����
	Graph(string fileLoc);
};

