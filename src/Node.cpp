#include "Node.h"
#include <iostream>
#include "Utility.h"

unsigned short Node::M = 0;

Node::Node(const KEY &ID) {
	InitNode(ID);
}

Node::~Node() {
	DestroyNode();
}

void Node::InitNode(const KEY &ID) {
	this->ID = ID;
	// this->succ = nullptr;
	this->pred = nullptr;
	fingers = new Finger[M];
	data.reserve(M);

	for (auto i = 0; i < M; ++i) {
        setFinger(i, this);
	}
}

void Node::DestroyNode() {
	if (fingers != nullptr) delete [] fingers;
}

const Node::KEY &Node::GetID() const {
	return ID;
}

Node *&Node::Successor()
{
	return succ;
}

Node *&Node::Predecessor()
{
	return pred;
}

void Node::SetSuccessor(Node *node) {
    setFinger(0, node);
}

void Node::SetPredecessor(Node *node) {
	pred = node;
}

// void Node::SetMBit(const unsigned int &M) {
// 	Node::M = M;
// }

// const unsigned int &Node::GetMBit() {
// 	return M;
// }

Node *Node::FindNode(const KEY &ID) {
    Node *n1 = FindSuccessor(ID, std::cout);
    if (n1->ID != ID) {
        return nullptr;
    } else {
        return n1;
    }
}

Node *Node::FindSuccessor(const KEY &ID) {
	if (IsInRightClosedInterval(ID, this->ID, succ->ID) || this == succ) {
        return succ;
    } else {
        Node *n1 = ClosestPrecedingFinger(ID);
        if (n1 == this) {
            return n1;
        }
        return n1->FindSuccessor(ID);
    }
}

Node *Node::FindSuccessor(const KEY &ID, std::ostream &out) {
    out << this->ID << ">";
    if (IsInRightClosedInterval(ID, this->ID, succ->ID) || this == succ) {
        out << succ->ID << std::endl;
        return succ;
    } else {
        Node *n1 = ClosestPrecedingFinger(ID);
        if (n1 == this) {
            out << n1->ID << std::endl;
            return n1;
        }
        return n1->FindSuccessor(ID, out);
    }
}

Node *Node::ClosestPrecedingFinger(const KEY &ID) {
    for (auto i = M - 1; i >= 0; --i) {
        // if (!_invalidIDs.empty() && _invalidIDs.find(fingers[i].ID) != _invalidIDs.end()) continue;
        if (IsInOpenInterval(fingers[i].ID, this->ID, ID)) {
            return fingers[i].node;
        }
    }
    return this;
}

// NON-CONCURRENT MODE
Node *Node::FindSuccessor2(const KEY &ID) {
    Node *n1 = FindPredecessor(ID);
    return n1->succ;
}

// NON-CONCURRENT MODE
Node *Node::FindPredecessor(const KEY &ID) {
	Node *check = nullptr;
	Node *n1 = this;

	// bool test3 = n1->ID != n1->fingers[0].node->ID;
	// bool test = !(ID > n1->ID && ID <= n1->fingers[0].node->ID) && test3;
	// bool test2 = IsInLeftClosedInterval(ID, n1->fingers[0].node->ID, n1->ID) && test3;

	// std::cout << n1->ID << " " << ID << " " << n1->fingers[0].node->ID << " "
	// << (test ? "TRUE" : " FALSE") << " " << (test2 ? "TRUE" : " FALSE");

	// std::cout << "FindPredecessor: Called by " << this->ID << ", Finding: " << ID
	// 		  << ", Left: " << n1->fingers[0].node->ID << ", Right: " << n1->ID
	// 		  << ", Outcome: " << (IsInLeftClosedInterval(ID, n1->fingers[0].node->ID, n1->ID) ? "TRUE" : "FALSE")
	// 		  ;
	while (!IsInRightClosedInterval(ID, n1->ID, n1->succ->ID)) {
		check = n1;
		n1 = n1->ClosestPrecedingFinger(ID);
		if (n1 == check) break;
	}

	// std::cout << ", Pred: " << n1->ID << std::endl;


	return n1;
}

void Node::Create() {
    pred = nullptr;
    SetSuccessor(this);
}

void Node::Join(Node *n1) {
    pred = nullptr;
    SetSuccessor(n1->FindSuccessor(ID, std::cout));
}

void Node::Stabilize() {
    Node *x = succ->pred;
    if (x != nullptr && (IsInOpenInterval(x->ID, this->ID, succ->ID) || this == succ)) {
        SetSuccessor(x);
    }
    succ->Notify(this);
}

