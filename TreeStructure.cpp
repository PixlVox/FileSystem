#include "TreeStructure.h"
#include <iostream>

TreeStructure::TreeStructure() {

	this->currentDir = new Node(0, nullptr);

}

TreeStructure::~TreeStructure() {

	Node* temp = nullptr;

	while (this->currentDir != nullptr && this->currentDir->previousDir != nullptr) {

		temp = this->currentDir->previousDir;
		delete this->currentDir;
		this->currentDir = temp;
		
	}

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

			this->currentDir = this->currentDir->previousDir;

		}

	}

	return error;

}

int TreeStructure::goToNextDir(int index) {



}

int TreeStructure::changeDir(std::string dirName) {

	int error = 0;

	if (this->currentDir == nullptr) {

		error = 1;

	}
	else if (dirName == "..") {

		if (this->currentDir->previousDir == nullptr) {

			error = 2;

		}
		else {

			this->currentDir = this->currentDir->previousDir;

		}

	}
	else {

		bool stop = false;

		for (int i = 0; i < this->currentDir->nrOfSubs && !stop; i++) {

			//Hantera data som hämtats från MemBlockDevice

		}

		if (!stop) {

			error = 3;

		}

	}

	return error;

}

int* TreeStructure::getCurrentSubs() const{

	return this->currentDir->subs;

}

int TreeStructure::getNrOfCurrentSubs() const{

	return this->currentDir->nrOfSubs;

}