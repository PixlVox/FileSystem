#include "TreeStructure.h"
#include <iostream>

TreeStructure::TreeStructure() {

	this->currentDir = new Node(0, nullptr, nullptr);

}

TreeStructure::~TreeStructure() {

	Node* temp = nullptr;

	while (this->currentDir->previousDir != nullptr) {

		temp = this->currentDir->previousDir;
		delete this->currentDir;
		this->currentDir = temp;
		
	}

	delete this->currentDir;

}

void TreeStructure::changeDir(std::string dirName) {

	

}