void Node::Notify(Node *n1) {
    if (pred == nullptr || IsInOpenInterval(n1->ID, pred->ID, this->ID)) {
        pred = n1;
    }
}

void Node::FixFingers() {
    for (auto i = 0; i < M; ++i) {
        setFinger(i, FindSuccessor(FingerStart(ID, i)));
    }
}

// void Node::FixFingers(const std::unordered_set<KEY> &invalidIDs) {
//     _invalidIDs.insert(std::begin(invalidIDs), std::end(invalidIDs));
//     if (_invalidIDs.empty()) return;
//     for (auto i = 0; i < M; ++i) {
//         if (_invalidIDs.find(fingers[i].ID) != _invalidIDs.end()) {
//             setFinger(i, FindSuccessor(FingerStart(ID, i)));
//         }
//     }

//     for (auto i = 0; i < M; ++i) {
//         if (_invalidIDs.find(fingers[i].ID) != _invalidIDs.end()) {
//             _invalidIDs.erase(fingers[i].ID);
//         }
//     }
// }

void Node::ResetFingers() {
    for (auto i = 0; i < M; ++i) {
        setFinger(i, this);
    }
}

// NON-CONCURRENT MODE
void Node::Join2(Node *existingNode) {
	if (existingNode) {
        // if (fingers != nullptr) delete [] fingers;
        // fingers = new Finger[M];
		InitFingerTable(existingNode);
		// std::cout << "############### Start UpdateOthers() ###############" << std::endl << std::endl;
		// UpdateOthers();
		// std::cout << "############### End UpdateOthers() ###############" << std::endl << std::endl;
		//TODO Move keys
	} else {
        for (auto i = 0; i < M; ++i) {
            setFinger(i, this);
        }
		pred = this;
	}
}

// NON-CONCURRENT MODE
void Node::InitFingerTable(Node *existingNode) {
	// fingers[0].node = existingNode->FindSuccessor(fingers[0].start);
    // SetSuccessor(existingNode->FindSuccessor2(ID));
    SetSuccessor(existingNode);
    pred = succ->pred;
    // std::cout << "InitFingerTable Called by: " << Utility::ColorOut(ID, Color::GREEN) << " "
              // << "My successor: " << Utility::ColorOut(succ->ID, Color::BLUE) << std::endl;
    // CopyNodeInfo(succ);

    // pred = succ->pred;
    // pred->succ = this;
    succ->pred->succ = this;
    succ->pred = this;

    // for (auto i = 1; i < M; ++i) {
        // if (IsInLeftClosedInterval(FingerStart(ID, i + 1), ID, fingers[i].ID)) {
            // fingers[i + 1].node = fingers[i].node;
            // setFinger(i + 1, fingers[i].node);
        // } else {
            // fingers[i + 1].node = existingNode->FindSuccessor2(FingerStart(ID, i + 1));
            // setFinger(i, existingNode->FindSuccessor2(FingerStart(ID, i)));
        // }
    // }

	// pred = fingers[0].node->pred;
	// fingers[0].node->pred = this;
	// for (auto i = 0; i < M - 1; ++i) {
	// 	if (IsInLeftClosedInterval(fingers[i + 1].start, ID, fingers[i].node->ID)) {
	// 		fingers[i + 1].node = fingers[i].node;
	// 	} else {


	// 		fingers[i + 1].node = existingNode->FindSuccessor(fingers[i + 1].start);
	// 	}
	// }
}

// NON-CONCURRENT MODE
void Node::UpdateOthers() {
	for (auto i = 0; i < M; ++i) {
		Node *p = FindPredecessor((ID - (1UL << i)) & ((1UL << M) - 1));

		std::cout << "UpdateOthers: Called by " << Utility::ColorOut(ID, Color::GREEN)
                  << ", p: " << Utility::ColorOut(p->ID, Color::RED)
                  << ", ith: " << ((ID - (1UL << i)) & ((1UL << M) - 1))
		          << ", i: " << i << std::endl;

		p->UpdateFingerTable(this, i);

		// std::cout << std::endl;
	}
}

