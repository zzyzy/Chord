#include "Chord.h"
#include <iostream>

Chord::Chord(const Node::KEY &startID) {
	InitChord(startID);
}

Chord::~Chord() {
	for (auto n : nodes) {
		delete n.second;
	}
}

void Chord::InitChord(const Node::KEY &startID) {
	if (startID > Node::MaxKey()) return;
	Node *startNode = new Node(startID);
	if (!nodes.insert({startID, startNode}).second) {
		delete startNode;
		return;
	}
	startNode->Create();
}

const bool Chord::AddNode(const Node::KEY &ID) {
	if (ID > Node::MaxKey()) return false;

	Node *startNode = nullptr;
	Node *newNode = new Node(ID);

	if (!nodes.empty()) startNode = nodes.begin()->second;

	if (!nodes.insert({newNode->GetID(), newNode}).second) {
		delete newNode;
		return false;
	}

	if (nodes.size() == 1) {		// I'm the only one
		newNode->Create();
		return true;
	} else {						// There's someone else here, I have to say hi
		newNode->Join(startNode);
	}

	newNode->Stabilize();
	newNode->FixFingers(); //TODO Think of a way to fix fingers faster, B-Tree O(log n) maybe?

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        it->second->Stabilize();
        it->second->FixFingers();
    }

    // invalidIDs.clear();

    // I'm not going to move data to myself lol
    if (newNode->Successor()->HasData()) {
    	if (newNode != newNode->Successor()) newNode->Successor()->MoveDataToPredecessor();
    }

    return true;
}

const bool Chord::RemoveNode(const Node::KEY &ID) {
	if (nodes.empty()) return false;
	if (ID > Node::MaxKey()) return false;

	// Where am I oh where am I~?
	Node *startNode = nodes.begin()->second;
	Node *nodeToRemove = startNode->FindNode(ID);
	if (nodeToRemove == nullptr) return false;

	// I'm the last node that will be disappearing!!
	if (nodes.size() == 1) {
		nodes.erase(ID);
		delete nodeToRemove;
		return true;
	}

	// If I'm not the last one oh oh
	if (nodes.size() == 2) { 	// There's only the two of us!!
		nodeToRemove->Predecessor()->SetSuccessor(nodeToRemove->Successor());
		nodeToRemove->Successor()->SetPredecessor(nullptr);
	} else { 					// There's plenty of us
		nodeToRemove->Predecessor()->SetSuccessor(nodeToRemove->Successor());
		nodeToRemove->Successor()->SetPredecessor(nodeToRemove->Predecessor());
	}

	// I'm only going to move my stuff to other person, not myself
	if (nodeToRemove->HasData()) {
		if (nodeToRemove != nodeToRemove->Successor()) nodeToRemove->MoveDataToSuccessor();
	}

	nodes.erase(ID);

	if (nodes.size() == 1) {
		nodes.begin()->second->ResetFingers();
	} else {
		// invalidIDs.insert(ID);
		for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
	        it->second->Stabilize();
	        // it->second->FixFingers(invalidIDs);
	        it->second->FixFingers();
	    }
		// invalidIDs.clear();
	}

	delete nodeToRemove;

	return true;
}

const std::pair<Node::KEY, bool> Chord::Put(const std::string &str) {
	if (nodes.empty()) return std::make_pair(0, false);
	Node *startNode = nodes.begin()->second;
	return std::make_pair(startNode->Insert(str), true);
}

const std::pair<Node::KEY, bool> Chord::Delete(const std::string &str) {
	if (nodes.empty()) return std::make_pair(0, false);
	Node *startNode = nodes.begin()->second;
	return std::make_pair(startNode->Delete(str), true);
}

const std::pair<Node::KEY, bool> Chord::FindKey(const Node::KEY &key) const {
	if (nodes.empty()) return std::make_pair(0, false);
	if (key > Node::MaxKey()) return std::make_pair(0, false);
	Node *startNode = nodes.cbegin()->second;
	Node *foundNode = startNode->FindSuccessor(key, std::cout);
	return std::make_pair(foundNode->GetID(), true);
}

void Chord::PrintNode(const Node::KEY &key) const {
	if (nodes.empty()) return;
	if (key > Node::MaxKey()) return;
	Node *startNode = nodes.begin()->second;
	Node *foundNode = startNode->FindSuccessor(key, std::cout);
	foundNode->ShowData(std::cout);
	foundNode->ShowFingers(std::cout);
}

const bool Chord::CheckSuccessors() const {
	if (nodes.empty()) return true;

	for (auto it = nodes.begin(); it != std::prev(nodes.end()); ++it) {
		Node *prev = it->second;
		Node *next = std::next(it)->second;
		if (prev->Successor()->GetID() != next->GetID()) {
			return false;
		}
	}

	return true;
}

const bool Chord::CheckPredecessors() const {
	if (nodes.empty()) return true;

	for (auto it = nodes.rbegin(); it != std::prev(nodes.rend()); ++it) {
		Node *prev = it->second;
		Node *next = std::next(it)->second;
		if (prev->Predecessor() == nullptr) return false;
		if (prev->Predecessor()->GetID() != next->GetID()) {
			return false;
		}
	}

	return true;
}

void Chord::ShowAll() const {
	for (auto n : nodes) {
		#ifdef DEBUG
		std::cout << "#########################################################################" << std::endl;
		std::cout << "Map Node ID: " << n.second->GetID() << " ";
		n.second->ShowMe(std::cout);
		std::cout << "#########################################################################" << std::endl;
		#endif
		n.second->ShowFingers(std::cout);
		n.second->ShowData(std::cout);
		// n.second->ShowFingerStart();
	}
}

const bool Chord::AddNode2(const Node::KEY &ID) {
	if (ID > Node::MaxKey()) return false;

	Node *startNode = nullptr;
	// Node *endNode = nullptr;
	Node *newNode = new Node(ID);

	if (!nodes.empty()) startNode = nodes.begin()->second;
	// if (!nodes.empty()) endNode = nodes.end()->second;

	if (!nodes.insert({ID, newNode}).second) {
		delete newNode;
		return false;
	}

	newNode->Join2(startNode);

	// newNode->Stabilize();
	// newNode->FixFingers();

    // for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
    //     it->second->Stabilize();
    //     it->second->FixFingers();
    // }

    // invalidIDs.clear();

    // I'm not going to move data to myself lol
    // if (newNode->Successor()->HasData()) {
    // 	if (newNode != newNode->Successor()) newNode->Successor()->MoveDataToPredecessor();
    // }

    return true;
}