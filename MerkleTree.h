#pragma once
#include <iostream>
#include "cryptlib.h"
#include "RedBlackTree.h"
#include "sha.h"
using namespace std;
using namespace CryptoPP;
class MerkleTree : public RedBlackTree {
 public:
	// Constructor
	MerkleTree() = default;
	// Overridden function
	void Insert(const byte data[], const unsigned int& key);
	// Verifies if a passed in digest and the associated key exists and is valid
	bool Verify(const byte digest[], const unsigned int& key);
 private:
	// Calculates the hashes of all the tree after insert
	void CalculateHashes(Node* node);
	// Helper function that searches for a node using a key and returns it
	Node* SearchNode(Node* node, const unsigned int& key);
	// Verifies till the root
	bool VerifyUp(Node* node) const;
};

