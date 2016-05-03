//
// Author: Zhen Zhi Lee
// Contact: leezhenzhi@gmail.com
//
// Driver program for the chord implementation.
//
// C++11 is required to compile this program!
// make sure to pass `-std=c++11` to the compiler!
// compile with `make` or any IDE like CLion
// Ignore unsigned comparison warnings
//
// Solved issues:
// 		1. SOLVED Segmentation fault when moving data during node joins/departures
//		2. SOLVED Removing non existant node causes segmentation fault
//		3. SOLVED Segmentation fault if issue the command InitChord twice
// 		4. SOLVED Successor mismatch if inserting node whose ID is less than the first node's ID
//		5. SOLVED Removing adjacent nodes cases successor mismatch
//		6. SOLVED FindKey displays wrong path
//
// Known issues:
//		1. Adding more than 5000 nodes slows down (must be an issue with how it fixes the fingers)
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <sys/stat.h>
#include "Chord.h"
#include "Utility.h"

#define NODE_START_ID 0

void InitChord(const unsigned short &M, const Node::KEY &startID, Chord *&chord);
void AddPeer(const Node::KEY &ID, Chord *&chord);
void RemovePeer(const Node::KEY &ID, Chord *&chord);
void FindKey(const Node::KEY &key, const Chord * const &chord);
void Insert(const std::string &str, Chord *&chord);
void Delete(const std::string &str, Chord *&chord);
void Print(const Node::KEY &key, const Chord * const &chord);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " <command_file> <log_file>" << std::endl;
		std::cerr << "note: logging only available in debug mode" << std::endl;
		return 1;
	}

	struct stat chkbuffer;
  	if (stat (argv[1], &chkbuffer) != 0) {
  		std::cerr << "Problem reading from file \"" << argv[1] << "\"" << std::endl;
  		std::cerr << "Make sure that the file exists and it is not locked" << std::endl;
  		return 1;
  	}

	#ifdef DEBUG
	std::ofstream logger;
	if (argc >= 3) logger.open(argv[2]);
	unsigned long currentLine = 0;
	#endif

	std::ifstream reader(argv[1]);
	std::string buffer;
	Chord *chord = nullptr;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	while (std::getline(reader, buffer)) {
		#ifdef DEBUG
		++currentLine;
		#endif

		if (buffer[0] == '#' || buffer[0] == '\0') continue;

		std::stringstream cmdStream(buffer);
		std::string cmd;

		cmdStream >> cmd;
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

		#ifdef DEBUG
		std::chrono::high_resolution_clock::time_point cmdt1 = std::chrono::high_resolution_clock::now();
		#endif

		if (cmd == "INITCHORD") {
			unsigned short M;
			cmdStream >> M;
			InitChord(M, NODE_START_ID, chord);
		} else if (cmd == "ADDPEER") {
			Node::KEY ID;
			cmdStream >> ID;
			AddPeer(ID, chord);
		} else if (cmd == "REMOVEPEER") {
			Node::KEY ID;
			cmdStream >> ID;
			RemovePeer(ID, chord);
		} else if (cmd == "FINDKEY") {
			Node::KEY key;
			cmdStream >> key;
			FindKey(key, chord);
		} else if (cmd == "INSERT") {
			std::string data;
			cmdStream >> std::ws;
			std::getline(cmdStream, data, '#');
			Insert(Utility::TrimRight(data), chord);
		} else if (cmd == "DELETE") {
			std::string data;
			cmdStream >> std::ws;
			std::getline(cmdStream, data, '#');
			Delete(Utility::TrimRight(data), chord);
		} else if (cmd == "PRINT") {
			Node::KEY key;
			cmdStream >> key;
			Print(key, chord);
		}

		#ifdef DEBUG
		std::chrono::high_resolution_clock::time_point cmdt2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> runtimeInMilliseconds = cmdt2 - cmdt1;
		std::chrono::duration<double> runtimeInSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(cmdt2 - cmdt1);
		if (logger.is_open()) {
			logger << "Finished cmd \"" << cmd << "\" in " << runtimeInMilliseconds.count() << " ms";
			logger << ", " << runtimeInSeconds.count() << " s" << std::endl;
		}

		if (chord == nullptr) continue;
		bool checkSucc = chord->CheckSuccessors();
		bool checkPred = chord->CheckPredecessors();

		if (logger.is_open()) {
			std::cout << std::endl;
			std::cout << "#########################################################################" << std::endl;
			std::cout << "                Showing all nodes for debugging purposes..." << std::endl;
			std::cout << "#########################################################################" << std::endl;
			chord->ShowAll();
			std::cout << "#########################################################################" << std::endl;
		}

		if (!checkSucc || !checkPred) {
			std::cout << "#########################################################################" << std::endl;
			std::cout << "COMMAND: " << cmd << " AT LINE NUMBER " << currentLine << " CAUSES:" << std::endl;
			if (!checkSucc) std::cout << "SUCCESSORS MISMATCH" << std::endl;
			if (!checkPred) std::cout << "PREDECESSORS MISMATCH" << std::endl;
			std::cout << "EXITING..." << std::endl;
			std::cout << "#########################################################################" << std::endl;
			break;
		}
		#endif
	}

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> runtimeInMilliseconds = t2 - t1;
	std::chrono::duration<double> runtimeInSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Finished in " << runtimeInMilliseconds.count() << " ms";
	std::cout << ", " << runtimeInSeconds.count() << " s" << std::endl;
	if (chord != nullptr) delete chord;
	reader.close();

	#ifdef DEBUG
	if (logger.is_open()) logger.close();
	#endif

	return 0;
}

void InitChord(const unsigned short &M, const Node::KEY &startID, Chord *&chord) {
	Node::M = M;
	if (chord != nullptr) delete chord;
	chord = new Chord(startID);
}

void AddPeer(const Node::KEY &ID, Chord *&chord) {
	if (chord->AddNode(ID))
		std::cout << "PEER " << ID << " ADDED" << std::endl;
	else
		std::cout << "UNABLE TO ADD PEER " << ID << std::endl;
}

void RemovePeer(const Node::KEY &ID, Chord *&chord) {
	if (chord->RemoveNode(ID))
		std::cout << "PEER " << ID << " REMOVED" << std::endl;
	else
		std::cout << "UNABLE TO REMOVE PEER " << ID << std::endl;
}

void FindKey(const Node::KEY &key, const Chord * const &chord) {
	auto result = chord->FindKey(key);
	if (result.second)
		std::cout << "PEER " << result.first << " FOUND" << std::endl;
	else
		std::cout << "NO PEER FOUND FOR KEY " << key << std::endl;
}

void Insert(const std::string &str, Chord *&chord) {
	auto result = chord->Put(str);
	if (result.second)
		std::cout << "INSERTED " << str << " (key=" << Utility::Hash(str, Node::M) << ")" << " AT " << result.first << std::endl;
	else
		std::cout << "UNABLE TO INSERT " << str << " (key=" << Utility::Hash(str, Node::M) << ")" << std::endl;
}

void Delete(const std::string &str, Chord *&chord) {
	auto result = chord->Delete(str);
	if (result.second)
		std::cout << "REMOVED " << str << " (key=" << Utility::Hash(str, Node::M) << ")" << " FROM " << result.first << std::endl;
	else
		std::cout << "UNABLE TO REMOVE " << str << " (key=" << Utility::Hash(str, Node::M) << ")" << std::endl;
}

void Print(const Node::KEY &key, const Chord * const &chord) {
	chord->PrintNode(key);
}