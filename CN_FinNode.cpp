// CN_FinNode.C 

#include"Error.h"
#include"CN_Node.h"
#include<queue>
#include<vector>
#include<iostream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Financial Nodes */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FinNode::FinNode(){
	setNodeID(-1);
	setNodeType();
}
void FinNode::setFinNode(int id){
	nodeID = id;
	setNodeType();
}

/////////////////* Useful tools for debugging *////////////////////////////////////////////////////
void FinNode::print(){
	cout << "FinNode " << nodeID << " fin_fin_in: " << endl;
	for (int i = 0; i<fin_fin_in.size(); i++){
		cout << fin_fin_in[i].node2->getNodeID() << " " << fin_fin_in[i].d_out_current << " / "
			<< fin_fin_in[i].c_in_max << ", ";
	}
	cout << endl;
	cout << "FinNode " << nodeID << " fin_fin_out: " << endl;
	for (int i = 0; i<fin_fin_out.size(); i++){
		cout << fin_fin_out[i].node2->getNodeID() << " " << fin_fin_out[i].d_in_current << " / "
			<< fin_fin_out[i].c_out_max << ", ";
	}
	cout << endl;
}

/* Find (current debt) from other node
* not found = 0
*/
int FinNode::getDebtFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<fin_fin_out.size(); i++){
			if (fin_fin_out[i].node2 == nodeT){
				return fin_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<fin_con_out.size(); i++){
			if (fin_con_out[i].node2 == nodeT){
				return fin_con_out[i].d_in_current;
			}
		}
	}
	else if (typeT == PRODUCER){
		for (int i = 0; i<fin_pro_out.size(); i++){
			if (fin_pro_out[i].node2 == nodeT){
				return fin_pro_out[i].d_in_current;
			}
		}
	}
	else if (typeT == BANK){
		for (int i = 0; i<fin_ban_out.size(); i++){
			if (fin_ban_out[i].node2 == nodeT){
				return fin_ban_out[i].d_in_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<fin_lab_out.size(); i++){
			if (fin_lab_out[i].node2 == nodeT){
				return fin_lab_out[i].d_in_current;
			}
		}
	}
	return 0;
}

