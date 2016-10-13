#include "relation.h"
#include "set.h"

Relation to_the_n_power(Relation R, int n) {
	Relation ans = R;

	for (int i = 1; i < n; i++) {
		ans *= R;
	}
	return ans;
}
Relation composite(Relation S, Relation R) {
	Relation SR = S; //copy properties of S to SR
	SR.rel.clear(); //but clear the existing relation

	for (auto p : R) { //each pair in R
		for (auto q : S) { //each pair in S
			if (p.second == q.first) { //(a,b) (b,c)
				SR.insert(std::make_pair(p.first, q.second)); //insert(a,c)
			}
		}
	}

	return SR;
}
Relation getTransitiveClosure(Relation R) {
	Relation closure = R;
	Relation orig = R;

	while (!closure.isTransitive()) {
		R *= orig;
		closure = closure | R;
	}

	closure.mat.clear(); //refresh the matrix
	closure.genMatrix();

	closure.updateName("tc_" + closure.getName());
	return closure;
}
Relation warshawAlgorithm(Relation R) {
	size_t upper_bound = R.domain.second - R.domain.first;

	for (int i = 0; i <= upper_bound; i++) { //first index of matrix is 0 regardless of what actual domain.first is.
		std::cout << "W(" << i << ") = \n";
		R.printMatrix();

		if (R.rel.size() == R.mat.size() * R.mat.begin()->size()) { //num ordered pairs equals matrix size
			std::cout << "Matrix is full at W(" << i << ")\n";
			return R;
		}
		std::set<int> rows = R.get_row_i(i);
		std::set<int> cols = R.get_col_j(i);

		std::cout << "Combining { ";
		for (auto r : rows) {
			std::cout << r << " ";
		}

		std::cout << "}\n With: { ";
		for (auto c : cols) {
			std::cout << c << " ";
		}

		std::cout << "}\n";

		for (auto j : rows) {
			for (auto k : cols) {
				R.insert(std::make_pair(j,k));
			}
		}

		R.mat.clear();
		R.genMatrix();
	}

	std::cout << "W(" << upper_bound << ") = \n";
	R.printMatrix();

	return R;
}

std::set<int> Relation::get_row_i(int i) {
	std::set<int> rows;

	for (int j = 0; j < mat.size(); j++) {
		if (mat[j][i] == 1) {
			rows.insert(j + domain.first);
		}
	}

	return rows;
}
std::set<int> Relation::get_col_j(int j) {
	std::set<int> cols;

	for (int i = 0; i < mat[j].size(); i++) {
		if (mat[j][i] == 1) {
			cols.insert(i + domain.first);
		}
	}

	return cols;
}

Relation::Relation(std::string filename, std::string name, std::pair<int, int> domain, Set* parent):name(name), parent(parent){
	std::ifstream input_file(filename);

	if (input_file.fail()) {
		std::cout << "Could not find file.\n";
		return;
	}

	while (!input_file.eof()) {
		std::pair<int, int> p;
		input_file >> p.first >> p.second;

		rel.insert(p);
	}
	
	Set::domain = domain;

	input_file.close();
	updateProperties();

	genMatrix();
}
Relation::Relation(const Relation& R):rel(R.rel), properties(R.properties), mat(R.mat), name(R.name)
{
	this->domain = R.domain;
	this->parent = R.parent;
}

void Relation::updateDomain() {
	domain.first = rel.begin()->first;

	auto it = rel.end();
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
	if (!rel.empty()) {
		isReflexive(domain);
		isSymmetric();
		isAntiSymmetric();
		isTransitive();
	}
}

bool Relation::isReflexive() {
	for (int i = domain.first; i <= domain.second; i++) {
		if (rel.find(std::make_pair(i, i)) == rel.end()) {
			properties.erase("Reflexive");
			return false;
		}
	}

	properties.insert("Reflexive");
	return true;
}
bool Relation::isReflexive(std::pair<int, int> domain) {
	for (int i = domain.first; i <= domain.second; i++) {
		if (rel.find(std::make_pair(i, i)) == rel.end()) {
			properties.erase("Reflexive");
			return false;
		}
	}

	properties.insert("Reflexive");
	return true;
}
bool Relation::isSymmetric() {
	for (auto &p : rel) {
		if (p.first != p.second) {//ignore (a,a)
			if (rel.find(std::make_pair(p.second, p.first)) == rel.end()) { //(a,b) exists, but (b,a) does not 
				properties.erase("Symmetric");
				return false;
			}
		}
	}

	properties.insert("Symmetric");
	return true;
}
bool Relation::isAntiSymmetric() {
	for (auto &p : rel) {
		if (p.first != p.second) {//ignore (a,a)
			if (rel.find(std::make_pair(p.second, p.first)) != rel.end()) { //(a,b), (b,a) exist and a != b
				properties.erase("AntiSymmetric");
				return false;
			}
		}
	}

	properties.insert("AntiSymmetric");
	return true;
}
bool Relation::isTransitive() {
	for (auto curr = rel.begin(); curr != rel.end(); curr++) {
		for (auto next = (++curr)--; next != rel.end(); next++) { // next = curr + 1, change curr back to orginal
			if (curr->second == next->first) { //(a,b) (b,c)
				if (rel.find(std::make_pair(curr->first, next->second)) == rel.end()) { // (a,c) exist?
					properties.erase("Transitive");
					return false;
				}
			}
			if (curr->first == next->second) { //(b,a) (c,b), compare as (c,b) vs (b,a)
				if (rel.find(std::make_pair(next->second, curr->first)) == rel.end()) {
					properties.erase("Transitive");
					return false;
				}
			}
		}
	}

	properties.insert("Transitive");
	return true;
}

