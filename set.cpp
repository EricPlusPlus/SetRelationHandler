#include <sstream>

#include "relation.h"
#include "set.h"

Set Set::operator|(const Set& a) {
	Set temp;

	for (auto p : this->s) {
		temp.insert(p);
	}

	for (auto p : a.s) {
		temp.insert(p);
	}
	return temp;
}
Set Set::operator^(const Set& a) {
	Set ans;

	for (auto e : this->s) {
		if (a.s.find(e) != a.s.end()) {
			ans.insert(e);
		}
	}

	return ans;
}

Set::Set(char c) {
	int num;

	std::cout << "Enter the values for the set.\nClose the set with a '}' when finished." << std::endl;
	std::cout << c << " = { ";

	std::string input;
	std::getline(std::cin, input, '}');

	std::replace_if(input.begin(), input.end(), [](const char&c) { return c == ','; }, ' ');
	std::stringstream ss(input);

	while (ss >> num) {
		s.insert(num);
	}

	std::cin.clear();
	auto it = s.end();
	it--;

	domain.first = *s.begin();
	domain.second = *it;
}

void Set::addRelation() {
	std::string ans;
	std::string r_name;

	std::cout << "Is your relation in a file? (y/n): ";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin >> ans;

	std::cout << "Enter a name for this relation: ";
	std::cin >> r_name;

	if (tolower(ans.front()) == 'y') {
		std::string filename;
		std::cout << "Enter the filename: ";
		std::cin >> filename;

		Relation R(filename, r_name);
		R.updateProperties(domain);

		if (!R.empty()) {
			relations[r_name] = R;
		}
	}
	else {
		Relation R(r_name);

		std::cout << "Enter the pairs for the relation.\nEnter two numbers per line seperated by a space.\nType Done when finished\n";

		int a, b;
		while (std::cin >> a >> b) {
			R.addPair(a, b);
		}

		R.updateProperties(domain);
		relations[r_name] = R;
	}
}
void Set::removeRelation(std::string key) {
	if (relations.find(key) != relations.end()) {
		relations.erase(key);
		std::cout << "Reservation removed\n";
	}
}

void Set::displayRelationProperties(std::string relation_name) {
	std::string ans;

	if (relations.find(relation_name) != relations.end()) {
		auto r = relations[relation_name];

		r.updateProperties(domain);
		r.displayProperties();
	}
	else {
		std::cout << "Relation " << relation_name << "does not exist!\n";
	}
}
void Set::print() {
	std::cout << "{ ";
	for (auto it = s.begin(); it != s.end(); it++) {
		auto next = (++it)--;

		std::cout << *it;
		if (next != s.end()) { //avoid printing a comma after the last number
			std::cout << ", ";
		}
	}
	std::cout << " }";
}
void Set::displayRelations() {
	for (auto r : relations) {
		std::cout << r.first << std::endl;
	}
}
void Set::viewRelation(std::string key) {
	if (relations.find(key) != relations.end()) {
		auto &r = relations[key];
		std::cout << std::endl;
		std::cout << "The relation " << key << " has the following properties: \n";

		r.displayProperties();

		std::cout << std::endl;
		std::cout << key << " = ";
		r.printRelation(std::cout);
		std::cout << std::endl;
	}
}

void Set::combineRelation(std::string key) {
	int n = 2;

	std::cout << "How many times do you want to combine " << key << "?\n";
	std::cout << "(i.e. " << key << "^n)\n";

	std::cin >> n;

	if (relations.find(key) != relations.end()) {
		auto R = relations[key];
		
		R = R.r_to_the_n_power(n);

		std::cout << key << "^" << n << " = ";
		R.printRelation(std::cout);
		std::cout << std::endl;
		
		std::string c;
		std::cout << "Save to file? (y/n): ";
		std::cin >> c;

		if (tolower(c.front()) == 'y') {
			std::cout << "Enter a file name: ";
			std::cin >> c;

			std::ofstream out(c);
			R.printRelation(out);
			out.close();
		}
	}
}
std::string Set::selectRelation() {
	std::cout << "Choose from one of the relations below:\n";

	displayRelations();
	std::string key;
	std::cin >> key;

	if (relations.find(key) != relations.end()) {
		return key;
	}
	return "";
}