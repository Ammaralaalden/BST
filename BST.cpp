// BST.cpp
#include "BST.h"
#include <iostream>
#include <stack>
#include <algorithm> // для std::max


// Node

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right): keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other): keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) 
{
	if (other.left) 
	{
		left = new Node(*other.left);
		left->parent = this;
	}
	if (other.right) 
	{
		right = new Node(*other.right);
		right->parent = this;
	}
}

bool BinarySearchTree::Node::operator==(const Node &other) const 
{
	return (keyValuePair == other.keyValuePair) && (left == other.left) && (right == other.right);
}

void BinarySearchTree::Node::output_node_tree() const 
{
	std::stack<const Node*> stack;
	const Node* current = this;

	while (current || !stack.empty()) 
	{
		while (current) 
		{
			stack.push(current);
			current = current->left;
		}
		current = stack.top();
		stack.pop();
		std::cout << "[" << current->keyValuePair.first << ": " << current->keyValuePair.second << "] ";
		current = current->right;
	}
}



void BinarySearchTree::Node::insert(const Key &key, const Value &value) 
{
	if (key < keyValuePair.first) 
	{
		if (left) 
		{
			left->insert(key, value);
		} else 
		{
			left = new Node(key, value, this);
			left->root = root;
		}
	} else
	{
		if (right) 
		{
			right->insert(key, value);
		} else 
		{
			right = new Node(key, value, this);
			right->root = root;
		}
	}
}





void BinarySearchTree::Node::erase(const Key &key)
{
	if (key == keyValuePair.first)
	{
		//правый наследник
		if (!left && right)
		{
			if (parent)
			{
				if (parent->left == this)
				{
					parent->left = right;
				}
				else
				{
					parent->right = right;
				}
			}
			else
			{
				*root = right;
				(*root)->endd = endd;
			}
			right->parent = parent;
			delete this;
		}//левый наследник
		else if (left && !right)
		{
			if (parent)
			{
				if (parent->left == this)
				{
					parent->left = left;
				}
				else
				{
					parent->right = left;
				}
			}
			else
			{
				*root = left;
				(*root)->endd = endd;
			}
			left->parent = parent;
			delete this;
		}// нет насследников
		else if (!left && !right)
		{
			if (parent)
			{
				if (parent->left == this)
				{
					parent->left = nullptr;
				}
				else
				{
					parent->right = nullptr;
				}
			}
			else
			{
				*root = nullptr;
			}
			delete this;
		}
		else //    2 наследника
		{
			Node* node = right;
			while (node->left)
			{
				node = node->left;
			}
			if (node->parent->left == node)
			{
				node->parent->left = node->right; 
			}
			else
			{
				node->parent->right = node->right;
			}
			if (node->right)
			{
				node->right->parent = node->parent;
			}
			if (parent)
			{
				if (parent->left == this)
				{
					parent->left = node;
				}
				else
				{
					parent->right = node;
				}
				node->parent = parent;
			}
			else
			{
				*root = node;
				(*root)->endd = endd;
				node->parent = nullptr;
			}
			node->left = left;
			if (node->left)
			{
				node->left->parent = node;
			}
			node->right = right;
			if (node->right)
			{
				node->right->parent = node;
			}
			delete this;
		}
    	}
	else if (key >= keyValuePair.first)
	{
		if (right)
		{
			right->erase(key);
		}
	}
	else
	{
		if (left)
		{
			left->erase(key);
		}
	}
}



//----------------------------------------------------------------------

// BinarySearchTree

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) 
{
	if (other._root) 
	{
		_root = new Node(*other._root);
		_size = other._size;
	}
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) 
{
	if (this != &other) 
	{
		clear();
		_root = new Node(*other._root);
		_size = other._size;
	}
	return *this;
}




void BinarySearchTree::nmove(Node* node, Node** roott) // Обновляет _root для всех узлов дерева
{
	if (node == nullptr)
	{
		return;
	}
	node->root = roott;
	nmove(node->left, roott);
	nmove(node->right, roott);
}



