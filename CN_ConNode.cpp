// CN_ConNode.C 

#include"Error.h"
#include"CN_Node.h"
#include<queue>
#include<vector>
#include<iostream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Consumer Nodes */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ConNode::ConNode(){
	setNodeID(-1);
	setNodeType();
}

void ConNode::setConNode(int id){
	nodeID = id;
	setNodeType();
}

void ConNode::print(){
	cout << "ConNode " << nodeID << " con_fin_in: " << endl;
	for (int i = 0; i<con_fin_in.size(); i++){
		cout << con_fin_in[i].node2->getNodeID() << " " << con_fin_in[i].d_out_current
			<< " /" << con_fin_in[i].c_in_max << ", ";
	}
	cout << endl;
	cout << "ConNode " << nodeID << " con_fin_out: " << endl;
	for (int i = 0; i<con_fin_out.size(); i++){
		cout << con_fin_out[i].node2->getNodeID() << " " << con_fin_out[i].d_in_current
			<< " / " << con_fin_in[i].c_in_max << ", ";
	}
	cout << endl;
}



/* Find (current debt) from other node
* not found = 0
*/
int ConNode::getDebtFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<con_fin_out.size(); i++){
			if (con_fin_out[i].node2 == nodeT){
				return con_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<con_lab_out.size(); i++){
			if (con_lab_out[i].node2 == nodeT){
				return con_lab_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Find (credit remaining) from other node
* not found = 0
*/
int ConNode::getCreditFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<con_fin_in.size(); i++){
			if (con_fin_in[i].node2 == nodeT){
				return con_fin_in[i].c_in_max - con_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<con_lab_in.size(); i++){
			if (con_lab_in[i].node2 == nodeT){
				return con_lab_in[i].c_in_max - con_lab_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Find (current debt) to other node
* not found = 0
*/
int ConNode::getDebtTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<con_fin_in.size(); i++){
			if (con_fin_in[i].node2 == nodeT){
				return con_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<con_lab_in.size(); i++){
			if (con_lab_in[i].node2 == nodeT){
				return con_lab_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Set (credit remained) to other node
* not found = 0
*/
int ConNode::getCreditTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<con_fin_out.size(); i++){
			if (con_fin_out[i].node2 == nodeT){
				return con_fin_out[i].c_out_max - con_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<con_lab_out.size(); i++){
			if (con_lab_out[i].node2 == nodeT){
				return con_lab_out[i].c_out_max - con_lab_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Set (current debt) from other node
* Input : value <= c_out_max
* Modify : d_in_current = value
* must be valid
*/
void ConNode::setDebtFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < con_fin_out.size(); i++){
				if (con_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_fin_out[i].d_in_current += value; break;
					case SUB:
						con_fin_out[i].d_in_current -= value; break;
					case EQ:
						con_fin_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE; 
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < con_lab_out.size(); i++){
				if (con_lab_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_lab_out[i].d_in_current += value; break;
					case SUB:
						con_lab_out[i].d_in_current -= value; break;
					case EQ:
						con_lab_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE; 
					}
				}
			}
		}
	}
	catch (Status error){
		cout << "something is wrong :(, operation not define" << endl;
	}
	return;
}

/* Set (credit remained) from other node
* Input : value >= d_out_current
* Modify : c_in_max = value
* must be valid
*/
void ConNode::setCreditFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < con_fin_in.size(); i++){
				if (con_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_fin_in[i].c_in_max += value; break;
					case SUB:
						con_fin_in[i].c_in_max -= value; break;
					case EQ:
						con_fin_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < con_lab_in.size(); i++){
				if (con_lab_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_lab_in[i].c_in_max += value; break;
					case SUB:
						con_lab_in[i].c_in_max -= value; break;
					case EQ:
						con_lab_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
	}
	catch (Status error){
		cout << "something is wrong :(, operation not define" << endl;
	}
	return;
}

/* Set (current debt) to other node
* Input : value <= c_in_max
* Modify : d_out_current = value
* must be valid
*/
void ConNode::setDebtTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < con_fin_in.size(); i++){
				if (con_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_fin_in[i].d_out_current += value; break;
					case SUB:
						con_fin_in[i].d_out_current -= value; break;
					case EQ:
						con_fin_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < con_lab_in.size(); i++){
				if (con_lab_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_lab_in[i].d_out_current += value; break;
					case SUB:
						con_lab_in[i].d_out_current -= value; break;
					case EQ:
						con_lab_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
	}
	catch (Status error){
		cout << "something is wrong :(, operation not define" << endl;
	}
	return;
}

/* Set (credit remained) to other node
* Input : value >= d_in_current
* Modify : c_out_max = value
* must be valid
*/
void ConNode::setCreditTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < con_fin_out.size(); i++){
				if (con_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_fin_out[i].c_out_max += value; break;
					case SUB:
						con_fin_out[i].c_out_max -= value; break;
					case EQ:
						con_fin_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < con_lab_out.size(); i++){
				if (con_lab_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						con_lab_out[i].c_out_max += value; break;
					case SUB:
						con_lab_out[i].c_out_max -= value; break;
					case EQ:
						con_lab_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
	}
	catch (Status error){
		cout << "something is wrong :(, operation not define" << endl;
	}
	return;
}
