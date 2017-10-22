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
	void removeSub(int blockId);
	void resetTree();

	//Set
	void setNewSub(int blockId);
	void setNrOfSubs(int nr);

	//Get Functions
	const int* getCurrentSubs() const;
	int getNrOfCurrentSubs() const;
	int getCurrentBlockId() const;
	int getPreviousBlockId(int layers);

};

#endif // !TREESTRUCTURE_H