// Конструктор перем.
BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept : _root(other._root), _size(other._size) 
{
	nmove(_root, &_root);
	other._root = nullptr;	
	other._size = 0;
}

// Присваивание перем.
BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept 
{
	if (this != &other) 
	{
		clear();
		_root = other._root;
		_size = other._size;
		nmove(_root, &_root);
		other._root = nullptr;
		other._size = 0;
	}
	return *this;
}

BinarySearchTree::~BinarySearchTree() 
{
	clear();
}

// Очистка дерева
void BinarySearchTree::clear() 
{
	std::stack<Node*> stack;
	Node *current = _root;

	while (current || !stack.empty()) 
	{
		while (current) 
		{
			stack.push(current);
			current = current->left;
		}
		current = stack.top();
		stack.pop();
		Node *right = current->right;
		delete current;
		current = right;
	}
	_root = nullptr;
	_size = 0;
}


void BinarySearchTree::insert(const Key &key, const Value &value) 
{
	if (!_root) 
	{
		_root = new Node(key, value);
		_size++;
		_root->root = &_root;
		_endd.root = &_root;
		_root->endd = &_endd;
	} else 
	{
		_root->insert(key, value);
		_size++;
	}
}



void BinarySearchTree::erase(const Key &key)
{
	if (!_root)
	{
		return;
	}
	while ((_size > 0) && (find(key) != end()))
	{
		_root->erase(key);
		_size--;
	}
}



BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const 
{
	Node *current = _root;
	while (current) 
	{
		if (key < current->keyValuePair.first) 
		{
			current = current->left;

		} else if (key > current->keyValuePair.first) 
		{
			current = current->right;
		} else 
		{
			return ConstIterator(current);
		}

		if (current == nullptr)
		{
			return cend();
		}
	}
	return cend();
}


BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) 
{
	Node *current = _root;
	while (current) 
	{
		if (key < current->keyValuePair.first) 
		{
			current = current->left;
		} else if (key > current->keyValuePair.first) 
		{
			current = current->right;
		} else 
		{
			return Iterator(current);
		}
		if (current == nullptr)
		{
			return end();
		}
	}
	return end();
}






std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> 
BinarySearchTree::equalRange(const Key &key) 
{
	auto lower = find(key);
	auto upper = lower;
	while (upper != end() && upper->first == key) 
	{
		++upper;
	}
	return std::make_pair(lower, upper);
}



std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> 
BinarySearchTree::equalRange(const Key &key) const 
{
	auto lower = find(key);
	auto upper = lower;
	while (upper != cend() && upper->first == key) 
	{
		++upper;
	}
	return std::make_pair(lower, upper);
}




BinarySearchTree::ConstIterator BinarySearchTree::min() const 
{
	if (!_root) return cend();
	Node *current = _root;
	while (current->left) 
	{
		current = current->left;
	}
	return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const 
{
	if (!_root) return cend();
	Node *current = _root;
	while (current->right) 
	{
		current = current->right;
	}
	return ConstIterator(current);
}

// Минимальный элемент с заданным ключом C НАИМЕНЬШИМ ЗНАЧЕНИЕМ!!!

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const 
{
	auto range = equalRange(key);
	if (range.first == range.second) return cend();
	ConstIterator min_it = range.first;
	for (auto it = range.first; it != range.second; ++it) 
	{
		if (it->second < min_it->second) 
		{
			min_it = it;
		}
	}
	return min_it;
}


BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const 
{
	auto range = equalRange(key);
	if (range.first == range.second) return cend();
	ConstIterator max_it = range.first;
	for (auto it = range.first; it != range.second; ++it) 
	{
		if (it->second > max_it->second) 
		{
			max_it = it;
		}
	}
	return max_it;
}



BinarySearchTree::Iterator BinarySearchTree::begin() 
{
	if (!_root) return end();
	Node *current = _root;
	while (current->left) 
	{
		current = current->left;
	}
	return Iterator(current);
}


BinarySearchTree::Iterator BinarySearchTree::end() 
{
	if (!_root) return Iterator(nullptr);
	Node *current = _root;
	while (current->right)
	{
		current = current->right;
	}
	return (++Iterator(current));
}



BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const 
{
	return min();
}



BinarySearchTree::ConstIterator BinarySearchTree::cend() const 
{
	return (++max());
}



size_t BinarySearchTree::size() const 
{
	return _size;
}

void BinarySearchTree::output_tree() 
{
	if (_root)
	{
		_root->output_node_tree();
		std::cout << std::endl;
	} else
	{
		std::cout << "Пустое дерево" << "\n";
	}
}


size_t BinarySearchTree::max_height() const 
{
	struct Helper 
	{
		size_t height(Node *node) 
		{ 
			if (!node) return 0;
			return 1 + std::max(height(node->left), height(node->right));
		}
	};
	Helper helper;
	return helper.height(_root);
}


//----------------------------------------------------------------------

// Iterator
BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}


