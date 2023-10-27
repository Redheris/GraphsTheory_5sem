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
	// this->edgeChars = g.edgeChars;
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
			// pair<int, string> edgeChar = make_pair(stoi(s.substr(lBrack + 1, comma - lBrack - 1)), s.substr(lBrack + comma, s.find(')') - comma - 1));
			int weight = stoi(s.substr(lBrack + 1, comma - lBrack - 1));
			string mark = s.substr(lBrack + comma, s.find(')') - comma - 1);


			adjList.at(nodeNum).insert(make_tuple(edgeEndNode, weight, mark));
			//adjList.insert(make_pair(nodeNum, set<tuple<int, int, string>>(make_tuple(edgeEndNode, weight, mark))))

			// adjList.at(nodeNum).insert(edgeEndNode);
			// edgeChars.insert(make_pair(make_pair(nodeNum, edgeEndNode), make_pair(edgeChar.first, edgeChar.second)));
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
	adjList.at(x).insert(make_tuple(y, weight, mark));
	// edgeChars.insert(make_pair(make_pair(x, y), make_pair(weight, mark)));
	if (!isOriented && x != y) {
		adjList.at(y).insert(make_tuple(x, weight, mark));
		// edgeChars.insert(make_pair(make_pair(y, x), make_pair(weight, mark)));
	}
}

tuple<int, int, string> getEdgeTo(Graph &g, int from, int to) {
	for (tuple<int, int, string> y : g.getAdjList().at(from)) {
		if (get<0>(y) == to) return y;
	}
}

void Graph::delNode(int x) {

	// Обход всех вершин
	for (auto y : adjList) {
		// Обход всех рёбер из вершины
		for (auto nodeTo : y.second) {
			// Если найдено ребро, выходящее в вершину x - удаляем
			if (get<0>(nodeTo) == x)
			adjList.at(y.first).erase(nodeTo);
		}
	}
	// Удаляем из спика саму вершину x
	adjList.erase(x);


	// if (!isOriented) {
	// 	// for (auto y : adjList.at(x)) {
	// 	// 	adjList.at(y).erase(find(adjList.at(y).begin(), adjList.at(y).end(), x));
	// 	// 	adjList.at(y).erase()
	// 	// }
	// 	for (auto y : adjList) {
	// 		for (auto nodeTo : y.second) {
	// 			if (get<1>(nodeTo) == x)
	// 			adjList.at(y.first).erase(nodeTo);
	// 		}
			
	// 	}
	// }
	// else {
	// 	for (auto y : adjList) {
	// 		if (y.first != x)
	// 			y.second.erase(find(y.second.begin(), y.second.end(), x));
	// 	}
	// }
	// adjList.erase(x);
}

void Graph::delEdge(int x, int y) {

	// Проход по всем рёберам из вершины x
	for (auto nodeTo : adjList.at(x))
		// Если найдено ребро, выходящее в вершину y - удаляем
		if (get<0>(nodeTo) == y)
			adjList.at(x).erase(nodeTo);

	// // Удаление элемента со значением y из вектора x
	// adjList.at(x).erase(find(adjList.at(x).begin(), adjList.at(x).end(), y));
	// edgeChars.erase(make_pair(x, y));
	// // Зеркальное действие для неориентированного графа
	// if (!isOriented) {
	// 	adjList.at(y).erase(find(adjList.at(y).begin(), adjList.at(y).end(), x));
	// 	edgeChars.erase(make_pair(y, x));
	// }
}

void Graph::delEdges(string mark) {

	// Проход по всем вершинам
	for (auto x : adjList)
		// Проход по всем рёбрам из вершины
		for (auto y : x.second)
			// Если метка совпадает - удаляем
			if (get<2>(y) == mark)
				x.second.erase(y);

	// auto tempEdges = edgeChars;
	// for (auto edge : tempEdges) {
	// 	if (edge.second.second == mark) {
	// 		delEdge(edge.first.first, edge.first.second);
	// 		if (!isOriented)
	// 			tempEdges.erase(make_pair(edge.first.second, edge.first.first));
	// 	}
	// }
}

void Graph::printList() {
	cout << "isOriented: " << getIsOriented() << endl;
	for (auto node : getAdjList()) {
		int x = node.first;
		set<tuple<int, int, string>> adjList = getAdjList().at(x);
		cout << x << ": [ ";
		for (auto y : adjList) {
			// pair<int, string> edgeChar = edgeChars.at(make_pair(x, y));
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
			// auto edgeChar = edgeChars.at(make_pair(x, *y));

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

// map<pair<int, int>, pair<int, string>> Graph::getEdgeChars() {
// 	return edgeChars;
// }
map<int, set<tuple<int, int, string>>> Graph::getAdjList() {
	return adjList;
}
bool Graph::getIsOriented() {
	return isOriented;
};