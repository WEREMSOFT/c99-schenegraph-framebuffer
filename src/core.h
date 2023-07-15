#ifndef __CORE_H__
#define __CORE_H__
#include <stdio.h>
#include <stdlib.h>
struct Node;

typedef struct
{
	struct Node* next;
	struct Node* prev;
	struct Node* children;
	void* data;
} Node;

Node* nodeCreate()
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->next = NULL;
	node->children = NULL;
	return node;
}

void nodeAddSibling(Node* _this, Node* sibling)
{
	if(_this->next == NULL)
	{
		_this->next = sibling;
		sibling->prev = _this;
		return;
	}

	Node* lastSibling = _this->next;
	while(lastSibling->next != NULL)
	{
		lastSibling = lastSibling->next;
	}

	lastSibling->next = sibling;
	sibling->prev = lastSibling;
}

void nodeAddChild(Node* _this, Node* child)
{
	if(_this->children == NULL)
	{
		_this->children = child;
		return;
	}

	nodeAddSibling(_this->children, child);
}

typedef void (*TraverseNodeCallback)(Node*, void*);

void traverseGraph(Node* root, void* context, TraverseNodeCallback funPtr)
{
	if(root == NULL) return;
	Node* lastChildren = root->children;
	while(lastChildren != NULL)
	{
		Node* next = lastChildren->next;
		traverseGraph(lastChildren, context, funPtr);
		lastChildren = next;
	}
	funPtr(root, context);
}

void freeNode(Node* node)
{
	free(node->data);
}

SDL_FRect rectToRectf(SDL_Rect rectI)
{
	SDL_FRect rect = {0};
	rect.x = (float)rectI.x;
	rect.y = (float)rectI.y;
	rect.w = (float)rectI.w;
	rect.h = (float)rectI.h;

	return rect;
}

SDL_Rect rectfToRect(SDL_FRect rectF)
{
	SDL_Rect rect = {0};
	rect.x = (int)rectF.x;
	rect.y = (int)rectF.y;
	rect.w = (int)rectF.w;
	rect.h = (int)rectF.h;

	return rect;
}
#endif
