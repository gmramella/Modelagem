#include "Graph.h"

Edge::Edge(unsigned int targetNodeId, unsigned int weight)
{
	this->targetNodeId = targetNodeId;
	this->weight = weight;
}

unsigned int Edge::getTargetNodeId()
{
	return targetNodeId;
}

unsigned int Edge::getWeight()
{
	return weight;
}

Node::Node()
{
	this->id = UINT_MAX;
}

Node::Node(unsigned int id)
{
	this->id = id;
}

unsigned int Node::getId()
{
	return id;
}

std::vector<Edge> Node::getEdges()
{
	return edges;
}

void Node::addNeighbor(unsigned int neighborId, unsigned int weight)
{
	edges.push_back(Edge(neighborId, weight));
}

void Node::operator=(Node & other)
{
	this->id = other.id;
	for (std::vector<Edge>::iterator edge = other.edges.begin(); edge != other.edges.end(); edge++) {
		this->addNeighbor(edge->getTargetNodeId(), edge->getWeight());
	}
}

void Graph::addNode(unsigned int id)
{
	nodes.push_back(Node(id));
}

void Graph::addEdge(unsigned int sId, unsigned int tId, unsigned int weight)
{
	if (nodeIdInGraph(sId) && nodeIdInGraph(tId)) {
		Node* s = getNodeById(sId);
		s->addNeighbor(tId, weight);
	}
}

std::vector<Node> Graph::getNodes()
{
	return nodes;
}

Node * Graph::getNodeById(unsigned int id)
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].getId() == id)
			return &nodes[i];
	}
	return nullptr;
}

unsigned int Graph::getNodePosById(unsigned int id)
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].getId() == id)
			return i;
	}
	return UINT_MAX;
}

bool Graph::nodeIdInGraph(unsigned int id)
{
	for (std::vector<Node>::iterator node = nodes.begin(); node != nodes.end(); node++)
	{
		if (node->getId() == id)
			return true;
	}
	return false;
}

void Graph::print()
{
	std::cout << "Graph has " << nodes.size() << " nodes" << std::endl;
	for (std::vector<Node>::iterator node = nodes.begin(); node != nodes.end(); node++)
	{
		std::vector<Edge> edges = node->getEdges();
		std::cout << "Node " << node->getId() << " has " << edges.size() << " neighbors:" << std::endl;
		for (std::vector<Edge>::iterator edge = edges.begin(); edge != edges.end(); edge++)
		{
			std::cout << "\t node " << edge->getTargetNodeId() << " with weight " << edge->getWeight() << std::endl;
		}
	}
}

std::vector<unsigned int> Graph::dijkstra(unsigned int sId, unsigned int tId)
{
	std::vector<unsigned int> sequence;

	if (!nodeIdInGraph(sId) || !nodeIdInGraph(tId))
		return sequence;
	if (sId == tId) {
		sequence.push_back(sId);
		return sequence;
	}

	unsigned int len = nodes.size();

	Heap heap;
	for (unsigned int i = 0; i < len; i++)
	{
		HeapNode hn;
		hn.setNode(nodes[i]);
		hn.setCost(UINT_MAX);
		hn.setPred(UINT_MAX);
		heap.addHeapNode(hn);
	}
	unsigned int pos = getNodePosById(sId);
	heap.setCostAtPos(pos, 0);

	while (!heap.isEmpty())
	{
		HeapNodeMin* heapNodeMin = heap.min();
		HeapNode* x = heapNodeMin->getHeapNode();
		heapNodeMin->getHeapNode()->setVisited(true);
		std::vector<Edge> edges = x->getNode().getEdges();
		for (size_t i = 0; i < edges.size(); i++)
		{
			Edge e = edges[i];
			unsigned int posX = getNodePosById(x->getNode().getId());
			unsigned int posY = getNodePosById(e.getTargetNodeId());
			unsigned int w = e.getWeight();
			if (heap.getHeapNodes()[posY].getCost() > heap.getHeapNodes()[posX].getCost() + w) {
				heap.setCostAtPos(posY, heap.getHeapNodes()[posX].getCost() + w);
				heap.setPredAtPos(posY, x->getNode().getId());
			}
		}
	}

	HeapNode curr = heap.getHeapNodes().at(getNodePosById(tId));
	sequence.insert(sequence.begin(), curr.getNode().getId());
	while (curr.getPred() != UINT_MAX) {
		sequence.insert(sequence.begin(), curr.getPred());
		curr = heap.getHeapNodes().at(getNodePosById(curr.getPred()));
	}
	return sequence;
}