// NON-CONCURRENT MODE
void Node::UpdateFingerTable(Node *s, const unsigned short &i) {
    std::cout << "UpdateFingerTable: Called by " << Utility::ColorOut(ID, Color::RED)
              << ", s: " << Utility::ColorOut(s->ID, Color::GREEN)
              << ", finger id: " << fingers[i].node->ID
              << ", i: " << i
              << ", condition: " << (IsInLeftClosedInterval(s->ID, ID, fingers[i].ID) ? "TRUE" : "FALSE") << std::endl;

    // if (IsInLeftClosedInterval(s->ID, ID, fingers[i].ID)) {
    if (!IsInLeftClosedInterval(s->ID, ID, fingers[i].ID)) {
        setFinger(i, s);
        Node *p = pred;
        p->UpdateFingerTable(s, i);
    }
}

// NON-CONCURRENT MODE
void Node::CopyNodeInfo(Node *existingNode) {
    pred = existingNode->pred;
    for (auto i = 1; i < M; ++i) {
        setFinger(i, existingNode->fingers[i].node);
    }
}

const Node::KEY &Node::Insert(const std::string &str) {
    Node *target = FindSuccessor(Hash(str), std::cout);
    target->data.insert(str);
    return target->ID;
}

const Node::KEY &Node::Delete(const std::string &str) {
    Node *target = FindSuccessor(Hash(str), std::cout);
    target->data.erase(str);
    return target->ID;
}

const bool Node::HasData() const {
    return !data.empty();
}

void Node::MoveDataToPredecessor() {
    // Check for data items that needs moving
    for (auto it = data.begin(); it != data.end(); ) {
        if (IsInRightClosedInterval(Hash(*it), pred->pred->ID, pred->ID)) {
            pred->data.insert(*it);
            it = data.erase(it);
        } else {
            ++it;
        }
    }
}

void Node::MoveDataToSuccessor() {
    // Move away ho~
    if (succ->data.empty()) {
        succ->data = std::move(data);
    } else {
        succ->data.reserve(succ->data.size() + data.size());
        std::move(std::begin(data), std::end(data), std::inserter(succ->data, succ->data.begin()));
        data.clear();
    }
}

void Node::ShowMe(std::ostream &out) const {
    out << "PRED: " << (pred ? Utility::ColorOut(pred->ID, Color::RED) : "NULL") << " ";
    out << "NODE ID: " << Utility::ColorOut(ID, Color::GREEN) << " ";
    out << "SUCC: " << Utility::ColorOut(succ->ID, Color::BLUE) << std::endl;
}

void Node::ShowID(std::ostream &out) const {
	out << "NODE ID: " << ID << std::endl;
}

void Node::ShowSuccPred(std::ostream &out) const {
	out << "PRED: ";
	out << (pred ? std::to_string(pred->ID) : "NULL");
    out << ", SUCC: ";
    out << succ->ID;
    out << std::endl;
}

void Node::ShowData(std::ostream &out) const {
	out << "DATA AT INDEX NODE " << ID << ":" << std::endl;
    for (auto s : data) {
        out << s << std::endl;
    }
}

void Node::ShowFingers(std::ostream &out) const {
	out << "FINGER TABLE OF NODE " << ID << ":" << std::endl;
    for (auto i = 0; i < M; ++i) {
        out << fingers[i].ID << " ";
    }
    out << std::endl;
}

void Node::ShowFingerStart(std::ostream &out) const {
	out << "FINGER START OF NODE " << ID << ":" << std::endl;
    for (auto i = 0; i < M; ++i) {
        out << (FingerStart(ID, i)) << " ";
    }
    out << std::endl;
}

const Node::KEY Node::MaxKey() {
    return (1UL << M) - 1;
}

const bool Node::IsInRightClosedInterval(const long &in, const long &left, const long &right) {
    if (left > right) {
        return in <= right || in > left;
    } else if (left == right) {
    	return false;
    } else {
        return in > left && in <= right;
    }
}

const bool Node::IsInOpenInterval(const long &in, const long &left, const long &right) {
    if (left > right) {
        return in < right || in > left;
    } else if (left == right) {
    	return false;
    } else {
        return in > left && in < right;
    }
}

const bool Node::IsInLeftClosedInterval(const long &in, const long &left, const long &right) {
    if (left > right) {
        return in < right || in >= left;
    } else if (left == right) {
    	return false;
    } else {
        return in >= left && in < right;
    }
}

const Node::KEY Node::Hash(const std::string &str) {
    return Utility::Hash(str, M);
}

const Node::KEY Node::FingerStart(const KEY &ID, const unsigned short &i) {
	return (ID + (1UL << i)) & ((1UL << M) - 1);
}

void Node::setFinger(const unsigned short &i, Node *node) {
    fingers[i].node = node;
    fingers[i].ID = node->ID;
    if (i == 0) succ = node;
}