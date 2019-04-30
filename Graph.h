#include <iostream>
#include <climits>
#include <vector>
#pragma once

class Edge
{
public:
	Edge(unsigned int targetNodeId, unsigned int weight);
	unsigned int getTargetNodeId();
	unsigned int getWeight();
private:
	unsigned int targetNodeId;
	unsigned int weight;
};

class Node
{
public:
	Node();
	Node(unsigned int id);
	unsigned int getId();
	std::vector<Edge> getEdges();
	void addNeighbor(unsigned int neighborId, unsigned int weight);
	void operator=(Node& other);
private:
	unsigned int id;
	std::vector<Edge> edges;
};

class Graph
{
public:
	void addNode(unsigned int id);
	void addEdge(unsigned int sId, unsigned int tId, unsigned int weight);
	std::vector<Node> getNodes();
	Node* getNodeById(unsigned int id);
	unsigned int getNodePosById(unsigned int id);
	bool nodeIdInGraph(unsigned int id);
	void print();
	std::vector<unsigned int> dijkstra(unsigned int sId, unsigned int tId);
private:
	std::vector<Node> nodes;
};

class HeapNode
{
public:
	HeapNode();
	HeapNode(HeapNode* other);
	Node getNode();
	void setNode(Node node);
	unsigned int getCost();
	void setCost(unsigned int cost);
	unsigned int getPred();
	void setPred(unsigned int pred);
	bool getVisited();
	void setVisited(bool visited);
private:
	Node node;
	unsigned int cost;
	unsigned int pred;
	bool visited = false;
};

class HeapNodeMin
{
public:
	HeapNode* getHeapNode();
	void setHeapNode(HeapNode* heapNode);
	unsigned int getPos();
	void setPos(unsigned int pos);
private:
	HeapNode* heapNode;
	unsigned int pos;
};

class Heap
{
public:
	std::vector<HeapNode> getHeapNodes();
	void addHeapNode(HeapNode hn);
	void setCostAtPos(unsigned int pos, unsigned int cost);
	void setPredAtPos(unsigned int pos, unsigned int pred);
	bool isEmpty();
	HeapNodeMin* min();
	void print();
private:
	std::vector<HeapNode> heapNodes;
};
