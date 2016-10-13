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

class Relation;

#include "set.h"

class Relation:public Set {
	friend Relation composite(Relation S, Relation R);
	friend Relation getTransitiveClosure(Relation R);
	friend Relation to_the_n_power(Relation R, int n);
	friend Relation warshawAlgorithm(Relation R);

private:
	relation rel;
	std::string name;

	Set* parent;
	matrix mat;
	void updateProperties();
	void updateDomain();
public:
	Relation(std::string filename, std::string name, std::pair<int, int> domain, Set* parent); //Relation from file
	Relation(std::string name, std::pair<int, int> domain, Set* parent) :name(name), parent(parent) { Set::domain = domain; genMatrix(); }; //user input relation
	Relation(const Relation& R);
	Relation() = default;


	std::set<std::string> properties;
	std::set<int> get_row_i(int i);
	std::set<int> get_col_j(int j);

	bool isReflexive();
	bool isReflexive(std::pair<int, int> domain);
	bool isSymmetric();
	bool isAntiSymmetric();
	bool isTransitive();
	bool empty() { return rel.empty(); }

	std::string getName() { return name; }
	relation::iterator begin() { return rel.begin(); }
	relation::iterator	end() { return rel.end(); }

	void displayProperties();
	void updateName(std::string name) { this->name = name; }
	void addPair(int a, int b);
	void addToSet(); //adds relation to parent Set::relations container
	void updateProperties(std::pair<int, int>);
	void genMatrix();
	void insert(std::pair<int, int> p) { rel.insert(p); }

	void printMatrix();
	void printMatrix(std::ostream &os, bool isSaved = false);
	void printMatrix(Relation R, std::ostream &os, bool isSaved = false);
	void printRelation(std::ostream &os);
	void printRelation(Relation r, std::string name);

	Relation& operator=(const Relation& rhs); // this->R = rhs
	Relation& operator*=(const Relation& rhs); //Composition Assignment 
	Relation operator|(const Relation& rhs); //Union
	Relation& Relation::operator|=(const Relation& rhs); //Compound Union
	Relation operator&(const Relation& rhs); //Intersection
	Relation operator-(const Relation& rhs); //Difference
	Relation operator*(const Relation& rhs); //Composition
	Relation& operator~(); //R^-1
};


