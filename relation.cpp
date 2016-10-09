#include "relation.h"

Relation::Relation(std::string filename, std::string name):name(name) {
	std::ifstream input_file(filename);

	if (input_file.fail()) {
		std::cout << "Could not find file.\n";
		return;
	}

	while (!input_file.eof()) {
		std::pair<int, int> p;
		input_file >> p.first >> p.second;

		R.insert(p);
	}

	input_file.close();
	updateDomain();
	updateProperties();

	genMatrix();
}

void Relation::updateDomain() {
	domain.first = R.begin()->first;

	auto it = R.end();
	it--;

	domain.second = it->first;
}
void Relation::updateProperties() {
	isReflexive();
	isSymmetric();
	isAntiSymmetric();
	isTransitive();
}
void Relation::updateProperties(std::pair<int, int> domain) {
	if (!R.empty()) {
		isReflexive(domain);
		isSymmetric();
		isAntiSymmetric();
		isTransitive();
	}
}

bool Relation::isReflexive() {
	for (int i = domain.first; i <= domain.second; i++) {
		if (R.find(std::make_pair(i, i)) == R.end()) {
			properties.erase("Reflexive");
			return false;
		}
	}

	properties.insert("Reflexive");
	return true;
}
bool Relation::isReflexive(std::pair<int, int> domain) {
	for (int i = domain.first; i <= domain.second; i++) {
		if (R.find(std::make_pair(i, i)) == R.end()) {
			properties.erase("Reflexive");
			return false;
		}
	}

	properties.insert("Reflexive");
	return true;
}
bool Relation::isSymmetric() {
	for (auto &p : R) {
		if (p.first != p.second) {//ignore (a,a)
			if (R.find(std::make_pair(p.second, p.first)) == R.end()) { //(a,b) exists, but (b,a) does not 
				properties.erase("Symmetric");
				return false;
			}
		}
	}

	properties.insert("Symmetric");
	return true;
}
bool Relation::isAntiSymmetric() {
	for (auto &p : R) {
		if (p.first != p.second) {//ignore (a,a)
			if (R.find(std::make_pair(p.second, p.first)) != R.end()) { //(a,b), (b,a) exist and a != b
				properties.erase("AntiSymmetric");
				return false;
			}
		}
	}

	properties.insert("AntiSymmetric");
	return true;
}
bool Relation::isTransitive() {
	for (auto curr = R.begin(); curr != R.end(); curr++) {
		for (auto next = (++curr)--; next != R.end(); next++) { // next = curr + 1, change curr back to orginal
			if (curr->second == next->first) { //(a,b) (b,c)
				if (R.find(std::make_pair(curr->first, next->second)) == R.end()) { // (a,c) exist?
					properties.erase("Transitive");
					return false;
				}
			}
			if (curr->first == next->second) { //(b,a) (c,b), compare as (c,b) vs (b,a)
				if (R.find(std::make_pair(next->second, curr->first)) == R.end()) {
					properties.erase("Transitive");
					return false;
				}
			}
		}
	}

	properties.insert("Transitive");
	return true;
}

void Relation::addPair(int a, int b) {
	R.insert(std::make_pair(a, b));
}
void Relation::displayProperties() {
	for (auto &p : properties) {
		std::cout << p << std::endl;
	}
}
void Relation::printRelation(std::ostream &os) {
	os << "{";
	auto last = R.rbegin();

	for (auto &p : R) {
		os << "\n(" << p.first << "," << p.second << ")";
		if (*last != p) {
			std::cout << ",";
		}
	}

	os << "\n}\n\n";
}
void Relation::printRelation(relation r, std::string name) {
	std::cout << name << " = {";

	for (auto &p : r) {
		std::cout << "\n(" << p.first << "," << p.second << ")";
	}

	std::cout << "\n}";
}

relation Relation::r_to_the_n_power(int n) {
	relation result = R;
	relation ans;
	
	if (n > 2) {
		result = r_to_the_n_power(n - 1);
	}

	for (auto curr = result.begin(); curr != result.end(); curr++) {
		if (curr->first == curr->second) {
			ans.insert(std::make_pair(curr->first, curr->second));
		}

		for (auto next = (++curr)--; next != result.end(); next++) { //set curr back to oringal after assigning next to ++curr (i.e. i = 0, j = i + 1)
			if (curr->second == next->first) {
				ans.insert(std::make_pair(curr->first, next->second));
			}
			if (curr->first == next->second) {
				ans.insert(std::make_pair(next->first, curr->second));
			}
		}
	}

	//if (ans.size() == result.size) {} transitive closure met.

	return ans;
}
Relation& Relation::operator=(const relation& rhs) {
	this->R = rhs;

	updateDomain();
	updateProperties();
	return *this;
}

void Relation::genMatrix() {
	size_t range = domain.second - domain.first;

	for (int i = 0; i <= range; i++) {
		mat.push_back(std::vector<int>());

		for (int j = 0; j <= range; j++) {
			mat[i].push_back(0);
		}
	}

	for (auto &p : R) {
		auto row = p.first - domain.first;
		auto col = p.second - domain.first;

		//std::cout << "Inserting " << "(" << p.first << ", " << p.second << ") at " << "[" << row << ", " << col << "]\n";
		mat[row][col] = 1;
	}

}
void Relation::printMatrix(std::ostream &os, bool isSaved) {
	size_t range = domain.second - domain.first;
	os << "Matrix for Relation: " << this->name << std::endl << std::endl;

	for (int i = 0; i <= range; i++) {
		for (int j = 0; j <= range; j++) {
			os << "\t" << mat[i][j];
		}
		os << std::endl;
		os << std::endl;
	}

	if (!isSaved) {
		std::string choice;
		std::cout << "Save matrix to file? (y/n): ";
		std::cin >> choice;

		if (tolower(choice.front() == 'y')) {
			std::string filename;
			std::cout << "Enter a name for the output file: ";
			std::cin >> filename;

			std::ofstream out(filename);
			printMatrix(out, true);
			out.close();
		}
	}
}
