#include "BST.h"
#include <iostream>
#include <stack>
#include <algorithm> // для std::max
#include <cmath>


// Node

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right): keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other): keyValuePair(other.keyValuePair), parent(other.parent), left(other.left), right(other.right), root(other.root){}

bool BinarySearchTree::Node::operator==(const Node &other) const
{
	return (keyValuePair == other.keyValuePair);
}

void BinarySearchTree::Node::output_node_tree() const 
{
	if (left)
	{
		left->output_node_tree();
	}
	std::cout << "[" << keyValuePair.first << ": " << keyValuePair.second << "]\n";	
	if (right) 
	{
		right->output_node_tree();
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


size_t BinarySearchTree::Node::height() const
{
	size_t Hei = 0;
	if (left)
	{
		Hei = left->height();
	}
	if (right)
	{
		Hei = std::max(Hei, right->height());
	}
	return Hei+1;
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

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other): _size(other._size)
{
	_root = cp_node(other._root, nullptr, &_root);
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other)
{
	if (this != &other) 
	{
		_size = other._size;
		_root = cp_node(other._root, nullptr, &_root);
	}
	return *this;
}




void BinarySearchTree::nmove(Node* node, Node** roott) // Обновляет _root для всех узлов
{
	if (node == nullptr)
	{
		return;
	}

	node->root = roott;
	nmove(node->left, roott);
	nmove(node->right, roott);
}


BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept : _root(other._root), _size(other._size)
{
	nmove(_root, &_root);
	other._root = nullptr;	
	other._size = 0;
}

BinarySearchTree::Node* BinarySearchTree::cp_node(Node* other, Node* parr, Node** roott)
{
	if (other == nullptr)
	{
	return nullptr;
	}
	Node* node = new Node(other->keyValuePair.first, other->keyValuePair.second, parr);
	node->root = roott;
	node->left = cp_node(other->left, node, roott);
	node->right = cp_node(other->right, node, roott);
	return node;
}


BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
	if (this != &other) 
	{
		_size = other._size;
		_root = other._root;
		nmove(_root, &_root);
		other._root = nullptr;
		other._size = 0;
	}
	return *this;
}

BinarySearchTree::~BinarySearchTree()
{
	Key for_delete;
	while (_size - 1 > 0 && _root != nullptr)
	{
		for_delete = min()->first;
		erase(for_delete);
	}
	delete _root;
}



//void BinarySearchTree::clear() 
//{
//	std::stack<Node*> stack;
//	Node *current = _root;
//
//	while (current || !stack.empty()) 
//	{
//		while (current) 
//		{
//			stack.push(current);
//			current = current->left;
//		}
//		current = stack.top();
//		stack.pop();
//		Node *right = current->right;
//		delete current;
//		current = right;
//	}
//	_root = nullptr;
//	_size = 0;
//}


void BinarySearchTree::insert(const Key &key, const Value &value)
{
	if (!_root) 
	{
		_root = new Node(std::numeric_limits<Key>::max(), 0);
		_size++;
		_root->root = &_root;
		_root->isend = true;
	} 

	_root->insert(key, value);
	_size++;
	
}



void BinarySearchTree::erase(const Key &key)
{
	if (!_root)
	{
		return;
	}
	while ((_size-1 > 0) && (find(key) != end()))
	{
		_root->erase(key);
		_size--;
	}
}



BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
	if (_root == nullptr)
	{
		return ConstIterator(nullptr);
	}


	Node *current = _root;
	while (true) 
	{
		if (current == nullptr)
		{
			return cend();
		}

		if (key < current->keyValuePair.first) 
		{
			if (!current->left)
			{
				return cend();
			}
			current = current->left;
		} else if (key > current->keyValuePair.first) 
		{
			if (current->right == nullptr)
			{
				return cend();
			}
			current = current->right;
			
		} else   //  ЕСЛИ key == keyvaluepair.first
		{
			return ConstIterator(current);
		}
	}
	return cend();
}



BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
	if (_root == nullptr)
	{
		return Iterator(nullptr);
	}


	Node *current = _root;
	while (true) 
	{
		if (current == nullptr)
		{
			return end();
		}

		if (key < current->keyValuePair.first) 
		{
			if (!current->left)
			{
				return end();
			}
			current = current->left;
		} else if (key > current->keyValuePair.first) 
		{
			if (current->right == nullptr)
			{
				return end();
			}
			current = current->right;
			
		} else   //  ЕСЛИ key == keyvaluepair.first
		{
			return Iterator(current);
		}
	}
	return end();
}



std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) 
{
	if (_root == nullptr)
	{
		return std::make_pair(Iterator(nullptr), Iterator(nullptr));
	}


	auto lower = find(key);
	if (lower == end())
	{
		return std::make_pair(end(), end());
	}

	auto upper = lower;
	while (lower->first == upper->first) 
	{
		++upper;
		if (upper == end())
		{
			break;
		}
	}
	return std::make_pair(lower, upper);
}



std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> 
BinarySearchTree::equalRange(const Key &key) const
{
	if (_root == nullptr)
	{
		return std::make_pair(ConstIterator(nullptr), ConstIterator(nullptr));
	}


	auto lower = find(key);
	if (lower == cend())
	{
		return std::make_pair(cend(), cend());
	}

	auto upper = lower;
	while (lower->first == upper->first) 
	{
		++upper;
		if (upper == cend())
		{
			break;
		}
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
	return --ConstIterator(current);
}

// Минимальный элемент с заданным ключом C НАИМЕНЬШИМ ЗНАЧЕНИЕМ!!!

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
	auto range = equalRange(key);
	if (range.first == cend())
	{
		 return cend();
	}
	ConstIterator min_it = range.first;
	for (auto i = range.first; i != range.second; ++i) 
	{
		if (i->second < min_it->second) 
		{
			min_it = i;
		}
	}
	return min_it;
}


BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
	auto range = equalRange(key);
	if (range.first == range.second)
	{
		 return cend();
	}	
	ConstIterator max_it = range.first;
	for (auto i = range.first; i != range.second; ++i) 
	{
		if (i->second > max_it->second) 
		{
			max_it = i;
		}
	}
	return max_it;
}



BinarySearchTree::Iterator BinarySearchTree::begin()
{
	if (!_root) 
	{
		return Iterator(nullptr);
	}
	Node *current = _root;
	while (current->left) 
	{
		current = current->left;
	}
	return Iterator(current);
}


BinarySearchTree::Iterator BinarySearchTree::end()
{
	if (!_root) 
	{
		return Iterator(nullptr);
	}
	Node *current = _root;
	while (current->right)
	{
		current = current->right;
	}
	return (Iterator(current));
}



BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
	return min();
}



BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
	if (!_root) 
	{
		return ConstIterator(nullptr);
	}
	Node *current = _root;
	while (current->right)
	{
		current = current->right;
	}
	return (ConstIterator(current));
}



size_t BinarySearchTree::size() const
{
	if (_size == 0)
	{
		return 0;
	}
	return (_size - 1);
}

void BinarySearchTree::output_tree()
{
	if (_root && _size >= 2) 
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
	if (_root == nullptr)
	{
		return 0;
	}
	return _root->height();
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
	if (!_node || _node->isend) 
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
		if (!parent)
		{
			while (_node->right)
			{
				_node = _node->right;
			}
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
	return ((_node == other._node) && !(_node->isend) && !(other._node->isend)) || ((_node->isend) && (other._node->isend));
}



bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const 
{
	return !(*this == other);
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
	
	if (!_node || _node->isend) 
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
		if (!parent)
		{
			while (_node->right)
			{
				_node = _node->right;
			}
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
	return ((_node == other._node) && !(_node->isend) && !(other._node->isend)) || ((_node->isend) && (other._node->isend));
}


bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
	return !(*this == other);
}


