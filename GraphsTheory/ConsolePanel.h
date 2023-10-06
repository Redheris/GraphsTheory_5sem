#pragma once
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>

#include "Graph.h"

using namespace std;

map<string, Graph> memGraphs = map<string, Graph>();
string currentGraph = "";

Graph& getMemGraph(string title) {
	return memGraphs.at(title);
}

bool isCorrectGraphTitle(string title) {
	cout << "|" << title << "|\n";
	if (title == "") {
		cout << "������������ ��� �����.\n";
		return false;
	}
	return true;
}

bool doOverride(string title) {
	if (memGraphs.find(title) != memGraphs.end()) {
		cout << "���� � ������ " << title << " ��� ����������.\n"
			<< "�� ������ ������������ ��������� ����? Y/N\n";
		string ans;
		cin >> ans;
		string yes[]{ "y", "Y", "yes", "Yes" };
		if (yes->find(ans) == string::npos)
			return false;
	}
	return true;
}

bool isExists(string title) {
	if (memGraphs.find(title) == memGraphs.end()) {
		cout << "���� " << title << " �� ����������.\n";
		return false;
	}
	return true;
}

bool checkCurrentIsNull() {
	if (currentGraph == "") {
		cout << "��� �������������� �� ������ ������� ����. �������� ��� �������� edit.\n";
		return true;
	}
	return false;
}

