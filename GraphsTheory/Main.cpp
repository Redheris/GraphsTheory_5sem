#include <vector>
#include <iostream>
#include <string>
#include "ConsolePanel.h"

#include "Graph.h"


using namespace std;

void task1a1(Graph& g, int x) {
	cout << "task 1a1: ";
	// Проход по всем вершинам
	for (auto i : g.getAdjList()) {
		// if (i.second.find(x) != i.second.end())
		// 	cout << i.first << " ";

		// Проход по всем рёбрам из вершины
		for (auto y : i.second)
			if (get<0>(y) == x)
				cout << i.first << " ";
	}
	cout << endl;
}

void task1a2(Graph& g) {
	cout << "task 1b: ";
	vector<int> nodesDeg(g.getAdjList().size());
	for (auto i : g.getAdjList()) {
		nodesDeg[i.first] += i.second.size();
		for (auto j : i.second) {
			nodesDeg[get<0>(j)]++;
		}
	}
	
	for(int i = 0; i < nodesDeg.size(); ++i)
		cout << i << " : " << nodesDeg[i] << "\n";
	cout << endl;
}

Graph task1b(Graph& g) {
	cout << "task 1a2:\n";
	if (!g.getIsOriented())
		return g;
	Graph gNew(g);
	auto tempEdges = g.getEdgeChars();
	for (auto edge : g.getEdgeChars()) {
		gNew.addEdge(edge.first.second, edge.first.first, edge.second.first, edge.second.second);
	}

	return gNew;
}

void task2_1_DFS(Graph& g, int v, bool*& used, vector<int> &res) {
	used[v] = true;
	auto vAdj = g.getAdjList().at(v);
	for (auto i : vAdj)
		if (!used[i])
			task2_1_DFS(g, i, used, res);
	res.push_back(v);
}

Graph task2_1(Graph g) {
	bool* used = new bool[g.getAdjList().size()] {0};
	vector<int> res;
	map<int, set<int>> oldAdj = g.getAdjList();
	map<int, set<int>> resAdj;

	for (int i = 0; i < g.getAdjList().size(); ++i)
		if (!used[i])
			task2_1_DFS(g, i, used, res);

	reverse(res.begin(), res.end());


	for (int i = 0; i < res.size(); ++i) {
		set<int> tempAdj;
		for (auto y : oldAdj[i])
			tempAdj.insert(res[y]);
		resAdj[res[i]] = tempAdj;
		/*resAdj.insert(make_pair(res[i], oldAdj[i]));*/
	}

	g.setAdjList(resAdj);

	return g;
}

int main() {
	setlocale(LC_ALL, "RUS");

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
	task1.addEdge(0,1);
	task1.addEdge(0,0);
	task1.addEdge(1,2);
	task1.addEdge(2,3);
	task1.addEdge(2,4);
	task1.addEdge(3,4);
	task1.addEdge(2,1);
	task1.addEdge(4,2);
	task1.addEdge(4,5);
	task1.addEdge(5,4);

	task1.printList();

	task1a1(task1,1);
	task1a2(task1);
	
	cout << "\nTask 1b:\n";
	task1.printList();
	cout << "\nResult 1b:\n";
	task1b(task1).printList();
	

	/*Graph task21(true);
	task21.addNode(4);
	task21.addEdge(0,3);
	task21.addEdge(3,1);
	task21.addEdge(3,2);
	task21.addEdge(2,1);

	task21.printList();

	Graph task21Result = task2_1(task21);
	task21Result.printList();*/



	//setupPanel();
}