void Relation::addToSet() {
	std::string ans;
	std::cout << "Add this relation to set " << parent->getIdentifier() << "? (y/n) ";
	std::cin >> ans;

	if (tolower(ans.front()) == 'y') {
		if (parent != nullptr) {
			parent->addExistingRelation(*this);
			std::cout << "Added to " << parent->getIdentifier() << std::endl;
		}
	}
}
void Relation::addPair(int a, int b) {
	rel.insert(std::make_pair(a, b));
}
void Relation::displayProperties() {
	for (auto &p : properties) {
		std::cout << p << std::endl;
	}
}
void Relation::printRelation(std::ostream &os) {
	os << this->name << " = {";
	auto last = rel.rbegin();
	for (auto &p : rel) {
		os << "\n(" << p.first << "," << p.second << ")";
		if (*last != p) {
			std::cout << ",";
		}
	}

	os << "\n}\n\n";
}
void Relation::printRelation(Relation R, std::string name) {
	std::cout << name << " = {";

	for (auto p : R) {
		std::cout << "\n(" << p.first << "," << p.second << ")";
	}

	std::cout << "\n}\n\n";
}

Relation Relation::operator*(const Relation& rhs) {
	return composite(*this, rhs);
}
Relation& Relation::operator=(const Relation& rhs) {
	this->rel = rhs.rel;
	this->name = rhs.name;
	this->mat = rhs.mat;
	this->domain = rhs.domain;
	this->parent = rhs.parent;

	updateProperties();
	return *this;
}
Relation& Relation::operator*=(const Relation& rhs) {
	*this = *this * rhs;
	return *this;
}
Relation Relation::operator|(const Relation& rhs) {
	Relation ans(rhs.name, rhs.domain, this->parent);
	for (auto p : rel) {
		ans.rel.insert(p);
	}
	for (auto p : rhs.rel) {
		ans.rel.insert(p);
	}

	return ans;
}
Relation& Relation::operator|=(const Relation& rhs) {
	for (auto p : rhs.rel) {
		this->rel.insert(p);
	}

	return *this;
}
Relation Relation::operator&(const Relation& rhs) {
	Relation intersection;

	for (auto p : rel) {
		if (rhs.rel.find(p) != rhs.rel.end()) {
			intersection.insert(p);
		}
	}

	return intersection;
}
Relation Relation::operator-(const Relation& rhs) {
	Relation diff;

	for (auto p : this->rel) {
		if (rhs.rel.find(p) == rhs.rel.end()) { //in *this but not in rhs
			diff.insert(p);
		}
	}

	return diff;
}
Relation& Relation::operator~() {
	for (auto p : this->rel) {
		p = std::make_pair(p.second, p.first);
	}
	
	return *this;
}

void Relation::genMatrix() {
	size_t range = domain.second - domain.first;
	//std::cout << "The matrix will be " << range << " x " << range << std::endl;

	for (int i = 0; i <= range; i++) {
		mat.push_back(std::vector<int>());

		for (int j = 0; j <= range; j++) {
			mat[i].push_back(0);
		}
	}

	for (auto &p : rel) {
		auto row = p.first - domain.first;
		auto col = p.second - domain.first;

		//std::cout << "Inserting " << "(" << p.first << ", " << p.second << ") at " << "[" << row << ", " << col << "]\n";
		mat[row][col] = 1;
	}

}
void Relation::printMatrix() {
	size_t range = domain.second - domain.first;
	this->mat.clear();
	this->genMatrix();

	for (int i = 0; i <= range; i++) {
		for (int j = 0; j <= range; j++) {
			std::cout << "\t" << mat[i][j];
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}
void Relation::printMatrix(std::ostream &os, bool isSaved) {
	size_t range = domain.second - domain.first;
	os << "Matrix for Relation: " << this->name << std::endl << std::endl;
	std::cout << "The matrix will be " << range << " x " << range << std::endl;

	std::cout << "The num of vectors in the matrix is: " << mat.size() << std::endl;
	std::cout << "The num of elements is: " << mat.begin()->size() << std::endl;

	this->mat.clear();
	this->genMatrix();

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
void Relation::printMatrix(Relation R, std::ostream &os, bool isSaved) {
	size_t range = R.domain.second - R.domain.first;
	os << "Matrix for Relation: " << R.name << std::endl << std::endl;
	os << "The size of the matrix is: " << mat.size() << std::endl;

	R.mat.clear();
	R.genMatrix();

	for (int i = 0; i <= range; i++) {
		for (int j = 0; j <= range; j++) {
			os << "\t" << R.mat[i][j];
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
			printMatrix(R,out, true);
			out.close();
		}
	}
}