std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() 
{
	return _node->keyValuePair;
}


const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const 
{
	return _node->keyValuePair;
}


std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() 
{
	return &_node->keyValuePair;
}


const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const 
{
	return &_node->keyValuePair;
}


// Префиксный инкремент
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() 
{
	if (!_node) 
	{
		return *this;
	}

	if (_node->right) 
	{
		_node = _node->right;
		while (_node->left) 
		{
			_node = _node->left;
		}
	} else 
	{
		Node *parent = _node->parent;
		while (parent && _node == parent->right) 
		{
			_node = parent;
			parent = parent->parent;
		}
		if (parent == nullptr)
		{
			_node = (*(_node->root))->endd;
			_node->isend = true;
			return *this;
		}
		_node = parent;
	}
	return *this;
}



// Постфиксный инкремент
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) 
{
	if (!_node)
	{
		return *this;
	}
	return ++(*this);
}


// Префиксный декремент
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() 
{
	if (!_node)
	{
		return *this;
	}

	if (_node->isend)
	{
		_node->isend = false;
		_node = *_node->root;
		while (_node->right)
		{
			_node = _node->right;
		}
		return *this;
	}	

	if (_node->left) 
	{
		_node = _node->left;
		while (_node->right) 
		{
			_node = _node->right;
		}
	} else 
	{
		Node *parent = _node->parent;
		while (parent && _node == parent->left) 
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}


// Постфиксный декремент
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) 
{
	if (!_node)
	{
		return *this;
	}
	return --(*this);
}



bool BinarySearchTree::Iterator::operator==(const Iterator &other) const 
{
	return _node == other._node;
}



bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const 
{
	return !(_node == other._node);
}



//-------------------------------------------------------------------

// ConstIterator

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}


const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const 
{
	return _node->keyValuePair;
}


const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const 
{
	return &_node->keyValuePair;
}


BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() 
{
	
	if (!_node) 
	{
		return *this;
	}

	if (_node->right) 
	{
		_node = _node->right;
		while (_node->left) 
		{
			_node = _node->left;
		}
	} else 
	{
		Node *parent = _node->parent;
		while (parent && _node == parent->right) 
		{
			_node = parent;
			parent = parent->parent;
		}
		if (parent == nullptr)
		{
			_node = (*(_node->root))->endd;
			_node->isend = true;
			return *this;
		}
		_node = parent;
	}
	return *this;


}


BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) 
{

	if (!_node)
	{
		return *this;
	}
	return ++(*this);
}


BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() 
{

	if (!_node)
	{
		return *this;
	}

	if (_node->isend)
	{
		_node->isend = false;
		_node = *_node->root;
		while (_node->right)
		{
			_node = _node->right;
		}
		return *this;
	}	

	if (_node->left) 
	{
		_node = _node->left;
		while (_node->right) 
		{
			_node = _node->right;
		}
	} else 
	{
		Node *parent = _node->parent;
		while (parent && _node == parent->left) 
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;

}


BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) 
{
	if (!_node)
	{
		return *this;
	}
	return (--(*this));
}


bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const 
{
	return _node == other._node;
}


bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const 
{
	return !(_node == other._node);
}


