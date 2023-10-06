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
		cout << "Недопустимое имя графа.\n";
		return false;
	}
	return true;
}

bool doOverride(string title) {
	if (memGraphs.find(title) != memGraphs.end()) {
		cout << "Граф с именем " << title << " уже существует.\n"
			<< "Вы хотите перезаписать имеющийся граф? Y/N\n";
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
		cout << "Граф " << title << " не существует.\n";
		return false;
	}
	return true;
}

bool checkCurrentIsNull() {
	if (currentGraph == "") {
		cout << "Для редактирования не выбран никакой граф. Выберите его командой edit.\n";
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
			cout << "Примечание: чтобы отказаться писать флаг в команде, используйте -e.\n"
				<< "Доступные команды:\n"
				<< "- new <title> <1|true/0|false> :\n"
				<< "\tсоздаёт новый граф с именем title, второй аргумент определяет\n"
				<< "\tориентированность графа;\n"
				<< "- edit <title> :\n"
				<<  "\tвыбирает граф title для редактирования другими командами\n"
				<< "- list :\n"
				<< "\tвыводит список записанных в памяти графов;\n"
				<< "- delete/del <title> :\n"
				<< "\tудаляет граф title из памяти;\n"
				<< "- addNode [-n n] :\n"
				<< "\tдобавляет в граф новую изолированную вершину,\n"
				<< "\tn - количество добавляемых вершин, по умолчанию 1;\n"
				<< "- addEdge <x> <y>  [-w weight | -m mark | -wm weight mark] :\n"
				<< "\tдобавляет в граф ребро из точки x в точку y,\n"
				<< "\tweight и mark - вес и метка ребра соответственно;\n"
				<< "- delNode <x> :\n"
				<< "\tудаляет из графа узел x;\n"
				<< "- delEdge [x y | -m mark] :\n"
				<< "\tудаляет из графа ребро (x,y) или же все рёбра с\n"
				<< "\tметкой mark;\n"
				<< "- print <title | -e> :\n"
				<< "\tвыводит информацию о графе title или о текущем\n"
				<< "\t(ориентированность и список смежности)\n"
				<< "- import <title> <path> :\n"
				<< "\tимпортирует граф title из файла path и записать в памяти;\n"
				<< "- export <title> <path> :\n"
				<< "\tэкспортирует граф title в файл path;\n"
				<< "- copy <title> <original> :\n"
				<< "\tсоздаёт новый граф title на основе графа original;\n"
				<< "- exit/end :\n"
				<< "\tвыходит из режима консольного интерфейса.\n";
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
			cout << "Граф " << title << " успешно создан и выбран для редактирования.\n";

		}
		else if (cmd == "list") {
			cout << "\nИмеющиеся графы:\n";
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
			cout << "Граф " << currentGraph << " успешно удалён.\n";
		}
		else if (cmd == "addNode") {
			string flag;
			int n = 1;
			cin >> flag;
			
			if (checkCurrentIsNull()) continue;

			if (flag == "-n")
				cin >> n;
			getMemGraph(currentGraph).addNode(n);
			cout << "В граф " << currentGraph << " добавлено " << n << " вершин.\n";
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
				edgeChar = " с характеристиками (" + to_string(weight);
				if (mark != "")
					edgeChar += ", " + mark + ")";
				else
					edgeChar += ")";
			}
			cout << "В граф " << currentGraph << " добавлено ребро (" << x << "," << y << ")" << edgeChar << ".\n";
		}
		else if (cmd == "delNode") {
			int x;
			cin >> x;
			
			if (checkCurrentIsNull()) continue;

			getMemGraph(currentGraph).delNode(x);
			cout << "Из графа " << currentGraph << " удалена вершина " << x << ".\n";
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
				cout << "Из графа " << currentGraph << "удалены все рёбра с меткой " << mark << ".\n";
			}
			else {
				int x, y;
				x = stoi(flag);
				cin >> y;
				g.delEdge(x, y);
				cout << "Из графа " << currentGraph << " удалено ребро (" << x << "," << y << ").\n";
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

			cout << "\nИнформация о графе " << title << ":\n";
			getMemGraph(title).printList();
		}
		else if (cmd == "import") {
			string title, path;
			cin >> title >> path;

			if (!doOverride(title)) continue;

			ifstream in(path);
			if (!in.good()) {
				cout << "Файл не найден. Проверьте правильность пути.\n";
				continue;
			}
			memGraphs.insert(make_pair(title, NULL));
			getMemGraph(title) = Graph(path);
			in.close();
			cout << "Граф " << title << " успешно импортирован из файла " << path << ".\n";
		}
		else if (cmd == "export") {
			string title, path;
			cin >> title >> path;

			if (!isExists(title)) continue;

			getMemGraph(title).exportList(path);
			cout << "Граф " << title << " успешно экспротирован в файл " << path << ".\n";
		}
		else if (cmd == "copy") {
			string title, original;
			cin >> title >> original;

			if (!isExists(original) || !doOverride(title)) continue;



			memGraphs.insert(make_pair(title, NULL));
			getMemGraph(title) = getMemGraph(original);
			cout << "Граф " << currentGraph << " успешно создан на основе графа " << original << ".\n";
		}
		else if (cmd == "exit" || cmd == "end") {
			cout << "Консольный интерфейс закрыт.\n";
		}
		else
			cout << "Неизвестная команда. Напишите help для полного списка имеющихся команд.\n";
	}
}

void setupPanel() {
	cout
		<< "\nЗапущен консольный интерфейс для работы с графами.\n"
		<< "Для списка доступных команд напишите help, для выхода - exit.\n";
	start();
}

