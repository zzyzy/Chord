#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <vector>

using namespace std;

#define ID_LENGTH 40
#define MAX_TEST_CASE_NUM 3

typedef unsigned long KEY;

std::random_device rd;
std::mt19937 gen(rd());
std::unordered_set<KEY> keys;
std::vector<std::string> data;
std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

std::ofstream outfile;
unsigned short M;
KEY numberOfNodes;
unsigned long numberOfData;

void GenerateRandomString();
void GenerateKeys();
void MassAddingTest();
void MassAddRemoveTest();
void MassMultiAddRemoveTest();
void MassMultiAddRemoveWithInsertDeleteTest();
// void RandomMixedMassAddingRemovalTest();
// void SwitchedMassAddingRemovalTest();
// void RandomSwitchedMassAddingRemovalTest();
const KEY MaxKey();
const KEY Hash(const std::string &s);

int main(int argc, char *argv[]) {
	if (argc < 5) {
		std::cerr << "Usage: " << argv[0] << " <output_filename> <m_bit> <num_of_nodes> <test_case> <num_of_data>" << std::endl;
		return 1;
	}

	M = atoi(argv[2]);
	numberOfNodes = atoi(argv[3]);
	auto testCase = atoi(argv[4]);
	auto maxTestCaseNum = MAX_TEST_CASE_NUM;

	if (numberOfNodes > MaxKey()) {
		std::cerr << "Invalid number of nodes. Try again." << std::endl;
		return 1;
	}

	if (testCase < 0 || testCase > maxTestCaseNum) {
		std::cerr << "Invalid test case. Try 0 - " << maxTestCaseNum << " instead." << std::endl;
		return 1;
	}

	if (argc < 6 && testCase == 3) {
		std::cerr << "This test case requires the number of data items to generate" << std::endl;
		return 1;
	}

	outfile.open(argv[1]);
	if (!outfile.is_open()) return 1;

	std::cout << "Generating keys... " << std::endl;
	GenerateKeys();
	std::cout << "Generating keys... done" << std::endl;

	if (testCase == 3) {
		numberOfData = atoi(argv[5]);
		std::cout << "Generating data... " << std::endl;
		GenerateRandomString();
		std::cout << "Generating data... done" << std::endl;
	}

	outfile << "INITCHORD " << M << std::endl << std::endl;

	switch (testCase) {
		case 0:
			std::cout << "Adding mass adding test commands... " << std::endl;
			MassAddingTest();
			std::cout << "Adding mass adding test commands... done" << std::endl;
			break;
		case 1:
			std::cout << "Adding mass add remove test commands... " << std::endl;
			MassAddRemoveTest();
			std::cout << "Adding mass add remove test commands... done" << std::endl;
			break;
		case 2:
			std::cout << "Adding multi add remove test commands... " << std::endl;
			MassMultiAddRemoveTest();
			std::cout << "Adding multi add remove test commands... done" << std::endl;
			break;
		case 3:
			std::cout << "Adding mass multi add remove with insert delete test commands... " << std::endl;
			MassMultiAddRemoveWithInsertDeleteTest();
			std::cout << "Adding mass multi add remove with insert delete test commands... done" << std::endl;
			break;
	}

	outfile.close();

	return 0;
}

void GenerateRandomString() {
	std::string result;
	result.resize(ID_LENGTH);
    std::uniform_int_distribution<KEY> idRd(0, charset.length() - 1);

    data.reserve(numberOfData);

    for (auto i = 0; i < numberOfData; ++i) {
    	for (auto u = 0; u < result.length(); ++u) {
    		result[u] = charset[idRd(gen)];
    	}

    	data.push_back(result);
    }
}

void GenerateKeys() {
	std::string result;
	result.resize(ID_LENGTH);
    std::uniform_int_distribution<KEY> idRd(0, charset.length() - 1);

	keys.reserve(numberOfNodes);

    for (auto i = 0; i < numberOfNodes; ) {
    	for (auto u = 0; u < result.length(); ++u) {
    		result[u] = charset[idRd(gen)];
    	}

    	if (keys.insert(Hash(result)).second) {
    		++i;
    	}
    }

    //TODO Copy to another vector and swap contents randomly

    // for (auto k : keys) {
    // 	std::cout << "GENERATED KEY " << k << std::endl;
    // }
    // std::cout << "A TOTAL OF " << keys.size() << std::endl;
}

void MassAddingTest() {
	outfile << "# Mass adding test" << std::endl;

	for (auto k : keys) {
		outfile << "ADDPEER " << k << std::endl;
	}

	outfile << std::endl;
}

