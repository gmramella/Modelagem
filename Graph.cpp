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

void Node::setId(unsigned int id)
{
	this->id = id;
}

EdgeList Node::getEdges()
{
	return edges;
}

void Node::addNeighbor(unsigned int neighborId, unsigned int weight)
{
	Edge e(neighborId, weight);
	edges.addEdge(e);
}

void Node::operator=(Node & other)
{
	this->id = other.id;
	Edge* curr = edges.head;
	for (size_t i = 0; i < edges.size(); i++)
	{
		this->addNeighbor(curr->getTargetNodeId(), curr->getWeight());
		curr = curr->next;
	}
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

HeapNodeMin::HeapNodeMin(HeapNode * heapNode, unsigned int pos)
{
	this->heapNode = heapNode;
	this->pos = pos;
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

void Heap::setCost(unsigned int pos, unsigned int cost)
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
	if (heapNodes.size() == 0) {
		return new HeapNodeMin(nullptr, UINT_MAX);
	}
	else if (heapNodes.size() == 1) {
		if (!heapNodes[0].getVisited()) {
			return new HeapNodeMin(&heapNodes[0], 0);
		}
		else {
			return new HeapNodeMin(nullptr, UINT_MAX);
		}
	}
	else {
		HeapNode* hn = nullptr;
		unsigned int pos = UINT_MAX;
		unsigned int min = UINT_MAX;
		for (unsigned int i = 0; i < heapNodes.size(); i++) {
			if (!heapNodes[i].getVisited()) {
				if (min > heapNodes[i].getCost()) {
					hn = &heapNodes[i];
					pos = i;
					min = heapNodes[i].getCost();
				}
			}
		}
		return new HeapNodeMin(hn, pos);
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

void EdgeList::addEdge(Edge e)
{
	if (!head) {
		head = new Edge(e);
		head->next = nullptr;
	}
	else
	{
		Edge* curr = head;
		while (curr->next) {
			curr = curr->next;
		}
		curr->next = new Edge(e);
	}
	length++;
}

unsigned int EdgeList::size()
{
	return length;
}

Graph::Graph(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	nodes = new Node*[width];
	for (size_t i = 0; i < width; i++)
	{
		nodes[i] = new Node[height];
	}
}

Graph::~Graph()
{
	/*for (size_t i = 0; i < width; i++)
	{
	delete[] nodes[i];
	}
	delete[] nodes;*/
}

unsigned int Graph::size()
{
	return width * height;
}

void Graph::addNode(unsigned int id)
{
	Pos2 pos = getNodePosById(id);
	nodes[pos.x][pos.y].setId(id);
}

void Graph::addEdge(unsigned int sId, unsigned int tId, unsigned int weight)
{
	if (nodeInGraph(sId) && nodeInGraph(tId)) {
		Pos2 pos = getNodePosById(sId);
		nodes[pos.x][pos.y].addNeighbor(tId, weight);
	}
}

Node** Graph::getNodes()
{
	return nodes;
}

Node Graph::getNodeById(unsigned int id)
{
	Pos2 pos = getNodePosById(id);
	return nodes[pos.x][pos.y];
}

Pos2 Graph::getNodePosById(unsigned int id)
{
	unsigned int x = id % width;
	unsigned int y = id / width;
	return Pos2(x, y);
}

bool Graph::nodeInGraph(unsigned int id)
{
	Pos2 pos = getNodePosById(id);
	if (pos.x * height + pos.y >= size()) return false;
	return nodes[pos.x][pos.y].getId() != UINT_MAX;
}

void Graph::print()
{
	std::cout << "Graph has " << size() << " nodes" << std::endl;
	for (unsigned int j = 0; j < height; j++)
	{
		for (unsigned int i = 0; i < width; i++)
		{
			EdgeList edges = nodes[i][j].getEdges();
			std::cout << "Node " << nodes[i][j].getId() << " has " << edges.size() << " neighbors:" << std::endl;
			Edge* curr = edges.head;
			while (curr) {
				std::cout << "\t node " << curr->getTargetNodeId() << " with weight " << curr->getWeight() << std::endl;
				curr = curr->next;
			}
		}
	}
}

std::vector<unsigned int> Graph::dijkstra(unsigned int sId, unsigned int tId)
{
	std::vector<unsigned int> sequence;

	if (!nodeInGraph(sId) || !nodeInGraph(tId))
		return sequence;
	if (sId == tId) {
		sequence.push_back(sId);
		return sequence;
	}

	Heap heap;
	for (unsigned int i = 0; i < size(); i++)
	{
		HeapNode hn;
		hn.setNode(getNodeById(i));
		hn.setCost(UINT_MAX);
		hn.setPred(UINT_MAX);
		heap.addHeapNode(hn);
	}
	heap.setCost(sId, 0);

	while (!heap.isEmpty())
	{
		HeapNodeMin* heapNodeMin = heap.min();
		HeapNode* x = heapNodeMin->getHeapNode();
		if (!x)
			break;
		x->setVisited(true);
		EdgeList edges = getNodeById(x->getNode().getId()).getEdges();
		Edge* curr = edges.head;
		while (curr) {
			unsigned int posX = x->getNode().getId();
			unsigned int posY = curr->getTargetNodeId();
			unsigned int w = curr->getWeight();
			if (heap.getHeapNodes()[posY].getCost() > heap.getHeapNodes()[posX].getCost() + w) {
				heap.setCost(posY, heap.getHeapNodes()[posX].getCost() + w);
				heap.setPredAtPos(posY, x->getNode().getId());
			}
			curr = curr->next;
		}
	}
	
	HeapNode curr = heap.getHeapNodes().at(tId);
	sequence.insert(sequence.begin(), curr.getNode().getId());
	while (curr.getPred() != UINT_MAX) {
		sequence.insert(sequence.begin(), curr.getPred());
		curr = heap.getHeapNodes().at(curr.getPred());
	}
	return sequence;
}
