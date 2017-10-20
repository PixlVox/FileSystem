#ifndef TREESTRUCTURE_H
#define TREESTRUCTURE_H

#include <string>

class TreeStructure {

protected:

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
	void goToNextDir(int index, int nrOfSubs, int* subs);
	const int* getCurrentSubs() const;
	int getNrOfCurrentSubs() const;
};

#endif // !TREESTRUCTURE_H
