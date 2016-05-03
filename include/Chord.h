#ifndef CHORD_SIMULATION_INCLUDE_CHORD_H_INCLUDED
#define CHORD_SIMULATION_INCLUDE_CHORD_H_INCLUDED

#include "Node.h"
#include <map>

class Chord {
public:

	Chord() = default;
	Chord(const Node::KEY &startID);
	~Chord();
	void InitChord(const Node::KEY &startID);

	const bool AddNode(const Node::KEY &ID);
	const bool RemoveNode(const Node::KEY &ID);
	const std::pair<Node::KEY, bool> Put(const std::string &str);
	const std::pair<Node::KEY, bool> Delete(const std::string &str);

	const std::pair<Node::KEY, bool> FindKey(const Node::KEY &key) const;
	void PrintNode(const Node::KEY &key) const;

	const bool CheckSuccessors() const;
	const bool CheckPredecessors() const;

	void ShowAll() const; // Mainly for debugging

	// NON-CONCURRENT STYLE (AS PER DESCRIBED IN THE REFERENCES) FOR TESTING OPTIMIZATION
	const bool AddNode2(const Node::KEY &ID);

private:

	std::map<Node::KEY, Node *> nodes;
	// std::unordered_set<Node::KEY> invalidIDs;

};

#endif