#include "RedBlackTree.h"
// Constructor
RedBlackTree::RedBlackTree() {
	Root = nullptr;
}
// Destructor
RedBlackTree::~RedBlackTree() {
	DestructorHelper(Root);
}
// Helper for Destructor
void RedBlackTree::DestructorHelper(RedBlackTree::Node* node) {
	// DFS deletes
	if (node) {
		DestructorHelper(node->Left);
		DestructorHelper(node->Right);
		delete node;
	}
}
// Inserts bytes of data and the key into the RB tree like a BST and then balances if necessary
void RedBlackTree::Insert(const byte data[], const unsigned int& key) {
	// Creates a new node with the key and makes it red because all new inserted nodes are red unless its incorrect
	Node* insNode = new Node(key);
	SetColor(key, true);
	// Gets the length of the data by using byte math
	if (data != nullptr) {
		memcpy_s(insNode->Data, CryptoPP::SHA256::DIGESTSIZE, data, CryptoPP::SHA256::DIGESTSIZE);
	}
	insNode->Left = nullptr;
	insNode->Right = nullptr;
	// If the root is empty then tree is empty so insert and return
	if (this->Root == nullptr) {
		this->Root = insNode;
		SetColor(key, false);
		return;
	}
	Node* parent = nullptr;
	Node* root = this->Root;
	// Goes to the correct spot
	while (root != nullptr) {
		parent = root;
		if (insNode->Key > root->Key) {
			root = root->Right;
		} else {
			root = root->Left;
		}
	}
	// Changes the parent and checks if weather to place it on the left or right
	insNode->Parent = parent;
	if (parent != nullptr) {
		if (insNode->Key < parent->Key) {
			parent->Left = insNode;
		} else {
			parent->Right = insNode;
		}
	}
	// If the its already balanced then return else balance
	if (insNode->Parent->Parent == nullptr) {
		return;
	}
	// Balances after inserting
	BalanceInsert(insNode);
}
// Balances the tree
void RedBlackTree::BalanceInsert(Node* insNode) {
	Node* uncle;
	// Exits the loop when true
	bool exit = false;
	while (!exit && GetColor(insNode->Parent->Key) == 1) {
		if (insNode->Parent == insNode->Parent->Parent->Right) {
			uncle = insNode->Parent->Parent->Left;
			// If parent and uncle are both red
			if (uncle != nullptr && GetColor(uncle->Key) == 1) {
				SetColor(uncle->Key, false);
				SetColor(insNode->Parent->Key, false);
				SetColor(insNode->Parent->Parent->Key, true);
				insNode = insNode->Parent->Parent;
			} else {
				if (insNode == insNode->Parent->Left) {
					insNode = insNode->Parent;
					RightRotate(insNode);
				}
				SetColor(insNode->Parent->Key, false);
				SetColor(insNode->Parent->Parent->Key, true);
				LeftRotate(insNode->Parent->Parent);
			}
		} else {
			uncle = insNode->Parent->Parent->Right;
			if (uncle != nullptr && GetColor(uncle->Key) == 1) {
				SetColor(uncle->Key, false);
				SetColor(insNode->Parent->Key, false);
				SetColor(insNode->Parent->Parent->Key, true);
				insNode = insNode->Parent->Parent;
			} else {
				if (insNode == insNode->Parent->Right) {
					insNode = insNode->Parent;
					LeftRotate(insNode);
				}
				SetColor(insNode->Parent->Key, false);
				SetColor(insNode->Parent->Parent->Key, true);
				RightRotate(insNode->Parent->Parent);
			}
		}
		if (insNode == Root) {
			exit = true;
		}
	}
	SetColor(Root->Key, false);

}
// Rotates right relative to the node passed in
void RedBlackTree::RightRotate(RedBlackTree::Node* node) {
	Node* temp = node->Left;
	node->Left = temp->Right;
	if (temp->Right != nullptr) {
		temp->Right->Parent = node;
	}
	temp->Parent = node->Parent;
	if (node->Parent == nullptr) {
		this->Root = temp;
	} else if (node == node->Parent->Right) {
		node->Parent->Right = temp;
	} else {
		node->Parent->Left = temp;
	}
	temp->Right = node;
	node->Parent = temp;
}
// Rotates left relative to the node passed in
void RedBlackTree::LeftRotate(RedBlackTree::Node* node) {
	Node* temp = node->Right;
	node->Right = temp->Left;
	if (temp->Left != nullptr) {
		temp->Left->Parent = node;
	}
	temp->Parent = node->Parent;
	if (node->Parent == nullptr) {
		this->Root = temp;
	} else if (node == node->Parent->Left) {
		node->Parent->Left = temp;
	} else {
		node->Parent->Right = temp;
	}
	temp->Left = node;
	node->Parent = temp;
}
// Sets color in the colors vector
void RedBlackTree::SetColor(unsigned int key, bool color) {
	unsigned int colorIndex;
	// Checks if its a 50 node or a 100 node
	if (key == 0) {
		colorIndex = 0;
	} else if (key % 100 == 0) {
		colorIndex = (key / 50) - 1;
	} else {
		colorIndex = (key / 50) + 1;
	}
	// Checks if you need to add a new struct
	if (colorIndex + 1 > (8 * Colors.size())) {
		bitStruct bit{};
		Colors.push_back(bit);
	}
	// Adds to the correct bit
	switch (colorIndex % 8) {
	case 0:
		Colors[colorIndex / 8].B0 = color;
		break;
	case 1:
		Colors[colorIndex / 8].B1 = color;
		break;
	case 2:
		Colors[colorIndex / 8].B2 = color;
		break;
	case 3:
		Colors[colorIndex / 8].B3 = color;
		break;
	case 4:
		Colors[colorIndex / 8].B4 = color;
		break;
	case 5:
		Colors[colorIndex / 8].B5 = color;
		break;
	case 6:
		Colors[colorIndex / 8].B6 = color;
		break;
	case 7:
		Colors[colorIndex / 8].B7 = color;
		break;
	}
}
// Returns a color in the colors vector
bool RedBlackTree::GetColor(unsigned int key) {
	unsigned int colorIndex;
	// Gets correct index of vector
	if (key == 0) {
		colorIndex = 0;
	} else if (key % 100 == 0) {
		colorIndex = (key / 50) - 1;
	} else {
		colorIndex = (key / 50) + 1;
	}
	// Gets correct byte
	switch (colorIndex % 8) {
	case 0:
		return Colors[colorIndex / 8].B0;
	case 1:
		return Colors[colorIndex / 8].B1;
	case 2:
		return Colors[colorIndex / 8].B2;
	case 3:
		return Colors[colorIndex / 8].B3;
	case 4:
		return Colors[colorIndex / 8].B4;
	case 5:
		return Colors[colorIndex / 8].B5;
	case 6:
		return Colors[colorIndex / 8].B6;
	case 7:
		return Colors[colorIndex / 8].B7;
	}
	return true;
}
// Node Constructors
RedBlackTree::Node::Node(unsigned int key) : Data{ 0 } {
	Left = nullptr;
	Right = nullptr;
	Parent = nullptr;
	Key = key;
}
RedBlackTree::Node::Node() : Data{ 0 } {

	Left = nullptr;
	Right = nullptr;
	Parent = nullptr;
	Key = 0;
}