HeapNode::HeapNode()
{
	cost = UINT_MAX;
}

HeapNode::HeapNode(HeapNode * other)
{
	this->node = other->node;
	this->cost = other->cost;
	this->pred = other->pred;
}

Node HeapNode::getNode()
{
	return node;
}

void HeapNode::setNode(Node node)
{
	this->node = node;
}

unsigned int HeapNode::getCost()
{
	return cost;
}

void HeapNode::setCost(unsigned int cost)
{
	this->cost = cost;
}

unsigned int HeapNode::getPred()
{
	return pred;
}

void HeapNode::setPred(unsigned int pred)
{
	this->pred = pred;
}

bool HeapNode::getVisited()
{
	return visited;
}

void HeapNode::setVisited(bool visited)
{
	this->visited = visited;
}

HeapNode * HeapNodeMin::getHeapNode()
{
	return heapNode;
}

void HeapNodeMin::setHeapNode(HeapNode * heapNode)
{
	this->heapNode = heapNode;
}

unsigned int HeapNodeMin::getPos()
{
	return pos;
}

void HeapNodeMin::setPos(unsigned int pos)
{
	this->pos = pos;
}

std::vector<HeapNode> Heap::getHeapNodes()
{
	return heapNodes;
}

void Heap::addHeapNode(HeapNode hn)
{
	heapNodes.push_back(hn);
}

void Heap::setCostAtPos(unsigned int pos, unsigned int cost)
{
	heapNodes.at(pos).setCost(cost);
}

void Heap::setPredAtPos(unsigned int pos, unsigned int pred)
{
	heapNodes.at(pos).setPred(pred);
}

bool Heap::isEmpty()
{
	for (size_t i = 0; i < heapNodes.size(); i++)
	{
		if (heapNodes[i].getVisited() == false)
			return false;
	}
	return true;
}

HeapNodeMin * Heap::min()
{
	HeapNodeMin* out = new HeapNodeMin();
	if (heapNodes.size() == 0) {
		out->setHeapNode(nullptr);
		out->setPos(UINT_MAX);
		return out;
	}
	else if (heapNodes.size() == 1) {
		if (!heapNodes[0].getVisited()) {
			out->setHeapNode(&heapNodes[0]);
			out->setPos(0);
		}
		else {
			out->setHeapNode(nullptr);
			out->setPos(UINT_MAX);
		}
		return out;
	}
	else {
		unsigned int pos = UINT_MAX;
		unsigned int min = UINT_MAX;
		for (unsigned int i = 0; i < heapNodes.size(); i++) {
			if (!heapNodes[i].getVisited()) {
				if (min > heapNodes[i].getCost()) {
					pos = i;
					min = heapNodes[i].getCost();
				}
			}
		}
		out->setHeapNode(&heapNodes[pos]);
		out->setPos(pos);
		return out;
	}
}

void Heap::print()
{
	for (std::vector<HeapNode>::iterator node = heapNodes.begin(); node != heapNodes.end(); node++)
	{
		std::cout << "Node " << node->getNode().getId() << " costs " << node->getCost();
		if (node->getPred() != UINT_MAX)
			std::cout << " from " << node->getPred() << std::endl;
		else
			std::cout << std::endl;
	}
}
