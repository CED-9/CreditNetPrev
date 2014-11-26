#ifndef CN_Node
#define CN_Node

#include<vector>
#include<list>

enum NodeType{
	FINANCIAL, CONSUMER, PRODUCER, BANK, LABOR
};

enum EdgeType{
	D_IN, D_OUT, C_IN, C_OUT
};

enum ChangeNode{
	ADD, SUB, EQ
};

class Node;

struct InEdge{
	Node* node2;
	NodeType node2Type;
	int c_in_max;
	int d_out_current;
	double interest_rate;
};

struct OutEdge{
	Node* node2;
	NodeType node2Type;
	int c_out_max;
	int d_in_current;
	double interest_rate;
};

class Node{
public:
	NodeType nodeType;
	int nodeID;

	virtual void setNodeType() = 0;
	virtual NodeType getNodeType() = 0;
	void setNodeID(int id){ nodeID = id; }
	int getNodeID(){ return nodeID; }

	// return remained credit or current debt 
	// not found = 0
	virtual int getDebtFrom(Node* nodeT) = 0;
	virtual int getDebtTo(Node* nodeT) = 0;
	virtual int getCreditFrom(Node* nodeT) = 0;
	virtual int getCreditTo(Node* nodeT) = 0;

	// set total credit or current debt 
	// no invalid change 
	virtual void setDebtFrom(Node* nodeT, int value, ChangeNode mode) = 0;
	virtual void setDebtTo(Node* nodeT, int value, ChangeNode mode) = 0;
	virtual void setCreditFrom(Node* nodeT, int value, ChangeNode mode) = 0;
	virtual void setCreditTo(Node* nodeT, int value, ChangeNode mode) = 0;
};

class BanNode : public Node{
public:
	// To financial, 
	std::vector<OutEdge> ban_fin_out;
	std::vector<InEdge> ban_fin_in;

	void setNodeType(){ nodeType = BANK;  }
	NodeType getNodeType(){ return nodeType; }

	BanNode();
	void setBanNode(int id); 

	// return remained credit or current debt 
	// not found = 0
	int getDebtFrom(Node* nodeT);
	int getDebtTo(Node* nodeT);
	int getCreditFrom(Node* nodeT);
	int getCreditTo(Node* nodeT);

	// set total credit or current debt 
	// no invalid change 
	void setDebtFrom(Node* nodeT, int value, ChangeNode mode);
	void setDebtTo(Node* nodeT, int value, ChangeNode mode);
	void setCreditFrom(Node* nodeT, int value, ChangeNode mode);
	void setCreditTo(Node* nodeT, int value, ChangeNode mode);

	void print();
};

class LabNode : public Node{
public:

	// To consumer, 
	std::vector<OutEdge> lab_con_out;
	std::vector<InEdge> lab_con_in;

	// To financial, 
	std::vector<OutEdge> lab_fin_out;
	std::vector<InEdge> lab_fin_in;

	void setNodeType(){ nodeType = LABOR; }
	NodeType getNodeType(){ return nodeType; }

	LabNode();
	void setLabNode(int id); 

	// return remained credit or current debt 
	// not found = 0
	int getDebtFrom(Node* nodeT);
	int getDebtTo(Node* nodeT);
	int getCreditFrom(Node* nodeT);
	int getCreditTo(Node* nodeT);

	// set total credit or current debt 
	// no invalid change 
	void setDebtFrom(Node* nodeT, int value, ChangeNode mode);
	void setDebtTo(Node* nodeT, int value, ChangeNode mode);
	void setCreditFrom(Node* nodeT, int value, ChangeNode mode);
	void setCreditTo(Node* nodeT, int value, ChangeNode mode);

	void print();
};

class ConNode : public Node{
public:
	int laborToProvide;
	int prevIncome;
	int moneyToSpend;

	// To labor, in default 1 labor market
	std::vector<OutEdge> con_lab_out;
	std::vector<InEdge> con_lab_in;

	// To financial, 
	std::vector<OutEdge> con_fin_out;
	std::vector<InEdge> con_fin_in;

	void setNodeType(){ nodeType = CONSUMER; }
	NodeType getNodeType(){ return nodeType; }

	ConNode();
	void setConNode(int id); 

	// return remained credit or current debt 
	// not found = 0
	int getDebtFrom(Node* nodeT);
	int getDebtTo(Node* nodeT);
	int getCreditFrom(Node* nodeT);
	int getCreditTo(Node* nodeT);

	// set total credit or current debt 
	// no invalid change 
	void setDebtFrom(Node* nodeT, int value, ChangeNode mode);
	void setDebtTo(Node* nodeT, int value, ChangeNode mode);
	void setCreditFrom(Node* nodeT, int value, ChangeNode mode);
	void setCreditTo(Node* nodeT, int value, ChangeNode mode);

	void print();
};

class ProNode : public Node{
public:
	int capital;
	int laborCost;

	// To financial, 
	std::vector<OutEdge> pro_fin_out;
	std::vector<InEdge> pro_fin_in;

	void setNodeType(){ nodeType = PRODUCER; }
	NodeType getNodeType(){ return nodeType; }

	ProNode();
	void setProNode(int id); 

	// return remained credit or current debt 
	// not found = 0
	int getDebtFrom(Node* nodeT);
	int getDebtTo(Node* nodeT);
	int getCreditFrom(Node* nodeT);
	int getCreditTo(Node* nodeT);

	// set total credit or current debt 
	// no invalid change 
	void setDebtFrom(Node* nodeT, int value, ChangeNode mode);
	void setDebtTo(Node* nodeT, int value, ChangeNode mode);
	void setCreditFrom(Node* nodeT, int value, ChangeNode mode);
	void setCreditTo(Node* nodeT, int value, ChangeNode mode);

	void print();
};

class FinNode : public Node{
public:
	// To consumer, 
	std::vector<OutEdge> fin_con_out;
	std::vector<InEdge> fin_con_in;

	// To producer, 
	std::vector<OutEdge> fin_pro_out;
	std::vector<InEdge> fin_pro_in;

	// To financial, 
	std::vector<OutEdge> fin_fin_out;
	std::vector<InEdge> fin_fin_in;

	// To labor, 
	std::vector<OutEdge> fin_lab_out;
	std::vector<InEdge> fin_lab_in;

	// To bank, 
	std::vector<OutEdge> fin_ban_out;
	std::vector<InEdge> fin_ban_in;

	FinNode();
	void setFinNode(int id); 
	void setNodeType(){ nodeType = FINANCIAL; }
	NodeType getNodeType(){ return nodeType; }

	// return remained credit or current debt 
	// not found = 0
	int getDebtFrom(Node* nodeT);
	int getDebtTo(Node* nodeT);
	int getCreditFrom(Node* nodeT);
	int getCreditTo(Node* nodeT);

	// set total credit or current debt 
	// no invalid change 
	void setDebtFrom(Node* nodeT, int value, ChangeNode mode);
	void setDebtTo(Node* nodeT, int value, ChangeNode mode);
	void setCreditFrom(Node* nodeT, int value, ChangeNode mode);
	void setCreditTo(Node* nodeT, int value, ChangeNode mode);

	void print();
};



#endif
