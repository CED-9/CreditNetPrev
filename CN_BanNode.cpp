// CN_BanNode.C 

#include"Error.h"
#include"CN_Node.h"
#include<queue>
#include<vector>
#include<iostream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Bank Nodes */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BanNode::BanNode(){
	setNodeID(-1);
	setNodeType();
}

void BanNode::setBanNode(int id){
	nodeID = id; 
	setNodeType(); 
}

void BanNode::print(){
	cout << "BanNode" << nodeID << "ban_fin_out" << endl; 
	for (int i = 0; i < ban_fin_out.size(); i++){
		cout << ban_fin_out[i].node2->getNodeID() << ban_fin_out[i].d_in_current << " / "
			<< ban_fin_out[i].c_out_max << ", "; 
	}
	cout << endl; 
}

/* Find (current debt) from other node
* not found = 0
*/
int BanNode::getDebtFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<ban_fin_out.size(); i++){
			if (ban_fin_out[i].node2 == nodeT){
				return ban_fin_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Find (credit remaining) from other node
* not found = 0
*/
int BanNode::getCreditFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<ban_fin_in.size(); i++){
			if (ban_fin_in[i].node2 == nodeT){
				return ban_fin_in[i].c_in_max - ban_fin_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Find (current debt) to other node
* not found = 0
*/
int BanNode::getDebtTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<ban_fin_in.size(); i++){
			if (ban_fin_in[i].node2 == nodeT){
				return ban_fin_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Set (credit remained) to other node
* not found = 0
*/
int BanNode::getCreditTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<ban_fin_out.size(); i++){
			if (ban_fin_out[i].node2 == nodeT){
				return ban_fin_out[i].c_out_max - ban_fin_out[i].d_in_current;
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
void BanNode::setDebtFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < ban_fin_out.size(); i++){
				if (ban_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						ban_fin_out[i].d_in_current += value; break;
					case SUB:
						ban_fin_out[i].d_in_current -= value; break;
					case EQ:
						ban_fin_out[i].d_in_current = value; break;
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
void BanNode::setCreditFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < ban_fin_in.size(); i++){
				if (ban_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						ban_fin_in[i].c_in_max += value; break;
					case SUB:
						ban_fin_in[i].c_in_max -= value; break;
					case EQ:
						ban_fin_in[i].c_in_max = value; break;
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
void BanNode::setDebtTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < ban_fin_in.size(); i++){
				if (ban_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						ban_fin_in[i].d_out_current += value; break;
					case SUB:
						ban_fin_in[i].d_out_current -= value; break;
					case EQ:
						ban_fin_in[i].d_out_current = value; break;
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
void BanNode::setCreditTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < ban_fin_out.size(); i++){
				if (ban_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						ban_fin_out[i].c_out_max += value; break;
					case SUB:
						ban_fin_out[i].c_out_max -= value; break;
					case EQ:
						ban_fin_out[i].c_out_max = value; break;
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
