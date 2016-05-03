#ifndef CHORD_SIMULATION_INCLUDE_NODE_H_INCLUDED
#define CHORD_SIMULATION_INCLUDE_NODE_H_INCLUDED

#include <unordered_set>

class Node {
public:

	typedef unsigned long KEY;
	typedef std::unordered_multiset<std::string> DataContainer;

	Node() = default;
	Node(const KEY &ID);
	~Node();

	void InitNode(const KEY &ID);
	void DestroyNode();

	const KEY &GetID() const;
	Node *&Successor();											// Guaranteed doesn't return nullptr
	Node *&Predecessor(); 										// May return null
	void SetSuccessor(Node *node);
	void SetPredecessor(Node *node);

	Node *FindNode(const KEY &ID); 								// Returns nullptr if not found
	Node *FindSuccessor(const KEY &ID); 						// Guaranteed doesn't return nullptr
	Node *FindSuccessor(const KEY &ID, std::ostream &out); 		// Guaranteed doesn't return nullptr

	void Create();
    void Join(Node *n1);
    void Stabilize();
    void FixFingers();
    // void FixFingers(const std::unordered_set<KEY> &invalidIDs);
    void ResetFingers();

    const KEY &Insert(const std::string &str);
    const KEY &Delete(const std::string &str);
    const bool HasData() const;
    void MoveDataToPredecessor();
    void MoveDataToSuccessor();

    void ShowMe(std::ostream &out) const;
	void ShowID(std::ostream &out) const;
	void ShowSuccPred(std::ostream &out) const;
	void ShowData(std::ostream &out) const;
	void ShowFingers(std::ostream &out) const;
	void ShowFingerStart(std::ostream &out) const;

	static const KEY MaxKey();
	static unsigned short M;

	// NON-CONCURRENT STYLE (AS PER DESCRIBED IN THE REFERENCES) FOR TESTING OPTIMIZATION
	Node *FindSuccessor2(const KEY &ID);
	Node *FindPredecessor(const KEY &ID);

	void Join2(Node *existingNode);
	void InitFingerTable(Node *existingNode);
	void UpdateOthers();
	void UpdateFingerTable(Node *s, const unsigned short &i);
	void CopyNodeInfo(Node *existingNode);

private:

	struct Finger {
		KEY ID;
		Node *node;
	};

	KEY ID; // node ID
	Node *succ; // successor node
	Node *pred; // predecessor node
	Finger *fingers; // finger table
	DataContainer data; // data items
	// std::unordered_set<KEY> _invalidIDs;

	Node *ClosestPrecedingFinger(const KEY &ID); 				// Guaranteed doesn't return nullptr

    void Notify(Node *n1);

	static const bool IsInRightClosedInterval(const long &in, const long &left, const long &right);
    static const bool IsInOpenInterval(const long &in, const long &left, const long &right);
    static const bool IsInLeftClosedInterval(const long &in, const long &left, const long &right);
	static const Node::KEY Hash(const std::string &s);
    static const KEY FingerStart(const KEY &ID, const unsigned short &i);
    void setFinger(const unsigned short &i, Node *node);

};

#endif