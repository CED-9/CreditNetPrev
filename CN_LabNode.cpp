// CN_LabNode.C 

#include"Error.h"
#include"CN_Node.h"
#include<queue>
#include<vector>
#include<iostream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Labor Nodes */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LabNode::LabNode(){
	setNodeID(-1);
	setNodeType();
}

void LabNode::setLabNode(int id){
	nodeID = id;
	setNodeType();
}

void LabNode::print(){
	cout << "LabNode" << nodeID << "lab_con_in" << endl;
	for (int i = 0; i < lab_con_in.size(); i++){
		cout << lab_con_in[i].node2->getNodeID() << lab_con_in[i].d_out_current << " / "
			<< lab_con_in[i].c_in_max << ",";
	}
	cout << endl;
	return;
}


/* Find (current debt) from other node
* not found = 0
*/
int LabNode::getDebtFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<lab_fin_out.size(); i++){
			if (lab_fin_out[i].node2 == nodeT){
				return lab_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<lab_con_out.size(); i++){
			if (lab_con_out[i].node2 == nodeT){
				return lab_con_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Find (credit remaining) from other node
* not found = 0
*/
int LabNode::getCreditFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<lab_fin_in.size(); i++){
			if (lab_fin_in[i].node2 == nodeT){
				return lab_fin_in[i].c_in_max - lab_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<lab_con_in.size(); i++){
			if (lab_con_in[i].node2 == nodeT){
				return lab_con_in[i].c_in_max - lab_con_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Find (current debt) to other node
* not found = 0
*/
int LabNode::getDebtTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<lab_fin_in.size(); i++){
			if (lab_fin_in[i].node2 == nodeT){
				return lab_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<lab_con_in.size(); i++){
			if (lab_con_in[i].node2 == nodeT){
				return lab_con_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Set (credit remained) to other node
* not found = 0
*/
int LabNode::getCreditTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<lab_fin_out.size(); i++){
			if (lab_fin_out[i].node2 == nodeT){
				return lab_fin_out[i].c_out_max - lab_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<lab_con_out.size(); i++){
			if (lab_con_out[i].node2 == nodeT){
				return lab_con_out[i].c_out_max - lab_con_out[i].d_in_current;
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
void LabNode::setDebtFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < lab_fin_out.size(); i++){
				if (lab_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_fin_out[i].d_in_current += value; break;
					case SUB:
						lab_fin_out[i].d_in_current -= value; break;
					case EQ:
						lab_fin_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < lab_con_out.size(); i++){
				if (lab_con_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_con_out[i].d_in_current += value; break;
					case SUB:
						lab_con_out[i].d_in_current -= value; break;
					case EQ:
						lab_con_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else {
			throw NODE_NOT_REACHABLE; 
		}
	}
	catch (Status error){
		cout << "something is wrong :(" << endl;
	}
	return;
}

/* Set (credit remained) from other node
* Input : value >= d_out_current
* Modify : c_in_max = value
* must be valid
*/
void LabNode::setCreditFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < lab_fin_in.size(); i++){
				if (lab_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_fin_in[i].c_in_max += value; break;
					case SUB:
						lab_fin_in[i].c_in_max -= value; break;
					case EQ:
						lab_fin_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < lab_con_in.size(); i++){
				if (lab_con_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_con_in[i].c_in_max += value; break;
					case SUB:
						lab_con_in[i].c_in_max -= value; break;
					case EQ:
						lab_con_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else {
			throw NODE_NOT_REACHABLE; 
		}
	}
	catch (Status error){
		cout << "something is wrong :(" << endl;
	}
	return;
}

/* Set (current debt) to other node
* Input : value <= c_in_max
* Modify : d_out_current = value
* must be valid
*/
void LabNode::setDebtTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < lab_fin_in.size(); i++){
				if (lab_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_fin_in[i].d_out_current += value; break;
					case SUB:
						lab_fin_in[i].d_out_current -= value; break;
					case EQ:
						lab_fin_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < lab_con_in.size(); i++){
				if (lab_con_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_con_in[i].d_out_current += value; break;
					case SUB:
						lab_con_in[i].d_out_current -= value; break;
					case EQ:
						lab_con_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else {
			throw NODE_NOT_REACHABLE; 
		}
	}
	catch (Status error){
		cout << "something is wrong :(" << endl;
	}
	return;
}

/* Set (credit remained) to other node
* Input : value >= d_in_current
* Modify : c_out_max = value
* must be valid
*/
void LabNode::setCreditTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < lab_fin_out.size(); i++){
				if (lab_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_fin_out[i].c_out_max += value; break;
					case SUB:
						lab_fin_out[i].c_out_max -= value; break;
					case EQ:
						lab_fin_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < lab_con_out.size(); i++){
				if (lab_con_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						lab_con_out[i].c_out_max += value; break;
					case SUB:
						lab_con_out[i].c_out_max -= value; break;
					case EQ:
						lab_con_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else {
			throw NODE_NOT_REACHABLE; 
		}
	}
	catch (Status error){
		cout << "something is wrong :(" << endl;
	}
	return;
}
