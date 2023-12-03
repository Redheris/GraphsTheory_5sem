#include "Graph.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<string> split(string str, char sep) {
	vector<string> vec = vector<string>();
	int cnt = count(str.begin(), str.end(), sep) + 1;
	for (int i = 0; i < cnt; ++i) {
		size_t pos = str.find(sep);
		if (pos == string::npos) pos = str.length();
		vec.push_back(str.substr(0, pos));
		str.erase(0, pos + 1);
	}

	return vec;
}

Graph::Graph(bool isOrient) {
	this->isOriented = isOrient;
}

Graph::Graph(const Graph& g) {
	this->isOriented = g.isOriented;
	this->adjList = g.adjList;
}

Graph::Graph(string fileLoc) {
	ifstream in (fileLoc);

	map<pair<int, int>, pair<int, string>> readEdgeChars;

	in >> isOriented;

	while (!in.eof()) {
		string nodeInfo;
		
		getline(in, nodeInfo);
		
		if (nodeInfo == "")
			continue;

		int nodeNum = stoi(nodeInfo.substr(0, nodeInfo.find_first_of(':')).c_str());
		adjList.insert(make_pair(nodeNum, set<tuple<int, int, string>>()));
	
		vector<string> nodeEdges = split(nodeInfo.substr(nodeInfo.find('[') + 1, nodeInfo.find(']') - nodeInfo.find('[') - 1), ' ');

		if (nodeEdges[0] == "") continue;

		for (string s : nodeEdges) {
			size_t lBrack = s.find('(');
			size_t comma = (s.find(',') != string::npos ? s.find(',') : s.find(')'));

			int edgeEndNode = stoi(s.substr(0, lBrack));
			int weight = stoi(s.substr(lBrack + 1, comma - lBrack - 1));
			string mark = s.substr(lBrack + comma, s.find(')') - comma - 1);

			adjList.at(nodeNum).insert(make_tuple(edgeEndNode, weight, mark));
		}
		
	}
	

	in.close();
}

void Graph::setAdjList(map<int, set<tuple<int, int, string>>> adjList) {
	this->adjList = adjList;
}

void Graph::addNode(int n) {
	for (int i = 0; i < n; ++i)
		adjList[adjList.size()] = set<tuple<int, int, string>>();
}

void Graph::addEdge(int x, int y, int weight, string mark) {
	try {
		if (adjList.find(x) == adjList.end())
			throw out_of_range("Вершины " + to_string(x) + " не существует");
		if (adjList.find(y) == adjList.end())
			throw out_of_range("Вершины " + to_string(y) + " не существует");

		adjList.at(x).insert(make_tuple(y, weight, mark));
		if (!isOriented && x != y)
			adjList.at(y).insert(make_tuple(x, weight, mark));
	}
	catch (exception e) {
		cout << e.what() << endl;
		throw;
	}
}

void Graph::delNode(int x) {
	// Удаляем из спика саму вершину x
	try {
		if (adjList.find(x) == adjList.end())
			throw out_of_range("Вершины " + to_string(x) + " не существует");
		adjList.erase(x);
		// Обход всех вершин
		for (auto y : adjList) {
			// Обход всех рёбер из вершины
			for (auto nodeTo : y.second) {
				// Если найдено ребро, выходящее в вершину x - удаляем
				if (get<0>(nodeTo) == x)
				adjList.at(y.first).erase(nodeTo);
			}
		}
	}
	catch (out_of_range e) {
		cout << e.what() << endl;
		throw;
	}
}

void Graph::delEdge(int x, int y) {

	try {
		if (adjList.find(x) == adjList.end())
			throw out_of_range("Вершины " + to_string(x) + " не существует");
		if (adjList.find(y) == adjList.end())
			throw out_of_range("Вершины " + to_string(y) + " не существует");
		
		auto xAdjList = adjList.at(x);
		for (auto nodeTo : xAdjList)
			if (get<0>(nodeTo) == y)
				adjList.at(x).erase(nodeTo);
	}
	catch (exception e) {
		cout << e.what() << endl;
		throw;
	}
}

void Graph::delEdges(string mark) {
	auto tempAdjList = adjList;

	// Проход по всем вершинам
	for (auto x : tempAdjList) {
		// Проход по всем рёбрам из вершины
		auto xAdjList = tempAdjList.at(x.first);
		for (auto y : xAdjList)
			// Если метка совпадает - удаляем
			if (get<2>(y) == mark)
				adjList.at(x.first).erase(y);
	}
}

void Graph::printList() {
	cout << "isOriented: " << getIsOriented() << endl;
	for (auto node : getAdjList()) {
		int x = node.first;
		set<tuple<int, int, string>> adjList = getAdjList().at(x);
		cout << x << ": [ ";
		for (auto y : adjList) {
			cout << get<0>(y) << "(" << get<1>(y);
			if (get<2>(y) != "")
				cout << ", " << get<2>(y);
			cout << ") ";
		}
		cout << "]" << endl;
	}
}

void Graph::exportList(string fileLoc) {
	ofstream out = ofstream(fileLoc);
	
	out << this->isOriented << endl;
	for (auto node : adjList) {
		int x = node.first;

		out << x << ":[";

		for (auto y = adjList.at(x).begin(); y != adjList.at(x).end(); y++) {

			out << get<0>(*y) << "(" << get<1>(*y);
			if (get<2>(*y) != "")
				out << "," << get<2>(*y);
			out << ")";
			if (y != --(adjList.at(x).end()))
				out << " ";
		}

		out << "]";

		if (node != *adjList.rbegin())
			out << endl;
	}
}

map<int, set<tuple<int, int, string>>> Graph::getAdjList() {
	return adjList;
}

bool Graph::getIsOriented() {
	return isOriented;
};