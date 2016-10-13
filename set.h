#pragma once
#include <set>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <map>

#include "relation.h"

class Set {
private:
	std::set<int> s;
	char identifier = 'x';
protected:
	std::pair<int, int> domain;
	std::map<std::string, Relation> relations;
public:
	Set(char c); //dummy parameter protects constructor from being default
	Set() = default;

	void insert(int n) { s.insert(n); }
	void erase(int n) { s.erase(n); }
	void print();
	void displayRelationProperties(std::string relation_name);
	void addNewRelation();
	void addExistingRelation(Relation &R);

	void combineRelation(std::string key);
	void removeRelation(std::string key);
	void displayRelations();
	void viewRelation(std::string key);

	std::string selectRelation();
	char getIdentifier() { return identifier; }
	Relation* getRelation(std::string key) { return &(relations[key]); }

	Set operator|(const Set& a);
	Set operator^(const Set&a);
};

