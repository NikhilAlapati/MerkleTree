#pragma once
#include "iostream"
#include "cryptlib.h"
#include "string"
#include "sha.h"
using namespace std;
using namespace CryptoPP;

class RedBlackTree {
 public:
	// Constructor/ Destructor
	RedBlackTree();
	~RedBlackTree();
	// Public Insert
	virtual void Insert(const byte data[], const unsigned int& key);
 protected:
	// Struct to keep track of the colors
	struct bitStruct {
		unsigned char B0: 1, B1: 1, B2: 1, B3: 1, B4: 1, B5: 1, B6: 1, B7: 1;
		friend ostream& operator<<(ostream& os, const bitStruct& s) {
			os << (unsigned short)s.B7 << "," << (unsigned short)s.B6 << "," << (unsigned short)s.B5 << ","
			   << (unsigned short)s.B4 << "," << (unsigned short)s.B3 << "," << (unsigned short)s.B2 << ","
			   << (unsigned short)s.B1 << "," << (unsigned short)s.B0;
			return os;
		}
	};
	struct Node {
		explicit Node(unsigned int key);
		Node();
		byte Data[CryptoPP::SHA256::DIGESTSIZE];
		Node* Left;
		Node* Right;
		Node* Parent;
		unsigned int Key;
	};
	// Vector of colors
	vector<bitStruct> Colors;
	// Private Helpers
	void BalanceInsert(Node* insNode);
	void RightRotate(Node* node);
	void LeftRotate(Node* node);
	void DestructorHelper(Node* node);
	void SetColor(unsigned int key, bool color);
	bool GetColor(unsigned int key);
	// Root
	Node* Root;
};