// CN_ProNode.C 

#include"Error.h"
#include"CN_Node.h"
#include<queue>
#include<vector>
#include<iostream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Producer Nodes */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ProNode::ProNode(){
	setNodeID(-1);
	setNodeType();
}

void ProNode::setProNode(int id){
	nodeID = id;
	setNodeType();
}

void ProNode::print(){
	cout << "ProNode " << nodeID << " pro_fin_out: " << endl;
	for (int i = 0; i<pro_fin_out.size(); i++){
		cout << pro_fin_out[i].node2->getNodeID() << "  " << pro_fin_out[i].d_in_current << " / "
			<< pro_fin_out[i].c_out_max << ", ";
	}
	cout << endl;
}


/* Find (current debt) from other node
* not found = 0
*/
int ProNode::getDebtFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<pro_fin_out.size(); i++){
			if (pro_fin_out[i].node2 == nodeT){
				return pro_fin_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Find (credit remaining) from other node
* not found = 0
*/
int ProNode::getCreditFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<pro_fin_in.size(); i++){
			if (pro_fin_in[i].node2 == nodeT){
				return pro_fin_in[i].c_in_max - pro_fin_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Find (current debt) to other node
* not found = 0
*/
int ProNode::getDebtTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<pro_fin_in.size(); i++){
			if (pro_fin_in[i].node2 == nodeT){
				return pro_fin_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Set (credit remained) to other node
* not found = 0
*/
int ProNode::getCreditTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<pro_fin_out.size(); i++){
			if (pro_fin_out[i].node2 == nodeT){
				return pro_fin_out[i].c_out_max - pro_fin_out[i].d_in_current;
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
void ProNode::setDebtFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < pro_fin_out.size(); i++){
				if (pro_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						pro_fin_out[i].d_in_current += value; break;
					case SUB:
						pro_fin_out[i].d_in_current -= value; break;
					case EQ:
						pro_fin_out[i].d_in_current = value; break;
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
void ProNode::setCreditFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < pro_fin_in.size(); i++){
				if (pro_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						pro_fin_in[i].c_in_max += value; break;
					case SUB:
						pro_fin_in[i].c_in_max -= value; break;
					case EQ:
						pro_fin_in[i].c_in_max = value; break;
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
void ProNode::setDebtTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < pro_fin_in.size(); i++){
				if (pro_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						pro_fin_in[i].d_out_current += value; break;
					case SUB:
						pro_fin_in[i].d_out_current -= value; break;
					case EQ:
						pro_fin_in[i].d_out_current = value; break;
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
void ProNode::setCreditTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < pro_fin_out.size(); i++){
				if (pro_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						pro_fin_out[i].c_out_max += value; break;
					case SUB:
						pro_fin_out[i].c_out_max -= value; break;
					case EQ:
						pro_fin_out[i].c_out_max = value; break;
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
		cout << "something is wrong :(, operation not define" << endl;
	}
	return;
}

