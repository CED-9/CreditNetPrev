#ifndef CN_Graph
#define CN_Graph

#define NODE_MAX 1000

#include"CN_Node.h"
#include<list>

struct Transaction{
	ConNode* cnode;
	ProNode* pnode;
	int transValue;
};

class Graph{
public:
	BanNode banAgent;
	LabNode labAgent;
	ConNode conAgent[NODE_MAX];
	ProNode proAgent[NODE_MAX];
	FinNode finAgent[NODE_MAX];
	int finNum, conNum, proNum;

	Graph(int finNumT, int conNumT, int proNumT);
	void genErdosRenyiGraph();
	void print();

	std::list <Node*> pathMixed;
	int pathCapacityMixed();
	bool bfsMixed(Node* node1, Node* node2);
	void pathFillMixed(int cap);
	int maxFlowMixed(Node* fnode1, Node* fnode2);

};


#endif
