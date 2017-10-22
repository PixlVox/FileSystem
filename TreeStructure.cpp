#include "TreeStructure.h"
#include <iostream>

TreeStructure::TreeStructure() {

	this->currentDir = new Node(0, nullptr);

}

TreeStructure::~TreeStructure() {

	this->resetTree();
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
			
			//Deletes current node and its pointers
			delete[] this->currentDir->subs;
			delete this->currentDir;

			//Sets current to the temp pointer
			this->currentDir = temp;

		}

	}

	return error;

}

void TreeStructure::goToNextDir(int index, int nrOfSubs, int* subs) {

	Node* nextDir = new Node(index, this->currentDir);
	nextDir->nrOfSubs = nrOfSubs;
	nextDir->subs = subs;

	this->currentDir = nextDir;

}

void TreeStructure::setNewSub(int blockId) {

	//If there are no subs the array will be created with the new one
	if (this->currentDir->subs == nullptr) {

		this->currentDir->nrOfSubs = 1;
		this->currentDir->subs = new int[this->currentDir->nrOfSubs];
		this->currentDir->subs[0] = blockId;

	}
	else {	//If there are subs the array will be copied and resized with the new sub

		int* tempSubs = new int[(this->currentDir->nrOfSubs + 1)];

		for (int i = 0; i < this->currentDir->nrOfSubs; i++) {

			tempSubs[i] = this->currentDir->subs[i];

		}


		//Adding the new sub
		tempSubs[this->currentDir->nrOfSubs] = blockId;

		//Delete the old subs
		delete[] this->currentDir->subs;

		//Assign the new subs
		this->currentDir->subs = tempSubs;
		this->currentDir->nrOfSubs++;

	}

}

void TreeStructure::removeSub(int blockId) {

	//Creates a temp sub array that has room for one less
	int* tempSubs = new int[(this->currentDir->nrOfSubs - 1)];

	//Copy over all the subs exept for the one that is removed
	int counter = 0;
	for (int i = 0; i < this->currentDir->nrOfSubs; i++) {

		if (this->currentDir->subs[i] != blockId) {

			tempSubs[counter] = this->currentDir->subs[i];
			counter++;

		}

	}
	
	//Update the new variables
	this->currentDir->nrOfSubs--;
	delete[] this->currentDir->subs;
	this->currentDir->subs = tempSubs;

}

void TreeStructure::resetTree() {

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

	//Deletes the root nodes 
	if (this->currentDir->subs != nullptr) {

		delete[] this->currentDir->subs;
		this->currentDir->subs = nullptr;

	}

}

const int* TreeStructure::getCurrentSubs() const {

	return this->currentDir->subs;

}

int TreeStructure::getNrOfCurrentSubs() const{

	return this->currentDir->nrOfSubs;

}

int TreeStructure::getCurrentBlockId() const{

	return this->currentDir->blockId;

}

void TreeStructure::setNrOfSubs(int nr) {

	this->currentDir->nrOfSubs = nr;

}

int TreeStructure::getPreviousBlockId(int layers) {

	int blockId = -1;
	int counter = 0;

	Node* temp = this->currentDir;

	while (temp->previousDir != nullptr && counter != layers) {

		temp = temp->previousDir;
		counter++;

	}

	if (temp->previousDir != nullptr) {

		blockId = temp->blockId;

	}

	return blockId;

}