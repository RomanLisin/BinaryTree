#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#define tab '\t'
void main()
{
	class Tree
	{
		class Element
		{
		public:
			int getValue()
			{
				return Value;
			}
			void setValue(int value)
			{
				this->Value = value;
			}

			Element(int value, int height = 0, Element* pLeft = nullptr, Element* pRight = nullptr)
				:Value(value), Height(height), pLeft(pLeft), pRight(pRight)
			{
				cout << "EConstructor:\t" << this << endl;
			}
			~Element()
			{
				cout << "EDestructor:\t" << this << endl;
			}
			friend class Tree;

		protected:

			int Value;
			int Height;
			Element* pLeft;
			Element* pRight;

		}*Root;
	public:

		Element* getRoot()const
		{
			return Root;
		}

		Tree() :Root(nullptr)
		{
			cout << "TConstructor:\t" << this << endl;
		}
		~Tree()
		{
			cout << "TDestructor:\t" << this << endl;
		}

		void insert(int value, Element* root)
		{ // ���� ������ ������, �� ��������� ������ �������, 
		  //��� ���� ������� ��� �� ��������� ������� ������ �� ����������
			if (find(root, value))return; // ���� �������� ��� ���� �� ���������
			if (this->Root == nullptr)this->Root = new Element(value);
			if (root == nullptr) return;
			if (value < root->Value)
			{
				if (root->pLeft == nullptr) root->pLeft = new Element(value);
				else insert(value, root->pLeft);
			}
			else
			{
				if (root->pRight == nullptr) root->pRight = new Element(value);
				else insert(value, root->pRight);
			}
			updateHeight(root);
			balance(root);
		}

		void insert(int value)
		{
			insert(value, this->Root);
		}

		void clear(Element* root)
		{
			if (root == nullptr)return;
			clear(root->pLeft);
			clear(root->pRight);
			delete root;
		}
		void clear()
		{
			clear(Root);
			Root = nullptr;
		}
		Element* erase(double value, Element* root)
		{
			if (root == nullptr)return nullptr;
			if (value < root->getValue())
			{
				root->pLeft = erase(value, root->pLeft);
			}
			else if (value > root->getValue())
			{
				root->pRight = erase(value, root->pRight);
			}
			else
			{    // ���� ������� ������
				if (root->pLeft == nullptr || root->pRight == nullptr)
				{
					// ���� �� �������� ��� ����
					Element* Temp = (root->pLeft == nullptr) ? root->pRight : root->pLeft;
					if (Temp == nullptr) // ���� ��� ��������, ������ �������
					{
						Temp = root;
						root = nullptr;
					}
					else // ���� ���� ������� �������� �� ����� �������
					{
						*root = *Temp;
					}
					delete Temp;
				}
				else // ���� ���� ��� �������, ������� ������������ ���� � ����� ���������
				{
					Element* maxInLeft = max(root->pLeft);
					// �������� �������� ������������� �������� � ������� �������
					root->Value = maxInLeft->Value;
					// ������� ������������ ������� �� ������ ���������
					root->pLeft = erase(maxInLeft->Value, root->pLeft);
				}
			}
			if (root == nullptr) return root;
			// ��������� ������ ��������
			updateHeight(root);
			// ����������� �������
			balance(root);
			return root;
		}

		void erase(double value)
		{
			erase(value, this->Root);
		}

		Element* min(Element* root)const
		{
			if (root == nullptr)return nullptr;
			return (root->pLeft == nullptr) ? root : min(root->pLeft);
		}
		double min()const
		{
			return  (min(Root) != nullptr) ? min(Root)->Value : INT_MIN;
		}

		Element* max(Element* root)const
		{
			if (root == nullptr)return nullptr;
			return (root->pRight == nullptr) ? root : max(root->pRight);
		}
		double max()const
		{
			return (max(Root) != nullptr) ? max(Root)->Value : INT_MAX;
		}
		double avg(Element* root)const
		{
			return sumValues(root) / count(root);
		}
		double avg()const
		{
			return avg(Root);
		}

		int count(Element* root)const
		{
			int leftCount = 0;
			int rightCount = 0;
			if (root == nullptr)return 0;
			if (root->pLeft != nullptr)leftCount = count(root->pLeft);
			if (root->pRight != nullptr)rightCount = count(root->pRight);
			return 1 + leftCount + rightCount;
		}
		int count()const
		{
			return count(Root);
		}

		double sumValues(Element* root)const
		{
			if (root == nullptr)return 0;
			int sum = root->getValue();
			if (root->pLeft != nullptr) sum += sumValues(root->pLeft);
			if (root->pRight != nullptr) sum += sumValues(root->pRight);
			return sum;
		}
		double sumValues()const
		{
			return sumValues(Root);
		}
		void updateHeight(Element* root)
		{
			if (root == nullptr)return;

			if (getHeight(root->pLeft) > getHeight(root->pRight))
			{
				root->Height = getHeight(root->pLeft) + 1;
			}
			else
			{
				root->Height = getHeight(root->pRight) + 1;
			}
		}

		int getHeight(Element* root)
		{
			return (root == nullptr) ? -1 : root->Height;
		}
		int depth()
		{
			return getHeight(this->Root);
		}
		int getBalance(Element* root)
		{
			return (root == nullptr) ? 0 : getHeight(root->pRight) - getHeight(root->pLeft);
		}
		bool find(Element* root, int value)
		{
			if (root == nullptr) return false;
			if (root->Value == value) return true;
			if (value < root->Value) return find(root->pLeft, value);
			else return find(root->pRight, value);
		}
		void rightRotate(Element* root)
		{
			std::swap(root->Value, root->pLeft->Value);
			Element* Temp = root->pRight;
			root->pRight = root->pLeft;
			root->pLeft = root->pRight->pLeft;
			root->pRight->pLeft = root->pRight->pRight;
			root->pRight->pRight = Temp;
			updateHeight(root->pRight);
			updateHeight(root);
		}
		void leftRotate(Element* root)
		{
			std::swap(root->Value, root->pRight->Value);
			Element* Temp = root->pLeft;
			root->pLeft = root->pRight;
			root->pRight = root->pLeft->pRight;
			root->pRight->pLeft = root->pRight->pRight;
			root->pLeft->pRight = root->pLeft->pLeft;
			root->pLeft->pLeft = Temp;
			updateHeight(root->pLeft);
			updateHeight(root);
		}

		void balance(Element* root)
		{
			int balance = getBalance(root);
			// ���� ������ ����������� �����
			if (balance == -2)
			{
				// ��������� ������ ������ �������
				if (getBalance(root->pLeft) == 1)
				{	// ���� ����� ������� ���������� ������, ��������� ����� �������
					leftRotate(root->pLeft);
				}
				// ��������� ������ �������
				rightRotate(root);
			}
			// ���� ������ ����������� ������
			else if (balance == 2)
			{	// ��������� ������ ������� �������
				if (getBalance(root->pRight) == -1)
				{ // ���� ������ ������� ���������� �����, ��������� ������ �������
					rightRotate(root->pRight);
				}
				//  ��������� ����� �������
				leftRotate(root);
			}
		}
		void print(Element* Root)const
		{
			if (Root == nullptr)return;
			print(Root->pLeft);
			cout << Root->Value << tab;
			print(Root->pRight);
		}
		void print()const
		{
			print(Root);
		}

		void printTree(Element* root, int space = 0, int height = 10) {
			if (root == nullptr) {
				return;
			}

			space += height;

			printTree(root->pRight, space);

			std::cout << std::endl;
			for (int i = height; i < space; i++) {
				std::cout << " ";
			}
			std::cout << root->Value << "\n";

			printTree(root->pLeft, space);
		}
	};

	setlocale(LC_ALL, "");

	int n;
	cout << "������� ���������� ���������:\t"; cin >> n;
	Tree tree;
	for (int i = 0; i < n; i++)
	{
		tree.insert(rand() % 100);  // ����� �������� Root ������, ��������� ���������� ����� getRoot()
	}

	tree.print(tree.getRoot());

	cout << endl << "������������ ������� � ������ �� ���������: " << tree.max() << endl;
	cout << endl << "����������� ������� � ������ �� ���������: " << tree.min() << endl;
	cout << endl << "Count: " << tree.count() << endl;
	cout << endl << "Summ: " << tree.sumValues() << endl;
	cout << endl << "Avg: " << tree.avg() << endl;
	tree.print();
	cout << endl;
	cout << "\n������� ������: " << tree.depth() << endl;
	tree.erase(69);
	//tree.clear();
	tree.print();
	cout << "\n������� ������: " << tree.depth() << endl;
	tree.printTree(tree.getRoot());
}