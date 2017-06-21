#include <iostream>
#include <string>

using namespace std;

struct TreeNode;

typedef char ItemType;

enum OrderType { PRE_ORDER, IN_ORDER, POST_ORDER};

class TreeType
{
	public:
	TreeType();
	~TreeType();
	void MakeEmpty();
	bool IsEmpty() const;
	ItemType GetItem(TreeNode* tree, ItemType item, bool& found);
	void PutItem(ItemType item);
	void DeleteItem(ItemType item);
	int CountNodes(TreeNode* tree);
	void Destroy(TreeNode* tree);
	//void Retrieve(TreeNode* tree, ItemType& item, bool& found);
	void Insert(TreeNode*& tree, ItemType item);
	void Delete(TreeNode*& tree, ItemType item);
	void DeleteNode(TreeNode*& tree);
	void GetPredecessor(TreeNode* tree, ItemType& data);
	private:
	TreeNode* root;
};

int main()
{
	string word;
	string letters;
	string key;
	//////////////////////////////create encryption////////////////////////////
	cout << "enter string: ";
	cin >> word;
	
	for(int i = 0; i < word.size(); i++)
	{
		word[i] = tolower(word[i]);
	}
	
	letters = putItem(word);
	cout << "Encryption Tree created successfully with " << CountNodes(letters) << endl;
	
	/////////////////////////encrypt message///////////////////////////
	
	
	
	
	/////////////////////////remove key//////////////////////////
	
	cout << "enter letter to be removed ";
	cin >> key;
	for(int i = 0; i < key.size(); i++)
		{
			key[i] = tolower(key[i]);
		}

		DeleteItem(key);
		cout << "Encryption Tree modified successfully, total nodes " << CountNodes(letters) << endl;
		
		
	/////////////////////////decrypt message//////////////////////
	
	
	system("pause");	
		
}



struct TreeNode
{
	ItemType info;
	TreeNode* left;
	TreeNode* right;
};

TreeType::TreeType()
{
	root = NULL;
}

TreeType::~TreeType()
{
	Destroy(root);
}

void TreeType::Destroy(TreeNode* tree)
{
	if (tree != NULL)
	{
		Destroy(tree->left);
		Destroy(tree->right);
		delete tree;
	}
}

void TreeType::MakeEmpty()
{
	root = NULL;
}

bool TreeType::IsEmpty() const
{
	return root == NULL;
}

ItemType TreeType::GetItem(TreeNode* tree, ItemType item, bool& found)
{
	string str;
		if (tree == NULL)
				found = false;
			if (tree->info == item)
					return str;
			else if (tree->info < item)
					str = str concat "<";
					tree = tree->info left;
				else if (tree->info > item)
					str = str concat "<";
					tree = tree->info right;
		}
}
	
	//Retrieve(root, item, found);
	//return item;
//}

/****
void TreeType::Retrieve(TreeNode* tree, ItemType& item, bool& found)
{
	string str;
	
	if (tree == NULL)
		found = false;
	else if (item < tree->info)
		str = "<";
		Retrieve(tree->left, item, found);
	else if (item > tree->info)
		str = ">";
		Retrieve(tree->right, item, found);
	else 
	{ 
		item = tree->info;
		found = true;
	}
}
******/

void TreeType::PutItem(ItemType item)
{
	Insert(root, item);
}

void TreeType::Insert(TreeNode*& tree, ItemType item)
{
	if (tree == NULL)
	{
		tree = new TreeNode;
		tree->right = NULL;
		tree->left = NULL;
		tree->info = item;
	}
	else if (item < tree->info)
		Insert(tree->left, item);
	else
		Insert(tree->right, item);
}
	

void TreeType::DeleteItem(ItemType item)
{
	Delete(root, item);
}

void TreeType::Delete(TreeNode*& tree, ItemType item)
{
	if (item < tree->info)
		Delete(tree->left, item);
	else if (item > tree->info)
		Delete(tree->right, item);
	else
		DeleteNode(tree);	
}

void TreeType::DeleteNode(TreeNode*& tree)
{
	ItemType data;
	TreeNode* tempPtr;
	
	tempPtr = tree;
	if (tree->left == NULL)
	{
		tree = tree->right;
		delete tempPtr;
	}
	else if (tree->right == NULL)
	{
		tree = tree->left;
		delete tempPtr;
	}
	else
	{
		GetPredecessor(tree->left, data);
		tree->info = data;
		Delete(tree->left, data);
	}
}

void TreeType::GetPredecessor(TreeNode* tree, ItemType& data)
{
	while (tree->right != NULL)
		tree = tree->right;
		data = tree->info;
}

int TreeType::CountNodes(TreeNode* tree)
{
	if (tree == NULL)
	return 0;
	else
	return CountNodes(tree->left) + CountNodes(tree->right) + 1;
}