/* Find (credit remaining) from other node
* not found = 0
*/
int FinNode::getCreditFrom(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<fin_fin_in.size(); i++){
			if (fin_fin_in[i].node2 == nodeT){
				return fin_fin_in[i].c_in_max - fin_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<fin_con_in.size(); i++){
			if (fin_con_in[i].node2 == nodeT){
				return fin_con_in[i].c_in_max - fin_con_in[i].d_out_current;
			}
		}
	}
	else if (typeT == PRODUCER){
		for (int i = 0; i<fin_pro_in.size(); i++){
			if (fin_pro_in[i].node2 == nodeT){
				return fin_pro_in[i].c_in_max - fin_pro_in[i].d_out_current;
			}
		}
	}
	else if (typeT == BANK){
		for (int i = 0; i<fin_ban_in.size(); i++){
			if (fin_ban_in[i].node2 == nodeT){
				return fin_ban_in[i].c_in_max - fin_ban_in[i].d_out_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<fin_lab_in.size(); i++){
			if (fin_lab_in[i].node2 == nodeT){
				return fin_lab_in[i].c_in_max - fin_lab_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Find (current debt) to other node
* not found = 0
*/
int FinNode::getDebtTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<fin_fin_in.size(); i++){
			if (fin_fin_in[i].node2 == nodeT){
				return fin_fin_in[i].d_out_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<fin_con_in.size(); i++){
			if (fin_con_in[i].node2 == nodeT){
				return fin_con_in[i].d_out_current;
			}
		}
	}
	else if (typeT == PRODUCER){
		for (int i = 0; i<fin_pro_in.size(); i++){
			if (fin_pro_in[i].node2 == nodeT){
				return fin_pro_in[i].d_out_current;
			}
		}
	}
	else if (typeT == BANK){
		for (int i = 0; i<fin_ban_in.size(); i++){
			if (fin_ban_in[i].node2 == nodeT){
				return fin_ban_in[i].d_out_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<fin_lab_in.size(); i++){
			if (fin_lab_in[i].node2 == nodeT){
				return fin_lab_in[i].d_out_current;
			}
		}
	}
	return 0;
}

/* Set (credit remained) to other node
* not found = 0
*/
int FinNode::getCreditTo(Node* nodeT){
	NodeType typeT = nodeT->nodeType;
	if (typeT == FINANCIAL){
		for (int i = 0; i<fin_fin_out.size(); i++){
			if (fin_fin_out[i].node2 == nodeT){
				return fin_fin_out[i].c_out_max - fin_fin_out[i].d_in_current;
			}
		}
	}
	else if (typeT == CONSUMER){
		for (int i = 0; i<fin_con_out.size(); i++){
			if (fin_con_out[i].node2 == nodeT){
				return fin_con_out[i].c_out_max - fin_con_out[i].d_in_current;
			}
		}
	}
	else if (typeT == PRODUCER){
		for (int i = 0; i<fin_pro_out.size(); i++){
			if (fin_pro_out[i].node2 == nodeT){
				return fin_pro_out[i].c_out_max - fin_pro_out[i].d_in_current;
			}
		}
	}
	else if (typeT == BANK){
		for (int i = 0; i<fin_ban_out.size(); i++){
			if (fin_ban_out[i].node2 == nodeT){
				return fin_ban_out[i].c_out_max - fin_ban_out[i].d_in_current;
			}
		}
	}
	else if (typeT == LABOR){
		for (int i = 0; i<fin_lab_out.size(); i++){
			if (fin_lab_out[i].node2 == nodeT){
				return fin_lab_out[i].c_out_max - fin_lab_out[i].d_in_current;
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
void FinNode::setDebtFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < fin_fin_out.size(); i++){
				if (fin_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_fin_out[i].d_in_current += value; break;
					case SUB:
						fin_fin_out[i].d_in_current -= value; break;
					case EQ:
						fin_fin_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < fin_con_out.size(); i++){
				if (fin_con_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_con_out[i].d_in_current += value; break;
					case SUB:
						fin_con_out[i].d_in_current -= value; break;
					case EQ:
						fin_con_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == PRODUCER){
			for (int i = 0; i < fin_pro_out.size(); i++){
				if (fin_pro_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_pro_out[i].d_in_current += value; break;
					case SUB:
						fin_pro_out[i].d_in_current -= value; break;
					case EQ:
						fin_pro_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == BANK){
			for (int i = 0; i < fin_ban_out.size(); i++){
				if (fin_ban_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_ban_out[i].d_in_current += value; break;
					case SUB:
						fin_ban_out[i].d_in_current -= value; break;
					case EQ:
						fin_ban_out[i].d_in_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < fin_lab_out.size(); i++){
				if (fin_lab_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_lab_out[i].d_in_current += value; break;
					case SUB:
						fin_lab_out[i].d_in_current -= value; break;
					case EQ:
						fin_lab_out[i].d_in_current = value; break;
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
void FinNode::setCreditFrom(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < fin_fin_in.size(); i++){
				if (fin_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_fin_in[i].c_in_max += value; break;
					case SUB:
						fin_fin_in[i].c_in_max -= value; break;
					case EQ:
						fin_fin_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < fin_con_in.size(); i++){
				if (fin_con_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_con_in[i].c_in_max += value; break;
					case SUB:
						fin_con_in[i].c_in_max -= value; break;
					case EQ:
						fin_con_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == PRODUCER){
			for (int i = 0; i < fin_pro_in.size(); i++){
				if (fin_pro_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_pro_in[i].c_in_max += value; break;
					case SUB:
						fin_pro_in[i].c_in_max -= value; break;
					case EQ:
						fin_pro_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == BANK){
			for (int i = 0; i < fin_ban_in.size(); i++){
				if (fin_ban_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_ban_in[i].c_in_max += value; break;
					case SUB:
						fin_ban_in[i].c_in_max -= value; break;
					case EQ:
						fin_ban_in[i].c_in_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < fin_lab_in.size(); i++){
				if (fin_lab_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_lab_in[i].c_in_max += value; break;
					case SUB:
						fin_lab_in[i].c_in_max -= value; break;
					case EQ:
						fin_lab_in[i].c_in_max = value; break;
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
void FinNode::setDebtTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < fin_fin_in.size(); i++){
				if (fin_fin_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_fin_in[i].d_out_current += value; break;
					case SUB:
						fin_fin_in[i].d_out_current -= value; break;
					case EQ:
						fin_fin_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < fin_con_in.size(); i++){
				if (fin_con_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_con_in[i].d_out_current += value; break;
					case SUB:
						fin_con_in[i].d_out_current -= value; break;
					case EQ:
						fin_con_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == PRODUCER){
			for (int i = 0; i < fin_pro_in.size(); i++){
				if (fin_pro_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_pro_in[i].d_out_current += value; break;
					case SUB:
						fin_pro_in[i].d_out_current -= value; break;
					case EQ:
						fin_pro_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == BANK){
			for (int i = 0; i < fin_ban_in.size(); i++){
				if (fin_ban_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_ban_in[i].d_out_current += value; break;
					case SUB:
						fin_ban_in[i].d_out_current -= value; break;
					case EQ:
						fin_ban_in[i].d_out_current = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < fin_lab_in.size(); i++){
				if (fin_lab_in[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_lab_in[i].d_out_current += value; break;
					case SUB:
						fin_lab_in[i].d_out_current -= value; break;
					case EQ:
						fin_lab_in[i].d_out_current = value; break;
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
void FinNode::setCreditTo(Node* nodeT, int value, ChangeNode mode){
	try{
		NodeType typeT = nodeT->nodeType;
		if (typeT == FINANCIAL){
			for (int i = 0; i < fin_fin_out.size(); i++){
				if (fin_fin_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_fin_out[i].c_out_max += value; break;
					case SUB:
						fin_fin_out[i].c_out_max -= value; break;
					case EQ:
						fin_fin_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == CONSUMER){
			for (int i = 0; i < fin_con_out.size(); i++){
				if (fin_con_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_con_out[i].c_out_max += value; break;
					case SUB:
						fin_con_out[i].c_out_max -= value; break;
					case EQ:
						fin_con_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == PRODUCER){
			for (int i = 0; i < fin_pro_out.size(); i++){
				if (fin_pro_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_pro_out[i].c_out_max += value; break;
					case SUB:
						fin_pro_out[i].c_out_max -= value; break;
					case EQ:
						fin_pro_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == BANK){
			for (int i = 0; i < fin_ban_out.size(); i++){
				if (fin_ban_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_ban_out[i].c_out_max += value; break;
					case SUB:
						fin_ban_out[i].c_out_max -= value; break;
					case EQ:
						fin_ban_out[i].c_out_max = value; break;
					default:
						throw OP_NOT_DEFINE;
					}
				}
			}
		}
		else if (typeT == LABOR){
			for (int i = 0; i < fin_lab_out.size(); i++){
				if (fin_lab_out[i].node2 == nodeT){
					switch (mode){
					case ADD:
						fin_lab_out[i].c_out_max += value; break;
					case SUB:
						fin_lab_out[i].c_out_max -= value; break;
					case EQ:
						fin_lab_out[i].c_out_max = value; break;
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