void MassAddRemoveTest() {
	outfile << "# Mass add remove test" << std::endl;

	for (auto k : keys) {
		outfile << "ADDPEER " << k << std::endl;
	}

	for (auto k : keys) {
		outfile << "REMOVEPEER " << k << std::endl;
	}

	outfile << std::endl;
}

void MassMultiAddRemoveTest() {
	outfile << "# Mass multi add remove test" << std::endl;

	auto switchTimes = 4;
	auto switchVal = true;

	for (auto i = 0; i < switchTimes; ++i) {
		for (auto k : keys) {
			if (switchVal)
				outfile << "ADDPEER " << k << std::endl;
			else
				outfile << "REMOVEPEER " << k << std::endl;
		}
		switchVal = !switchVal;
	}

	outfile << std::endl;
}

void MassMultiAddRemoveWithInsertDeleteTest() {
	outfile << "# Mass multi add remove with insert delete test" << std::endl;

	KEY boundary = numberOfNodes / 4;
	KEY half = numberOfNodes / 2;
	KEY counter = 0;
	auto first = keys.begin();
	auto second = data.begin();
	auto runnable = true;

	while (runnable) {
		if (counter > half) {
			counter = 0;
		}

		if (counter <= boundary) {
			if (first != keys.end()) {
				outfile << "ADDPEER " << *first << std::endl;
				++first;
			}
		} else {
			if (second != data.end()) {
				outfile << "INSERT " << *second << std::endl;
				++second;
			}
		}

		counter++;
		if (first == keys.end() && second == data.end()) runnable = false;
	}

	for (auto k : keys) {
		outfile << "REMOVEPEER " << k << std::endl;
	}

	outfile << std::endl;
}

// void MixedMassAddingRemovalTest() {
// 	KEY boundary = numberOfNodes / 4;
// 	KEY half = numberOfNodes / 2;
// 	KEY counter = 0;

// 	outfile << "# Mixed mass adding removal test" << std::endl;
// 	for (auto k : keys) {
// 		if (counter > half) {
// 			counter = 0;
// 		}
// 		if (counter <= boundary) {
// 			outfile << "ADDPEER " << k << std::endl;
// 		} else {
// 			outfile << "REMOVEPEER " << k << std::endl;
// 		}
// 		counter++;
// 	}

// 	outfile << std::endl;
// }

// void RandomMixedMassAddingRemovalTest() {
// 	std::random_device rd;
// 	std::mt19937 gen(rd());
//     std::uniform_int_distribution<short> dist(2, numberOfNodes - 1);
// 	KEY boundary = numberOfNodes / dist(gen);
// 	KEY half = numberOfNodes / 2;
// 	KEY counter = 0;

// 	outfile << "# Random mixed mass adding removal test" << std::endl;
// 	for (auto k : keys) {
// 		if (counter > half) {
// 			counter = 0;
// 		}
// 		if (counter <= boundary) {
// 			outfile << "ADDPEER " << k << std::endl;
// 		} else {
// 			outfile << "REMOVEPEER " << k << std::endl;
// 		}
// 		counter++;
// 	}

// 	outfile << std::endl;
// }

// void SwitchedMassAddingRemovalTest() {
// 	std::random_device rd;
// 	std::mt19937 gen(rd());
//     std::uniform_int_distribution<short> dist(0, 1);
//     short switchval = dist(gen);

//     outfile << "# Switched mass adding removal test" << std::endl;
// 	for (auto k : keys) {
// 		if (switchval) {
// 			outfile << "ADDPEER " << k << std::endl;
// 		} else {
// 			outfile << "REMOVEPEER " << k << std::endl;
// 		}
// 		switchval = !switchval;
// 	}

// 	outfile << std::endl;
// }

// void RandomSwitchedMassAddingRemovalTest() {
// 	std::random_device rd;
// 	std::mt19937 gen(rd());
// 	std::uniform_int_distribution<short> dist(0, 1);
// 	short switchval = dist(gen);

// 	outfile << "# Random switched mass adding removal test" << std::endl;
// 	for (auto k : keys) {
// 		if (switchval) {
// 			outfile << "ADDPEER " << k << std::endl;
// 		} else {
// 			outfile << "REMOVEPEER " << k << std::endl;
// 		}
// 		switchval = dist(gen);
// 	}

// 	outfile << std::endl;
// }

const KEY MaxKey() {
	return (1UL << M) - 1;
}

const KEY Hash(const std::string &s) {
    KEY key = 0;
    for (auto c : s) {
        key = ((key << 5) + key) ^ c;
    }
    return key & ((1UL << M) - 1);
}