void start() {
	string cmd;

	while (cmd != "exit" && cmd != "end") {
		cmd = "";
		cout << endl << currentGraph << "> ";
		cin >> cmd;

		if (cmd == "help") {
			cout << "����������: ����� ���������� ������ ���� � �������, ����������� -e.\n"
				<< "��������� �������:\n"
				<< "- new <title> <1|true/0|false> :\n"
				<< "\t������ ����� ���� � ������ title, ������ �������� ����������\n"
				<< "\t����������������� �����;\n"
				<< "- edit <title> :\n"
				<<  "\t�������� ���� title ��� �������������� ������� ���������\n"
				<< "- list :\n"
				<< "\t������� ������ ���������� � ������ ������;\n"
				<< "- delete/del <title> :\n"
				<< "\t������� ���� title �� ������;\n"
				<< "- addNode [-n n] :\n"
				<< "\t��������� � ���� ����� ������������� �������,\n"
				<< "\tn - ���������� ����������� ������, �� ��������� 1;\n"
				<< "- addEdge <x> <y>  [-w weight | -m mark | -wm weight mark] :\n"
				<< "\t��������� � ���� ����� �� ����� x � ����� y,\n"
				<< "\tweight � mark - ��� � ����� ����� ��������������;\n"
				<< "- delNode <x> :\n"
				<< "\t������� �� ����� ���� x;\n"
				<< "- delEdge [x y | -m mark] :\n"
				<< "\t������� �� ����� ����� (x,y) ��� �� ��� ���� �\n"
				<< "\t������ mark;\n"
				<< "- print <title | -e> :\n"
				<< "\t������� ���������� � ����� title ��� � �������\n"
				<< "\t(����������������� � ������ ���������)\n"
				<< "- import <title> <path> :\n"
				<< "\t����������� ���� title �� ����� path � �������� � ������;\n"
				<< "- export <title> <path> :\n"
				<< "\t������������ ���� title � ���� path;\n"
				<< "- copy <title> <original> :\n"
				<< "\t������ ����� ���� title �� ������ ����� original;\n"
				<< "- exit/end :\n"
				<< "\t������� �� ������ ����������� ����������.\n";
		}
		else if (cmd == "edit") {
			cin >> currentGraph;

			if (!isExists(currentGraph))
				currentGraph = "";
		}
		else if (cmd == "new") {
			string title;
			string isOrientedStr;
			
			cin >> title >> isOrientedStr;

			bool isOriented =
				isOrientedStr == "1" || isOrientedStr == "true" || isOrientedStr == "True";
			
			if (!doOverride(title)) continue;
			
			memGraphs.insert(make_pair(title, Graph(isOriented)));
			currentGraph = title;
			cout << "���� " << title << " ������� ������ � ������ ��� ��������������.\n";

		}
		else if (cmd == "list") {
			cout << "\n��������� �����:\n";
			for (auto i : memGraphs) {
				cout << i.first;
				if (i.first != (*memGraphs.rbegin()).first) {
					cout << ", ";
				}
			}
			cout << "\n";
		}
		else if (cmd == "delete" || cmd == "del") {
			string title;
			cin >> title;

			if (!isExists(title)) continue;

			memGraphs.erase(currentGraph);
			currentGraph = "";
			cout << "���� " << currentGraph << " ������� �����.\n";
		}
		else if (cmd == "addNode") {
			string flag;
			int n = 1;
			cin >> flag;
			
			if (checkCurrentIsNull()) continue;

			if (flag == "-n")
				cin >> n;
			getMemGraph(currentGraph).addNode(n);
			cout << "� ���� " << currentGraph << " ��������� " << n << " ������.\n";
		}
		else if (cmd == "addEdge") {
			string flag, mark = "";
			int x, y, weight = 0;
			cin >> x >> y >> flag;
			
			if (checkCurrentIsNull()) continue;

			if (flag != "-e") {
				if (flag == "-w")
					cin >> weight;
				else if (flag == "-m")
					cin >> mark;
				else if (flag == "-wm")
					cin >> weight >> mark;
				else if (flag == "-mw")
					cin >> mark >> weight;
			}
			getMemGraph(currentGraph).addEdge(x, y, weight, mark);
			string edgeChar = "";
			if (weight != 0 || mark != "") {
				edgeChar = " � ���������������� (" + to_string(weight);
				if (mark != "")
					edgeChar += ", " + mark + ")";
				else
					edgeChar += ")";
			}
			cout << "� ���� " << currentGraph << " ��������� ����� (" << x << "," << y << ")" << edgeChar << ".\n";
		}
		else if (cmd == "delNode") {
			int x;
			cin >> x;
			
			if (checkCurrentIsNull()) continue;

			getMemGraph(currentGraph).delNode(x);
			cout << "�� ����� " << currentGraph << " ������� ������� " << x << ".\n";
		}
		else if (cmd == "delEdge") {
			string flag;
			cin >> flag;
			
			if (checkCurrentIsNull()) continue;

			Graph& g = getMemGraph(currentGraph);
			if (flag == "-m") {
				string mark;
				cin >> mark;
				g.delEdges(mark);
				cout << "�� ����� " << currentGraph << "������� ��� ���� � ������ " << mark << ".\n";
			}
			else {
				int x, y;
				x = stoi(flag);
				cin >> y;
				g.delEdge(x, y);
				cout << "�� ����� " << currentGraph << " ������� ����� (" << x << "," << y << ").\n";
			}
		}
		else if (cmd == "print") {
			string title;
			cin >> title;

			if (title == "-e") {
				if (checkCurrentIsNull()) continue;
				title = currentGraph;
			}
			else
				if (!isExists(title)) continue;

			cout << "\n���������� � ����� " << title << ":\n";
			getMemGraph(title).printList();
		}
		else if (cmd == "import") {
			string title, path;
			cin >> title >> path;

			if (!doOverride(title)) continue;

			ifstream in(path);
			if (!in.good()) {
				cout << "���� �� ������. ��������� ������������ ����.\n";
				continue;
			}
			memGraphs.insert(make_pair(title, NULL));
			getMemGraph(title) = Graph(path);
			in.close();
			cout << "���� " << title << " ������� ������������ �� ����� " << path << ".\n";
		}
		else if (cmd == "export") {
			string title, path;
			cin >> title >> path;

			if (!isExists(title)) continue;

			getMemGraph(title).exportList(path);
			cout << "���� " << title << " ������� ������������� � ���� " << path << ".\n";
		}
		else if (cmd == "copy") {
			string title, original;
			cin >> title >> original;

			if (!isExists(original) || !doOverride(title)) continue;



			memGraphs.insert(make_pair(title, NULL));
			getMemGraph(title) = getMemGraph(original);
			cout << "���� " << currentGraph << " ������� ������ �� ������ ����� " << original << ".\n";
		}
		else if (cmd == "exit" || cmd == "end") {
			cout << "���������� ��������� ������.\n";
		}
		else
			cout << "����������� �������. �������� help ��� ������� ������ ��������� ������.\n";
	}
}

void setupPanel() {
	cout
		<< "\n������� ���������� ��������� ��� ������ � �������.\n"
		<< "��� ������ ��������� ������ �������� help, ��� ������ - exit.\n";
	start();
}

