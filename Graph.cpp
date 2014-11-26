// Graph.C 

#include"Error.h"
#include"CN_Graph.h"
#include<random>
#include<vector>
#include<list>
#include<queue>
#include<set>
#include<iostream>
#include <algorithm>
using namespace std;

#define INTER_BANK_INTEREST 10  //  percent
#define FIN_CON_OUT_INTEREST 3  //  percent
#define PRO_FIN_OUT_INTEREST 10  //  percent
#define CREDIT_MAX_PRO_FIN 1000000
#define CREDIT_MAX_FIN_FIN 1000000
#define CREDIT_MAX_FIN_CON 2000
#define NODE_MAX_TOTAL 4000

void Graph::print(){
	for (int i = 0; i<conNum; i++){
		conAgent[i].print();
	}
	for (int i = 0; i<proNum; i++){
		proAgent[i].print();
	}
	for (int i = 0; i<finNum; i++){
		finAgent[i].print();
	}
}

Graph::Graph(int finNumT, int conNumT, int proNumT){
	finNum = finNumT;
	conNum = conNumT;
	proNum = proNumT;

	for (int i = 0; i<finNumT; i++){
		finAgent[i].setFinNode(i);
	}
	for (int i = 0; i<conNumT; i++){
		conAgent[i].setConNode(i);
	}
	for (int i = 0; i<proNumT; i++){
		proAgent[i].setProNode(i);
	}
	banAgent.setBanNode(1); 
	labAgent.setLabNode(1); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Generate Initial Network */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Graph::genErdosRenyiGraph(){
	default_random_engine generator;
	uniform_real_distribution<double> distribution1(0.0, 1.0);
	uniform_int_distribution<int> distribution2(1, CREDIT_MAX_FIN_FIN);
	uniform_int_distribution<int> distribution3(1, CREDIT_MAX_FIN_CON);
	uniform_int_distribution<int> distribution4(1, CREDIT_MAX_PRO_FIN);

	// f<=> f, c
	for (int i = 0; i<finNum; i++){
		// inter bank, f <=> f
		for (int j = i + 1; j<finNum; j++){
			double num = distribution1(generator);
			if (num > 0.5){
				// e1 = e3, e2 = e4
				// e1:	i->j, i_j_out
				// e2:	j->i, j_i_out
				// e3:	i->j, j_i_in 
				// e4:	j->i, i_j_in
				OutEdge e1, e2;
				InEdge e3, e4;

				e1.node2 = finAgent + j;
				e1.node2Type = FINANCIAL; 
				e1.interest_rate = double(INTER_BANK_INTEREST)/100.0; 
				e1.d_in_current = 0; 
				e1.c_out_max = distribution2(generator);

				e2.node2 = finAgent + i; 
				e2.node2Type = FINANCIAL; 
				e2.interest_rate = double(INTER_BANK_INTEREST) / 100.0;
				e2.d_in_current = 0; 
				e2.c_out_max = distribution2(generator);

				e3.node2 = finAgent + i; 
				e3.node2Type = FINANCIAL; 
				e3.interest_rate = double(INTER_BANK_INTEREST) / 100.0;
				e3.d_out_current = 0; 
				e3.c_in_max = e1.c_out_max; 

				e4.node2 = finAgent + j;
				e4.node2Type = FINANCIAL;
				e4.interest_rate = double(INTER_BANK_INTEREST) / 100.0;
				e4.d_out_current = 0; 
				e4.c_in_max = e2.c_out_max;

				finAgent[i].fin_fin_out.push_back(e1);
				finAgent[j].fin_fin_out.push_back(e2);
				finAgent[j].fin_fin_in.push_back(e3);
				finAgent[i].fin_fin_in.push_back(e4);
			}
		}
		// financial to consumer, f <=> c
		for (int j = 0; j<conNum; j++){
			double num = distribution1(generator);
			if (num > 0.5){
				// e1: i->j, fin_con_out = i_j_out, 
				// e2: i->j, con_fin_in = j_i_in, 
				OutEdge e1;
				InEdge e2;

				e1.node2 = conAgent + j;
				e1.node2Type = CONSUMER; 
				e1.interest_rate = double(FIN_CON_OUT_INTEREST) / 100;
				e1.d_in_current = 0; 
				e1.c_out_max = distribution3(generator);

				e2.node2 = finAgent + i; 
				e2.node2Type = FINANCIAL; 
				e2.interest_rate = e1.interest_rate;
				e2.d_out_current = 0; 
				e2.c_in_max = e1.c_out_max; 

				finAgent[i].fin_con_out.push_back(e1);
				conAgent[j].con_fin_in.push_back(e2);
			}
		}
	}

	// producer to financial, p <=> f
	for (int i = 0; i<proNum; i++){
		for (int j = i + 1; j<finNum; j++){
			double num = distribution1(generator);
			if (num > 0.5){
				// e1: i->j, pro_fin_out = i_j_out, 
				// e2: i->j, fin_pro_in = j_i_in, 
				OutEdge e1;
				InEdge e2;

				e1.node2 = finAgent + j; 
				e1.node2Type = FINANCIAL; 
				e1.interest_rate = double(PRO_FIN_OUT_INTEREST)/100.0;
				e1.d_in_current = 0; 
				e1.c_out_max = distribution3(generator);

				e2.node2 = proAgent + i; 
				e2.node2Type = PRODUCER; 
				e2.interest_rate = e1.interest_rate; 
				e2.d_out_current = 0; 
				e2.c_in_max = e1.c_out_max; 

				proAgent[i].pro_fin_out.push_back(e1);
				finAgent[j].fin_pro_in.push_back(e2);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Search for all Agents */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Graph::maxFlowMixed(Node* fnode1, Node* fnode2){
	int currentFlow = 0;
	int flow = 0;
	while (bfsMixed(fnode1, fnode2)){
		currentFlow = pathCapacityMixed();
		flow += currentFlow;
		pathFillMixed(currentFlow);
	}
	return flow;
}

void Graph::pathFillMixed(int currentFlow){
	Node* node1 = pathMixed.front();
	pathMixed.pop_front();
	Node* node2;
	int credit = 0; 

	while (!pathMixed.empty()){
		node2 = pathMixed.front();
		pathMixed.pop_front();

		credit = node1->getCreditFrom(node2);
		if (currentFlow <= credit){
			node1->setDebtTo(node2, currentFlow, ADD);
			node2->setDebtFrom(node1, currentFlow, ADD); 
		}
		else {
			node1->setDebtTo(node2, credit, ADD);
			node2->setDebtFrom(node1, credit, ADD);
			node1->setDebtFrom(node2, currentFlow - credit, SUB);
			node2->setDebtTo(node1, currentFlow - credit, SUB);
		}

		node1 = node2;
	}
}
 
int Graph::pathCapacityMixed(){
	std::list <Node*> pathTemp = pathMixed;

	Node* node1 = pathMixed.front();
	pathMixed.pop_front();
	Node* node2;

	int cap = 2 * CREDIT_MAX_FIN_FIN;

	while (!pathMixed.empty()){
		node2 = pathMixed.front();
		pathMixed.pop_front();
		cap = min(cap,
			node1->getDebtFrom(node2) + node1->getCreditFrom(node2));
		node1 = node2;
	}

	while (!pathTemp.empty()){
		pathMixed.push_back(pathTemp.front());
		pathTemp.pop_front();
	}
	return cap;
}


/*	try to find all next available hop
*	Input : front node, path recorder, tempQueue, tempVisited
*	Modify : path recorder, tempQueue, tempVisited
*	for all NodeType, search all NodeType
*/
static void bfsMixedHelper(
	Node* front, 
	queue<Node*> &tempQueue, 
	set<Node*> &tempVisited, 
	Node* prevOfFin[], 
	Node* prevOfCon[],
	Node* prevOfPro[],
	Node* prevOfBan,
	Node* prevOfLab)
{
	NodeType nodeTypeT = front->getNodeType();
	FinNode* f = NULL; 
	ConNode* c = NULL; 
	ProNode* p = NULL;
	BanNode* b = NULL;
	LabNode* l = NULL;

	try{
		switch (nodeTypeT){
		/*//////////////////////////////////////////////////////////////////////////////////////////////////
		*	front is a financial agent
		*	we need to search all available credits/ious it can utilize
		*	we need to search fin_(fin, pro, con, ban, lab)_(in, out)
		*///////////////////////////////////////////////////////////////////////////////////////////////////
		case FINANCIAL:
			f = dynamic_cast<FinNode*>(front);
			
			/* Fin <----> Fin */
			// search within fin_fin_in, credit in
			// next financial node has not been visited, there is some (remainning fin_fin_in)
			for (int i = 0; i < f->fin_fin_in.size(); i++){
				Node* next = f->fin_fin_in[i].node2;
				if (f->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within fin_fin_out, debt in
			// next financial node has not been visited, there is some (iou_current/fin_fin_out)
			for (int i = 0; i < f->fin_fin_out.size(); i++){
				Node* next = f->fin_fin_out[i].node2;
				if (f->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}

			/* Fin <----> Pro */
			// search within fin_pro_in, credit in
			// next producer node has not been visited, there is some (remainning fin_pro_in)
			for (int i = 0; i < f->fin_pro_in.size(); i++){
				Node* next = f->fin_pro_in[i].node2;
				if (f->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfPro[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within fin_pro_out, debt in
			// next producer node has not been visited, there is some (iou_current/fin_pro_out)
			for (int i = 0; i < f->fin_pro_out.size(); i++){
				Node* next = f->fin_pro_out[i].node2;
				if (f->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfPro[next->nodeID] = front;
				tempQueue.push(next);
			}

			/* Fin <----> Con */
			// search within fin_con_in, credit in
			// next consumer node has not been visited, there is some (remainning fin_con_in)
			for (int i = 0; i < f->fin_con_in.size(); i++){
				Node* next = f->fin_con_in[i].node2;
				if (f->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfCon[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within fin_con_out, debt in
			// next consumer node has not been visited, there is some (iou_current/fin_con_out)
			for (int i = 0; i < f->fin_con_out.size(); i++){
				Node* next = f->fin_con_out[i].node2;
				if (f->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfCon[next->nodeID] = front;
				tempQueue.push(next);
			}

			/* Fin <----> Ban */
			// search within fin_ban_in, credit in
			// next bank node has not been visited, there is some (remainning fin_ban_in)
			for (int i = 0; i < f->fin_ban_in.size(); i++){
				Node* next = f->fin_ban_in[i].node2;
				if (f->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfBan = front;
				tempQueue.push(next);
			}
			// search within fin_ban_out, debt in
			// next bank node has not been visited, there is some (iou_current/fin_ban_out)
			for (int i = 0; i < f->fin_ban_out.size(); i++){
				Node* next = f->fin_ban_out[i].node2;
				if (f->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfBan = front;
				tempQueue.push(next);
			}

			/* Fin <----> Lab */
			// search within fin_lab_in, credit in
			// next labor node has not been visited, there is some (remainning fin_lab_in)
			for (int i = 0; i < f->fin_lab_in.size(); i++){
				Node* next = f->fin_lab_in[i].node2;
				if (f->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfLab = front;
				tempQueue.push(next);
			}
			// search within fin_lab_out, debt in
			// next labor node has not been visited, there is some (iou_current/fin_lab_out)
			for (int i = 0; i < f->fin_lab_out.size(); i++){
				Node* next = f->fin_lab_out[i].node2;
				if (f->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfLab = front;
				tempQueue.push(next);
			}
			break;

		/*//////////////////////////////////////////////////////////////////////////////////////////////////
		*	front is a consumer agent
		*	we also need to search all available credits/ious it can utilize
		*	we need to search con_(fin, lab)_(in, out)
		*///////////////////////////////////////////////////////////////////////////////////////////////////
		case CONSUMER:
			// front node is consumer
			c = dynamic_cast<ConNode*>(front);

			/* Con <----> Fin */
			// search within con_fin_in, credit in
			// next financial node has not been visited, there is some (remainning con_fin_in)
			for (int i = 0; i < c->con_fin_in.size(); i++){
				Node* next = c->con_fin_in[i].node2;
				if (c->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within con_fin_out, debt in
			// next financial node has not been visited, there is some (iou_current/con_fin_out)
			for (int i = 0; i < c->con_fin_out.size(); i++){
				Node* next = c->con_fin_out[i].node2;
				if (c->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}

			/* Con <----> Lab */
			// search within con_lab_in, credit in
			// next labor node has not been visited, there is some (remainning con_lab_in)
			for (int i = 0; i < c->con_lab_in.size(); i++){
				Node* next = c->con_lab_in[i].node2;
				if (c->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfLab = front;
				tempQueue.push(next);
			}
			// search within con_lab_out, debt in
			// next labor node has not been visited, there is some (iou_current/con_lab_out)
			for (int i = 0; i < c->con_lab_out.size(); i++){
				Node* next = c->con_lab_out[i].node2;
				if (c->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfLab = front;
				tempQueue.push(next);
			}
			break;

		/*//////////////////////////////////////////////////////////////////////////////////////////////////
		*	front is a producer agent
		*	we also need to search all available credits/ious it can utilize
		*	we need to search pro_(fin)_(in, out)
		*///////////////////////////////////////////////////////////////////////////////////////////////////
		case PRODUCER:
			// front node is producer
			p = dynamic_cast<ProNode*>(front);
			/* Pro <----> Fin */
			// search within pro_fin_in, credit in
			// next financial node has not been visited, there is some (remainning pro_fin_in)
			for (int i = 0; i < p->pro_fin_in.size(); i++){
				Node* next = p->pro_fin_in[i].node2;
				if (p->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within con_fin_out, debt in
			// next financial node has not been visited, there is some (iou_current/con_fin_out)
			for (int i = 0; i < p->pro_fin_out.size(); i++){
				Node* next = p->pro_fin_out[i].node2;
				if (p->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			break; 

		/*//////////////////////////////////////////////////////////////////////////////////////////////////
		*	front is a bank agent
		*	we also need to search all available credits/ious it can utilize
		*	we need to search ban_(fin)_(in, out)
		*///////////////////////////////////////////////////////////////////////////////////////////////////
		case BANK:
			// front node is producer
			b = dynamic_cast<BanNode*>(front);
			/* Ban <----> Fin */
			// search within ban_fin_in, credit in
			// next financial node has not been visited, there is some (remainning ban_fin_in)
			for (int i = 0; i < b->ban_fin_in.size(); i++){
				Node* next = b->ban_fin_in[i].node2;
				if (b->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within ban_fin_out, debt in
			// next financial node has not been visited, there is some (iou_current/ban_fin_out)
			for (int i = 0; i < b->ban_fin_out.size(); i++){
				Node* next = b->ban_fin_out[i].node2;
				if (b->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			break;

		/*//////////////////////////////////////////////////////////////////////////////////////////////////
		*	front is a labor agent
		*	we also need to search all available credits/ious it can utilize
		*	we need to search lab_(fin)_(in, out)
		*///////////////////////////////////////////////////////////////////////////////////////////////////
		case LABOR:
			// front node is producer
			l = dynamic_cast<LabNode*>(front);
			
			/* Lab <----> Con */
			// search within lab_con_in, credit in
			// next financial node has not been visited, there is some (remainning lab_con_in)
			for (int i = 0; i < l->lab_con_in.size(); i++){
				Node* next = l->lab_con_in[i].node2;
				if (l->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfCon[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within lab_con_out, debt in
			// next financial node has not been visited, there is some (iou_current/lab_con_out)
			for (int i = 0; i < l->lab_con_out.size(); i++){
				Node* next = l->lab_con_out[i].node2;
				if (l->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfCon[next->nodeID] = front;
				tempQueue.push(next);
			}

			/* Lab <----> Fin */
			// search within lab_fin_in, credit in
			// next financial node has not been visited, there is some (remainning lab_fin_in)
			for (int i = 0; i < l->lab_fin_in.size(); i++){
				Node* next = l->lab_fin_in[i].node2;
				if (l->getCreditFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			// search within lab_fin_out, debt in
			// next financial node has not been visited, there is some (iou_current/lab_fin_out)
			for (int i = 0; i < l->lab_fin_out.size(); i++){
				Node* next = l->lab_fin_out[i].node2;
				if (l->getDebtFrom(next) == 0)
					continue;
				if (tempVisited.end() != tempVisited.find(next))
					continue;
				prevOfFin[next->nodeID] = front;
				tempQueue.push(next);
			}
			break;

		default:
			throw TYPE_NOT_DEFINE;
		}
	}
	catch (Status error){
		cout << "wow something is wrong :(, type not define" << endl;
	}
	return; 
}

/*	get Node *prve of Node *front
*	Input : path recorders
*	Modify : Node* prev
*/
static void bfsMixedHelperSwitch(
	Node* &prev, 
	Node* front, 
	Node* prevOfFin[],
	Node* prevOfCon[],
	Node* prevOfPro[],
	Node* prevOfBan,
	Node* prevOfLab)
{
	switch (front->getNodeType()){
	case FINANCIAL:
		prev = prevOfFin[front->getNodeID()]; break;
	case CONSUMER:
		prev = prevOfCon[front->getNodeID()]; break;
	case PRODUCER:
		prev = prevOfPro[front->getNodeID()]; break;
	case BANK:
		prev = prevOfBan; break;
	case LABOR:
		prev = prevOfLab; break;
	default:
		throw TYPE_NOT_DEFINE;
	}
	return; 
}

bool Graph::bfsMixed(Node* node1, Node* node2){
	this->pathMixed.clear();
	queue <Node*> tempQueue;
	set <Node*> tempVisited;
	Node* front;
	Node* prevOfFin[NODE_MAX];
	Node* prevOfCon[NODE_MAX];
	Node* prevOfPro[NODE_MAX];
	Node* prevOfBan(NULL);
	Node* prevOfLab(NULL);

	try{
		tempQueue.push(node1);
		for (int i = 0; i < NODE_MAX_TOTAL; i++){
			prevOfFin[i] = NULL;
			prevOfCon[i] = NULL;
			prevOfPro[i] = NULL;
		}

		while (tempQueue.size() != 0){
			front = tempQueue.front();
			tempQueue.pop();
			if (node2 == front){
				pathMixed.push_back(front);
				break;
			}
			if (tempVisited.end() != tempVisited.find(front)){
				continue;
			}
			tempVisited.insert(front);
			// try to find all next available hop
			bfsMixedHelper(front, tempQueue, tempVisited, prevOfFin, prevOfCon, prevOfPro, prevOfBan, prevOfLab);
		}

		if (front != node2){ throw NO_PATH_FOUND; }

		Node* prev; 
		bfsMixedHelperSwitch(prev, front, prevOfFin, prevOfCon, prevOfPro, prevOfBan, prevOfLab);
		while (prev){
			this->pathMixed.push_front(front);
			bfsMixedHelperSwitch(prev, front, prevOfFin, prevOfCon, prevOfPro, prevOfBan, prevOfLab);
		}
	}
	catch (Status error){
		this->pathMixed.clear();
		return 0; 
	}

	return 1;
}
