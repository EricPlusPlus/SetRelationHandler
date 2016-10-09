#pragma once
#include <set>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <map>
#include <fstream>
#include <vector>

typedef std::set<std::pair<int, int>> relation;
typedef std::vector<std::vector<int>> matrix;

class Relation {
private:
	relation R;
	std::string name;

	std::pair<int, int> domain;
	matrix mat;

	void updateProperties();
	void updateDomain();
public:
	Relation(std::string filename, std::string name);
	Relation(std::string name) :name(name) {};
	Relation() = default;

	std::set<std::string> properties;
	relation r_to_the_n_power(int n = 2);

	std::string getName() { return name; }
	relation getR() { return R; }

	bool isReflexive();
	bool isReflexive(std::pair<int, int> domain);
	bool isSymmetric();
	bool isAntiSymmetric();
	bool isTransitive();
	bool empty() { return R.empty(); }

	void displayProperties();
	void addPair(int a, int b);
	void updateProperties(std::pair<int, int>);
	void genMatrix();

	void printMatrix(std::ostream &os, bool isSaved = false);
	void printRelation(std::ostream &os);
	void printRelation(relation r, std::string name);

	Relation& operator=(const relation& rhs); // this->R = rhs
};