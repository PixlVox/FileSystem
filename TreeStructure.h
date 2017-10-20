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
<<<<<<< HEAD
	int* getCurrentSubs() const;
	int getNrOfCurrentSubs() const;
=======
	const int* getCurrentSubs() const;
	int getNrOfCurrentSubs();
>>>>>>> f801c485f4d285bf3acae1d99ff547f3646edef9
};

#endif // !TREESTRUCTURE_H
