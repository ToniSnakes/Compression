#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

ofstream fout ("compressed.txt");

bitset <8> working;
bitset <11> current;
int k, loc, active;
long long frequency[256];
char buff[50];

struct Car {
	long long freq;
	char c;
};
Car cars[256];

bool cmp (Car a, Car b)
{
	return a.freq > b.freq;	
}

struct Node {
	int type;
	char c;
	Node *left = 0, *right = 0;	
};

queue <Node *> q;

void build (Node *root)
{
	/*if (loc < k) {
		Node *nn = new Node;
		nn->type = 1;
		++loc;
		node->left = nn;
	}
	if (loc < k && node.type) {
		Node *nn = new Node;
		nn->type = 0;
		++loc;
		node->right = nn;
	}
	if (*/
	//cout << "type: " << root->type << endl;
	q.push(root);
	root->c = cars[0].c;
	cout << "type: " << q.front()->type << endl;
	loc = 1;
	while (!q.empty()) {
		Node *node = q.front();
		q.pop();
		if (loc >= k) {
			continue;	
		}
		Node *nn = new Node;
		nn->type = 1;
		nn->c = cars[loc].c;
		++loc;
		node->left = nn;
		q.push(nn);
		if (loc < k && node->type) {
			Node *nn = new Node;
			nn->type = 0;
			nn->c = cars[loc].c;
			++loc;
			node->right = nn;
			q.push(nn);
		}
	}
}

int findType (Node *root, char c)
{
	q.push(root);
	while (!q.empty()) {
		Node *node = q.front();
		q.pop();
		if (node->c == c) {
			return node->type;
		}
		if (node->left) {
			q.push(node->left);	
		}
		if (node->right) {
			q.push(node->right);	
		}
	}
	return -1;
}

void process ()
{
	char output = 0;
	for (int i = 0; i < 8; ++i) {
		//cout << working[i];
		output = ((output << 1) | working[i]);	
	}
	fout << output;
	//cout << endl;
}

void processFinal ()
{
	char output = 0;
	for (int i = 0; i < active; ++i) {
		//cout << working[i];
		output = ((output << 1) | working[i]);	
	}
	for (int i = active; i < 8; ++i) {
		//cout << 1;
		output = ((output << 1) | 1);
	}
	fout << output;
	//cout << endl;
}

void refresh ()
{
	for (int i = 0; i < 8; ++i) {
		working[i] = 0;	
	}
	active = 0;	
}

void add (int poz)
{
	/*for (int i = 0; i <= poz; ++i) {
		cout << current[i];	
	}*/
	for (int i = 0; i <= poz; ++i) {
		working[active] = current[i];
		++active;
		if (active > 7) {
			process();
			refresh();
		}
	}
	for (int i = 0; i <= 1; ++i) {
		working[active] = 0;
		++active;
		if (active > 7) {
			process();
			refresh();
		}
	}
}

int find (Node *node, char c, int poz)
{
	current[poz] = node->type;
	if (node->c == c) {
		add(poz);
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

int main (int argv, char **argc)
{
	if (argv != 2) {
		cout << "Specify exactly one file!";
		return 0;
	}
	else {
		//cout << argc[0] << " " << argc[1];	
		ifstream fin (argc[1]);
		cout << argc[1] << endl << endl;
		while (!fin.eof()) {
			fin.read(buff, 50);
			for (int i = 0; i < fin.gcount(); ++i) {
				cout << buff[i];
				++frequency[buff[i]];
			}
		}
		fin.close();
		fin.clear();
		fin.open(argc[1]);
		k = 0;
		for (int i = 0; i < 256; ++i) {
			if (frequency[i]) {
				cars[k].freq = frequency[i];
				cars[k].c = i;
				++k;
			}
		}
		sort(cars, cars + k, cmp);
		fout << k;
		Node *root = new Node;
		root->type = 1;
		loc = 1;
		build(root);
		for (int i = 0; i < k; ++i) {
			cout << "'" << cars[i].c << "' " << cars[i].freq << " " << findType(root, cars[i].c) << endl;
			fout << cars[i].c;
		}
		for (int i = 0; i < k; ++i) {
			cout << findType(root, cars[i].c);	
		}
		cout << endl;
		/*for (int i = 0; i < k; ++i) {
			find(root, cars[i].c, 0);
			//cout << endl;
		}*/
		cout << endl;
		while (!fin.eof()) {
			fin.read(buff, 50);
			for (int i = 0; i < fin.gcount(); ++i) {
				//cout << "'" <<  buff[i] << "' " << frequency[buff[i]] << endl;	
				//cout << "'" <<  buff[i] << "' " << findType(root, buff[i]) << endl;	
				//add(buff[i]);
				find(root, buff[i], 0);
			}
		}
		if (active) {
			processFinal();
			refresh();
		}
	}
	return 0;	
}
