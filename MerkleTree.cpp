#include "MerkleTree.h"
// Overridden insert
void MerkleTree::Insert(const byte data[], const unsigned int& key) {
	// Normal RB tree insert
	RedBlackTree::Insert(data, key);
	// If its not the first insert then it inserts an intermediate node and calcualtes hashes again
	if (key != 0) {
		RedBlackTree::Insert({}, key - 50);
		CalculateHashes(this->Root);
	}
}
// verifies if a digest assosiated with the key exists and is the same
bool MerkleTree::Verify(const byte digest[], const unsigned int& key) {
	// Searches for the node given the root
	Node* node = SearchNode(this->Root, key);
	// If the node is null that means that it doenst exist in the tree so it returns false
	if (node == nullptr) {
		return false;
	}
	// Compares the searched node's data with the digest and if its not equal then it returns false
	if (memcmp(node->Data, digest, CryptoPP::SHA256::DIGESTSIZE) != 0) {
		return false;
	}

	return VerifyUp(node);

}
bool MerkleTree::VerifyUp(RedBlackTree::Node* node) const {
	while (node != Root) {
		SHA256 hash;
		byte digestCheck[CryptoPP::SHA256::DIGESTSIZE];
		// If its the left sibling
		if (node == node->Parent->Left) {
			hash.Update(node->Data, CryptoPP::SHA256::DIGESTSIZE);
			hash.Update(node->Parent->Right->Data, CryptoPP::SHA256::DIGESTSIZE);
		} else if (node == node->Parent->Right) { // if its the right sibling
			hash.Update(node->Parent->Left->Data, CryptoPP::SHA256::DIGESTSIZE);
			hash.Update(node->Data, CryptoPP::SHA256::DIGESTSIZE);
		}
		hash.Final(digestCheck);
		// If its not the same it returns false
		if (memcmp(node->Parent->Data, digestCheck, CryptoPP::SHA256::DIGESTSIZE) != 0) {
			return false;
		}
		node = node->Parent;
	}

	// Returns true if none of the conditions failed
	return true;
}
// Searches for a node from the passed in key based on the passed in starting node
RedBlackTree::Node* MerkleTree::SearchNode(RedBlackTree::Node* node, const unsigned int& key) {
	// Standard Binary Tree Search
	if (node->Key == key) {
		return node;
	} else if (key < node->Key) {
		if (node->Left != nullptr) {
			return SearchNode(node->Left, key);
		}
	} else if (key > node->Key) {
		if (node->Right != nullptr) {
			return SearchNode(node->Right, key);
		}
	}
	return nullptr;
}
// Calculates hashes from the given node
void MerkleTree::CalculateHashes(RedBlackTree::Node* node) {
	if (node == nullptr) {
		return;
	}
	// Hash to store the new hash
	SHA256 newHash;
	// DFS
	// Recursive dive Left
	if (node->Left != nullptr && node->Left->Key % 100 != 0) {
		CalculateHashes(node->Left);
	}
	// Recursive Dive Right
	if (node->Right != nullptr && node->Right->Key % 100 != 0) {
		CalculateHashes(node->Right);
	}
	// Updates left if exists
	if (node->Left != nullptr) {
		newHash.Update(node->Left->Data, CryptoPP::SHA256::DIGESTSIZE);
	}
	// Updates right if exists
	if (node->Right != nullptr) {
		newHash.Update(node->Right->Data, CryptoPP::SHA256::DIGESTSIZE);
	}
	// Finalizes the node into the data
	newHash.Final(node->Data);
}
