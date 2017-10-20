#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include <string>

class TreeStructure {

private:

	class Node {

	public:

		int blockId;
		Node* previousDir;
		int* subs;
		int nrOfSubs;

	public:

		Node(int blockId, Node* previousDir) {

			this->blockId = blockId;
			this->previousDir = previousDir;
			this->nrOfSubs = 0;
			this->subs = nullptr;

		}

		~Node() {

			//Alla noder raderas i TreeStructure Destruktorn

		}

	};

	
private:

	Node* currentDir;

public:

	TreeStructure();
	~TreeStructure();

	int goToPreviousDir();
	int goToNextDir(int index);
	int changeDir(std::string dirName);
	const int* getCurrentSubs() const;
	int getNrOfCurrentSubs() const;
<<<<<<< HEAD
=======

>>>>>>> 73227b10c33f4f5f670c8a7669388f0130693e88
};

#endif // !TREESTRUCTURE_H
