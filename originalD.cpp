#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

ofstream fout ("decompressed.txt");

bitset <20> working;
bitset <8> current;
int k, loc, active;
char buff[50], cars[256];

struct Node {
	int type;
	char c;
	Node *left = 0, *right = 0;
};

queue <Node *> q;

void build (Node *root)
{
	root->type = 1;
	root->c = cars[0];
	loc = 1;
	q.push(root);
	while (!q.empty()) {
		Node *node = q.front();
		q.pop();
		if (loc >= k) {
			continue;	
		}
		Node *nn = new Node;
		nn->type = 1;
		nn->c = cars[loc];
		++loc;
		node->left = nn;
		q.push(nn);
		if (loc < k && node->type) {
			Node *nn = new Node;
			nn->type = 0;
			nn->c = cars[loc];
			++loc;
			node->right = nn;
			q.push(nn);
		}
	}
}

int find (Node *node, char c, int poz)
{
	if (node->c == c) {
		return node->type;	
	}
	int left = -1;
	if (node->left) {
		left = find(node->left, c, poz + 1);
	}
	int right = -1;
	if (node->right) {
		right = find(node->right, c, poz + 1);	
	}
	if (left != -1) {
		return left;	
	}
	return right;
}

void process (Node *root, int poz)
{
	loc = 1;
	Node *node = root;
	/*for (int i = 0; i <= poz+5; ++i) {
		cout << working[i];	
	}
	cout << endl;*/
	while (loc <= poz) {
		if (node->left && node->left->type == working[loc]) {
			node = node->left;
		}
		else 
		if (node->right) {
			node = node->right;	
		}
		++loc;
	}
	//cout << "'" << node->c << "'" << endl;
	fout << node->c;
}

void refresh (int poz)
{
	for (int i = 1; i <= active; ++i) {
		working[i] = 0;
	}
	working[0] = poz;
	active = poz - 1;
}

void add (Node *root, char c) 
{
	for (int i = 7; i >= 0; --i) {
		current[i] = (c & 1);
		c = (c >> 1);
	}
	for (int i = 0; i < 8; ++i) {
		working[active] = current[i];
		/*for (int j = 0; j <= active; ++j) {
			cout << working[j];
		}
		cout << endl;*/
		if (active >= 2 && working[active - 1] == 0 && working[active - 2] == 0) {
			if (working[active] == 0) {
				process(root, active-2);
				refresh(0);
				//active = -1;
			}
			else {
				process(root, active-3);
				refresh(1);
				//active = 0;
			}
		}
		++active;
		//cout << current[i];
	}
	//cout << endl;
}

int main (int argv, char **argc)
{
	if (argv != 2) {
		cout << "Specify exactly one file!";
		return 0;
	}
	else {
		ifstream fin (argc[1]);
		cout << argc[1] << endl << endl;
		fin >> k;
		fin.read(cars, k);
		Node *root = new Node;
		build(root);
		for (int i = 0; i < k; ++i) {
			cout << "'" << cars[i] << "' " << find(root, cars[i], 0) << endl;	
		}
		for (int i = 0; i < k; ++i) {
			cout << find(root, cars[i], 0);	
		}
		cout << endl;
		active = 0;
		while (!fin.eof()) {
			fin.read(buff, 50);
			for (int i = 0; i < fin.gcount(); ++i) {
				add(root, buff[i]);	
			}
		}
		//char eof = EOF;
		fout << '\n';
		/*if (active) {
			process(active-2);
			refresh(0);
		}*/
	}
	return 0;
}
