#include "TreeStructure.h"
#include <iostream>

TreeStructure::TreeStructure() {

	this->currentDir = new Node(0, nullptr);

}

TreeStructure::~TreeStructure() {

	Node* temp = nullptr;

	while (this->currentDir != nullptr && this->currentDir->previousDir != nullptr) {

		//Create temp for previous folder
		temp = this->currentDir->previousDir;

		//Delete subs for current
		if (this->currentDir->subs != nullptr) {

			delete[] this->currentDir->subs;

		}

		//Delete current
		delete this->currentDir;

		//Set current to temp
		this->currentDir = temp;
		
	}

	//Deletes the root node
	delete this->currentDir;

}

int TreeStructure::goToPreviousDir() {

	int error = -1;

	if (this->currentDir == nullptr) {

		error = 1;

	}
	else {

		if (this->currentDir->previousDir == nullptr) {

			error = 2;

		}
		else {

			//Saves the previous dir in a temp pointer
			Node* temp = this->currentDir->previousDir;
			
			//Deletes current node
			delete this->currentDir;

			//Sets current to the temp pointer
			this->currentDir = temp;

		}

	}

	return error;

}

int TreeStructure::goToNextDir(int index, int nrOfSubs, int* subs) {

	Node* nextDir = new Node(index, this->currentDir);
	nextDir->nrOfSubs = nrOfSubs;
	nextDir->subs = subs;

	this->currentDir = nextDir;

}


const int* TreeStructure::getCurrentSubs() const {

	return this->currentDir->subs;

}

int TreeStructure::getNrOfCurrentSubs() const{

	return this->currentDir->nrOfSubs;

}