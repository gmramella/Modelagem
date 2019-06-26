#pragma once
#include <iostream>
#include <climits>
#include <vector>
#include "GridGenerator.h"

class Edge
{
public:
	Edge(unsigned int targetNodeId, unsigned int weight);
	unsigned int getTargetNodeId();
	unsigned int getWeight();
	Edge* next = nullptr;
private:
	unsigned int targetNodeId;
	unsigned int weight;
};

class EdgeList
{
public:
	Edge* head = nullptr;
	void addEdge(Edge e);
	unsigned int size();
private:
	unsigned int length = 0;
};

class Node
{
public:
	Node();
	Node(unsigned int id);
	unsigned int getId();
	void setId(unsigned int id);
	EdgeList getEdges();
	void addNeighbor(unsigned int neighborId, unsigned int weight);
	void operator=(Node& other);
private:
	unsigned int id;
	EdgeList edges;
};

class Graph
{
public:
	Graph(unsigned int width, unsigned int height);
	~Graph();
	unsigned int size();
	void addNode(unsigned int id);
	void addEdge(unsigned int sId, unsigned int tId, unsigned int weight);
	Node** getNodes();
	Node getNodeById(unsigned int id);
	Pos2 getNodePosById(unsigned int id);
	bool nodeInGraph(unsigned int id);
	void print();
	std::vector<unsigned int> dijkstra(unsigned int sId, unsigned int tId);
private:
	Node** nodes;
	unsigned int width;
	unsigned int height;
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
	HeapNodeMin(HeapNode* heapNode, unsigned int pos);
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
	void setCost(unsigned int pos, unsigned int cost);
	void setPredAtPos(unsigned int pos, unsigned int pred);
	bool isEmpty();
	HeapNodeMin* min();
	void print();
private:
	std::vector<HeapNode> heapNodes;
};
