#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <limits>

//todo regex expressions for set-builder notation.
void pause() {
	//system("pause");
	//system("cls");
}

#include "relation.h"
#include "set.h"

void displaySets(std::map<char, Set> &sets) {
	for (auto set : sets) {
		std::cout << set.first << " - ";
		set.second.print();
		std::cout << std::endl;
	}

	if (sets.empty()) {
		std::cout << "No set exists\n";
	}
}
void displayMainMenu() {
	std::cout << "Choose one of the following options:\n";
	std::cout << "1 - Add a set\n";
	std::cout << "2 - View set options\n";
	std::cout << "3 - Remove a set\n";
	std::cout << "0 - Exit\n\n";
}
void addSet(std::map<char, Set> &sets) {
	char i;

	std::cout << "Enter a character to identify this set: ";
	std::cin >> i;

	Set s(i);
	sets[i] = s;

	std::cin.clear();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
char selectSet(std::map<char, Set> &sets) {
	char choice;
	std::cout << "Choose a set: ";

	std::cin >> choice;

	return choice;
}
void removeSet(std::map<char, Set> &sets, char s) {
	if (sets.erase(s))
		std::cout << "Set " << s << " removed.\n";
}

void displaySetOptions() {
	std::cout << "Choose one of the following options:\n";
	std::cout << "1 - Add a Relation\n";
	std::cout << "2 - Remove a Relation\n";
	std::cout << "3 - View Relation options\n";
	std::cout << "4 - Combine a Relation n times\n";
	std::cout << "5 - Union with another set\n";
	std::cout << "6 = Find intersection with another set\n";
	std::cout << "0 - Back\n\n";
}
void displayRelationOptions(std::string key) {
	std::cout << "Choose one of the following options:\n";
	std::cout << "1 - View Properties\n";
	std::cout << "2 - Display " << key << "^2\n";
	std::cout << "3 - Print the matrix\n";
	std::cout << "4 - Print the relation of the transitive closure\n";
	std::cout << "5 - Print the matrix for the transitive closure\n";
	std::cout << "6 - Print the matrix using warshaws algorithm\n";
	std::cout << "7 - Print the ordered pairs of the relation\n";
	std::cout << "0 - Back\n\n";
}

void handleRelationOption(Relation &rel, std::string key) {
	int c = 1;

	while (c >= 0) {
		std::cout << "Relation: " << rel.getName() << std::endl << std::endl;

		std::cin.clear();
		displayRelationOptions(key);
		std::cin >> c;

		switch (c) {
		case 1:
			rel.displayProperties();
			break;
		case 2:
			rel.printRelation(to_the_n_power(rel, 2), key);
			to_the_n_power(rel, 2).addToSet();
			break;
		case 3:
			rel.printMatrix(std::cout);
			break;
		case 4:
			getTransitiveClosure(rel).printRelation(std::cout);
			getTransitiveClosure(rel).addToSet();
			break;
		case 5:
			rel.printMatrix(getTransitiveClosure(rel), std::cout);
			getTransitiveClosure(rel).addToSet();
			break;
		case 6:
			warshawAlgorithm(rel).addToSet();
			break;
		case 7:
			rel.printRelation(std::cout);
			break;
		case 0:
			return;
		}
	}
}
void handleSetOption(Set &set, char key) {
	int c = 1;	

	while (c >= 0) {
		std::cout << "Set: " << key << " = ";
		set.print();
		std::cout << std::endl << std::endl;

		std::cin.clear();
		displaySetOptions();

		std::cin >> c;

		std::string key;
		switch (c) {
		case 1:
			set.addNewRelation();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		case 2:
			key = set.selectRelation();
			set.removeRelation(key);
			break;
		case 3:
			key = set.selectRelation();
			handleRelationOption(*set.getRelation(key), key);
			break;
		case 4:
			set.combineRelation(set.selectRelation());
			break;
		case 5:
			break;
		default:
			return;
		}

		pause();
	}
}

int main() {
	int choice = 0;
	char key;

	std::map<char, Set> sets;

	while (choice >= 0) {
		if (!sets.empty()) {
			std::cout << "Sets: \n";
			displaySets(sets);

			std::cout << std::endl;
		}

		displayMainMenu();
		std::cin.clear();

		std::cin >> choice;

		switch (choice) {
		case 1:
			addSet(sets);
			break;
		case 2:
			key = selectSet(sets);

			pause();
			if (sets.find(key) != sets.end()) {
				handleSetOption(sets[key], key);
			}
			else {
				std::cout << "Set not found\n";
			}
			break;
		case 3:

			break;
		case 0:
			return 0;
		}
	}

	return 0;
}