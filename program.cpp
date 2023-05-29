#include "./lib/query-document.h"
#include <stdio.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//compile: g++ ./lib/*.cpp program.cpp -o program.exe

int main(int argc, char **argv) {
	if(argc == 2) {
		Document *doc = new Document(argv[1], NULL);
		char query[100];
		while(1) {
			query[0] = '\0';
			cout << "\n\nQuery: ";
			scanf("%[^\n]", query);
			if(!(doc->query(query))) {
				cout << "\nResult not found...";
			}
			cout << "\n\nPress any key to continue...\n";
			getch();
			fflush(stdin);
			system("cls");
		}
		delete doc;
	}
}