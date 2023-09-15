#include <iostream>
#include <vector>

using namespace std;

#ifndef LAB6_8__TREE_H_
#define LAB6_8__TREE_H_

typedef struct Node Node;

struct Node {
  int value;
  Node* left, * right;
};

Node* init_tree(int data) {
  Node* root = (Node*)malloc(sizeof(Node));
  root->left = root->right = NULL;
  root->value = data;
  return root;
}

Node* create_node(int data) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->value = data;
  node->left = node->right = NULL;
  return node;
}

void free_tree(Node* root) {
  Node* temp = root;
  if (!temp)
	return;
  free_tree(temp->left);
  free_tree(temp->right);
  if (!temp->left && !temp->right) {
	free(temp);
	return;
  }
}

void insert(Node* root, int value) {
  if (!root->left && root->value > value) {
	root->left = create_node(value);
	return;
  } else if (!root->right && root->value < value) {
  	root->right = create_node(value);
	return;
  }
  if(root->value > value){
	insert(root->left, value);
  } else if (root->value < value) {
	insert(root->right, value);
  }
}
bool find(Node* root, int value) {
  if (!root) {
	return false;
  }
  if (root->value > value) {
	return find(root->left, value);
  } else if (root->value < value) {
	return find(root->right, value);
  } else {
	return true;
  }
}

Node* search(Node* root, int value){
  if (root->value == value) {
	return root;
  }
  if (root->value > value) {
	return search(root->left, value);
  } else  {
	return search(root->right, value);
  }
}

void getChildren (Node* root, vector<int>& children){
  if(!root){
	return;
  }
  getChildren(root->left, children);
  getChildren(root->right, children);
  children.push_back(root->value);
}

vector<int> getChildren (Node* root, int value){
  Node* node = search(root, value);
  vector<int> childern;
  getChildren(node, childern);
  return childern;
}


#endif
