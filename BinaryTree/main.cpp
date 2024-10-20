#include<iostream>
#include<ctime>
#include<cmath>
#include<iomanip>  // for setw (отступов)
#include<algorithm> // for max (для поиска максимального)
#include <queue>
#include<stack>

using namespace std;
using std::cin;
using std::cout;
using std::endl;

//#define DEBUG

#define tab '\t'
#define delimeter "\n-------------------------------------------------------------\n"

	class Tree
	{
	protected:
		class Element
		{
		public:

			Element(int value, Element* pLeft = nullptr, Element* pRight = nullptr)
				:Value(value), pLeft(pLeft), pRight(pRight)
			{
#ifdef DEBUG
				cout << "EConstructor:\t" << this << endl;

#endif // DEBUG
			}
			~Element()
			{
#ifdef DEBUG
				cout << "EDestructor:\t" << this << endl;
#endif // DEBUG

			}
			bool isLeaf()const   // если элемент лист
			{
				return pLeft == pRight;
			}
			friend class Tree;
			friend class UniqueTree;

		protected:

			int Value;
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
#ifdef DEBUG
			cout << "TConstructor:\t" << this << endl;
#endif // DEBUG
		}
		Tree(const std::initializer_list<int>& il) :Tree()   // конструктор
		{
			for (int const* it = il.begin(); it != il.end(); ++it)
				insert(*it, Root);
		}
		~Tree()
		{
			clear();
#ifdef DEBUG
			cout << "TDestructor:\t" << this << endl;
#endif // DEBUG
		}
		void clear()
		{
			clear(Root);
			Root = nullptr;
		}
		void insert(int value)
		{
			insert(value, Root); // this  не нуже, в этом пространстве имен только этот Root
		}
		void erase(int value)
		{
			erase(value, Root);
		}
		int minValue()const
		{
			return  minValue(Root);
		}
		int maxValue()const
		{
			return maxValue(Root);
		}
		int count()const
		{
			return count(Root); // this не нужен 
		}
		int sumValues()const
		{
			return sumValues(Root);
		}
		double avg()const
		{
			return (double)sumValues(Root)/count(Root);
		}

		int depth() {
			return depth(Root);
		}
		void print()
		{
			print(Root);
			cout << endl;
		}
		void printTree()
		{
			printTree(Root);
			cout << endl;
		}
	//private:
	protected:
		void clear(Element* root)
		{
			if (root == nullptr)return;
			clear(root->pLeft);
			clear(root->pRight);
			delete root;
		}
		//void insert(int value, Element* root)
		//{ // если дерево пустое, то добавляет первый элемент, 
		//  //без этой строчки код за следующей строкой ниразу не выполнится
		//	//if (find(value, root))return; // если значение уже есть не вставляем
		//	if (this->Root == nullptr)this->Root = new Element(value);
		//	if (root == nullptr) return;
		//	if (value < root->Value)
		//	{
		//		if (root->pLeft == nullptr) root->pLeft = new Element(value);
		//		else insert(value, root->pLeft);
		//	}
		//	else
		//	{
		//		if (root->pRight == nullptr) root->pRight = new Element(value);
		//		else insert(value, root->pRight);
		//	}
		//	//updateHeight(root);
		//	balance(root);
		//}


		//void insert(int value, Element*& root)
		//{
		//	if (root == nullptr) root = new Element(value); 
		//	else if (value < root->Value) insert(value, root->pLeft); 
		//	else insert(value, root->pRight); 
	
		//	//updateHeight(root);
		//	balance(root);
		//}
		 
		
		// итеративная вставка с флагами и минимизацией вычислений
		void insert(int value ,Element*& root) 
		{
		    std::stack<Element**> path;  // стек для хранения пути к новому элементу
		    Element** current = &root;
		
		    // спускаемся по дереву, пока не найдем место для вставки
		    while (*current != nullptr) {
		        path.push(current);  // запоминаем элементы на пути
		        if (value < (*current)->Value) {
		            current = &((*current)->pLeft);
		        }
		        else {
		            current = &((*current)->pRight);
		        }
		    }
		
		    // вставляем новый элемент
		    *current = new Element(value);
		
		    bool treeChanged = true;  // флаг изменения высоты и необходимости балансировки
		
		    // обратный проход по стеку с минимизацией обновлений
		    while (!path.empty() && treeChanged) 
			{
		        current = path.top();
		        path.pop();
		
				int oldHeight = depth(*current);//->height;
		        //updateHeight(*current);  // обновляем высоту
		
		        // если высота изменилась, проверяем необходимость балансировки
		        if  (depth(*current) != oldHeight) 
				{
		           balance(*current);
		        }
		        else 
				{
		            treeChanged = false;  // если высота не изменилась, можно завершить
		        }
		    }
		}
	
		void erase(int value, Element*& root)
		{
		if (root == nullptr)return ;
		erase(value, root->pLeft);
		erase(value, root->pRight);
		if (value == root->Value)
		{
			//if(root->pLeft==root->pRight)    // что равносильно: root->pLeft == nullptr && root->pRight == nullptr
			// или так:
			if (root->isLeaf())
			{
				delete root;
				root = nullptr;
			}
			else
			{
				if (count(root->pLeft) > count(root->pRight))   // если левая ветка тяжелее, чем правая
				{
					root->Value = maxValue(root->pLeft);
					erase(maxValue(root->pLeft), root->pLeft);  // балансируем
				}
				else
				{
					root->Value = minValue(root->pRight);
					erase(minValue(root->pRight), root->pRight);
				}
			}
		}
			//// балансируем элемент
		//	balance(root);
		}

		int minValue(Element* root)const
		{
			if (root == nullptr)throw std::exception("Error in minValue: Tree is empty");
			return (root->pLeft == nullptr)? root->Value: minValue(root->pLeft);
		}

		int maxValue(Element* root)const
		{
			if (root == nullptr)throw std::exception("Error in maxValue: Tree is empty");
			return root->pRight? maxValue(root->pRight):root->Value;
		}

		int count(Element* root)const
		{
			return !root ? 0 : 1 + count(root->pLeft) + count(root->pRight);
			/*if (root == nullptr)return 0;
			else return 1 + count(root->pLeft) + count(root->pRight);*/
		}

		int sumValues(Element* root)const
		{
			return !root ? 0 : sumValues(root->pLeft) + sumValues(root->pRight) + root->Value;
		}

		int depth(Element* root)
		{
			if (root == nullptr) return 0;
			return 1 + max(depth(root->pLeft), depth(root->pRight));
		}
	
		int getBalance(Element* root)
		{
			return (root == nullptr) ? 0 : depth(root->pRight) - depth(root->pLeft); //getHeight(root->pRight) - getHeight(root->pLeft);
		}

		void rightRotate(Element*& root)
		{
			Element* leftChild = root->pLeft;
			root->pLeft = leftChild->pRight;
			leftChild->pRight = root;
			root = leftChild; // Новый корень поддерева
			/*std::swap(root->Value, root->pLeft->Value);
			Element* Temp = root->pRight;
			root->pRight = root->pLeft;
			root->pLeft = root->pRight->pLeft;
			root->pRight->pLeft = root->pRight->pRight;
			root->pRight->pRight = Temp;
			updateHeight(root->pRight);
			updateHeight(root);*/

		}
		void leftRotate(Element*& root)
		{
			Element* rightChild = root->pRight;
			root->pRight = rightChild->pLeft;
			rightChild->pLeft = root;
			root = rightChild; // Новый корень поддерева
			/*std::swap(root->Value, root->pRight->Value);
			Element* Temp = root->pLeft;
			root->pLeft = root->pRight;
			root->pRight = root->pLeft->pRight;
			root->pRight->pLeft = root->pRight->pRight;
			root->pLeft->pRight = root->pLeft->pLeft;
			root->pLeft->pLeft = Temp;
			updateHeight(root->pLeft);
			updateHeight(root);*/
		}
		
		void balance(Element* root)
		{
			int balance = getBalance(root);
			// если дерево перегружено влево
			if (balance == -2)
			{
				// проверяем баланс левого потомка
				if (getBalance(root->pLeft) > 0)
				{	// если левый потомок перегружен вправо, выполняем левый поворот
					leftRotate(root->pLeft);
				}
				// выполняем правый поворот
				rightRotate(root);
			}
			// Если дерево перегружено вправо
			else if (balance == 2)
			{	
				if (getBalance(root->pRight)< 0)
				{ // если правый потомок перегружен влево, выполняем правый поворот
					rightRotate(root->pRight);
				}
				//  выполняем левый поворот
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

		//void printTree(Element* root) {
		//	if (!root) return;

		//	// Получаем высоту дерева
		//	int height = depth(root);

		//	// Очередь для обхода дерева в ширину
		//	queue<Element*> q;
		//	q.push(root);

		//	int level = 0;  // Текущий уровень
		//	while (level < height) {
		//		int nodesInLevel = pow(2, level);  // Количество узлов на этом уровне
		//		int spaceBetween = pow(2, height - level) - 1;  // Пространство между узлами

		//		// Печать узлов текущего уровня
		//		for (int i = 0; i < nodesInLevel; ++i) {
		//			Element* node = q.front();
		//			q.pop();

		//			if (node) {
		//				cout << setw(spaceBetween) << node->Value;
		//				q.push(node->pLeft);
		//				q.push(node->pRight);
		//			}
		//			else {
		//				cout << setw(spaceBetween) << " ";
		//				q.push(nullptr);
		//				q.push(nullptr);
		//			}
		//		}
		//		cout << endl;  // Переход на новую строку
		//		++level;

		//		// Прерываем цикл, если следующий уровень состоит только из `nullptr`
		//		bool hasMoreNodes = false;
		//		queue<Element*> temp = q;  // Копируем очередь для проверки
		//		while (!temp.empty()) {
		//			if (temp.front() != nullptr) {
		//				hasMoreNodes = true;
		//				break;
		//			}
		//			temp.pop();
		//		}
		//		if (!hasMoreNodes) break;  // Если узлов нет, прерываем вывод
		//	}
		//}
		
		void printTree(Element* root) {
			if (!root) return;

			int height = depth(root);
			int maxElement = pow(2, height) - 1;  // макс кол-во елементов на последнем уровне
			int width = maxElement * 4;  // ширина для печати элементов

			queue<Element*> q;
			q.push(root);

			for (int level = 0; level < height; ++level) {
				int elementsInLevel = pow(2, level);  // число элементов на текущем уровне
				int currentWidth = width / (elementsInLevel + 1);  // расчет отступов 

				// печать текущего уровня
				for (int i = 0; i < elementsInLevel; ++i) {
					Element* root = q.front();
					q.pop();

					if (root) {
						cout << setw(currentWidth) << root->Value;
						q.push(root->pLeft);
						q.push(root->pRight);
					}
					else {
						cout << setw(currentWidth) << " ";
						q.push(nullptr);
						q.push(nullptr);
					}
				}
				cout << endl;  // Переход на новую строку
			}
			//int level = 0;
			//int nodesInLevel = 1;  // Число узлов на текущем уровне

			//while (!q.empty()) {
			//	int currentWidth = width / (nodesInLevel + 1);  // Расчет отступов для узлов

			//	// Печать узлов текущего уровня
			//	for (int i = 0; i < nodesInLevel; ++i) {
			//		Element* root = q.front();
			//		q.pop();

			//		if (root) {
			//			cout << setw(currentWidth) << root->Value;
			//			q.push(root->pLeft);
			//			q.push(root->pRight);
			//		}
			//		else {
			//			cout << setw(currentWidth) << " ";
			//			q.push(nullptr);
			//			q.push(nullptr);
			//		}
			//	}
			//	cout << endl;  // Переход на новую строку

			//	nodesInLevel *= 2;  // На следующем уровне в 2 раза больше узлов
			//	++level;

			//	// Прерываем цикл, если все уровни дерева пройдены
			//	if (level >= height) break;
			//}
		}



	};

	class UniqueTree :public Tree
	{
		void insert(int value, Element* root)
		{ // если дерево пустое, то добавляет первый элемент, 
		  //без этой строчки код за следующей строкой ниразу не выполнится
			if (this->Root == nullptr)this->Root = new Element(value);
			if (root == nullptr) return;
			if (value < root->Value)
			{
				if (root->pLeft == nullptr) root->pLeft = new Element(value);
				else insert(value, root->pLeft);
			}
			if (value>root->Value)
			{
				if (root->pRight == nullptr) root->pRight = new Element(value);
				else insert(value, root->pRight);
			}
				/*updateHeight(root);
				balance(root);*/
		}
		
	public:
		void insert(int value)
		{
			insert(value, Root);
		}
	
	};

#define BASE_CHECK
//#define PERFECT_CHECK
	void main()
	{
		setlocale(LC_ALL, "");
#ifdef BASE_CHECK

		try
		{
			int n;
			cout << "Введите количество элементов:\t"; cin >> n;
			Tree tree;
			clock_t start1 = clock();
			for (int i = 0; i < n; i++)
			{
				tree.insert(rand() % 100);  // чтобы передать Root дерева, необходим константый метод getRoot()
			}

			//tree.print(tree.getRoot());
			clock_t end1 = clock();
			cout << "Выполнено за " << double(end1 - start1) / CLOCKS_PER_SEC << " секунд.\n";
			//cout << "Depth: " << tree.depth() << endl;
			//tree.printTree();
			cout << endl << "Минимальный элемент в дереве со значением: " << tree.minValue() << endl;
			clock_t start2 = clock();
			cout << endl << "Максимальный элемент в дереве со значением: " << tree.maxValue() << endl;
			clock_t end2 = clock();
			cout << " вычислено за " << double(end2 - start2) / CLOCKS_PER_SEC << " секунд.\n";
			clock_t start3 = clock();
			cout << endl << "Количество элементов дерева: " << tree.count() << endl;
			clock_t end3 = clock();
			cout << " вычислено за " << double(end3 - start3) / CLOCKS_PER_SEC << " секунд.\n";
			clock_t start4 = clock();
			cout << endl << "Сумма элементов дерева: " << tree.sumValues() << endl;
			clock_t end4 = clock();
			cout << " вычислено за " << double(end4 - start4) / CLOCKS_PER_SEC << " секунд.\n";
			clock_t start5 = clock();
			cout << endl << "Среднее арефмитическое элементов дерева: " << tree.avg() << endl;
			clock_t end5 = clock();
			cout << " вычислено за " << double(end5 - start5) / CLOCKS_PER_SEC << " секунд.\n";
			/*tree.print();
			cout << endl;*/
			cout << "\nГлубина дерева: " << tree.depth() << endl;

			clock_t start6 = clock();
			tree.erase(41);
			clock_t end6 = clock();
			//cout << " вычислено за " << double(end6 - start6) / CLOCKS_PER_SEC << " секунд.\n";
			////tree.print();
			//cout << endl;
			//tree.clear();
			//tree.print();
			//cout << "\nГлубина дерева: " << tree.depth() << endl;
			//tree.printTree(tree.getRoot());
			//UniqueTree u_tree;
			//for (int i = 0; i < n; i++)
			//{
			//	u_tree.insert(rand() % 100);
			//}
			////u_tree.print();
			//cout << endl << "Минимальный элемент в дереве со значением: " << u_tree.minValue() << endl;
			//cout << endl << "Максимальный элемент в дереве со значением: " << u_tree.maxValue() << endl;
			//cout << endl << "Количество элементов дерева: " << u_tree.count() << endl;
			//cout << endl << "Сумма элементов дерева: " << u_tree.sumValues() << endl;
			//cout << endl << "Среднее арефмитическое элементов дерева: " << u_tree.avg() << endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << endl;
		}
#endif // BASE_CHECK


#ifdef PERFECT_CHECK

		Tree tree =
		{
					50,
			 25,			75,
		  16,   32,      64,   85

		};
		tree.print();
		int value;
		cout << "Введите значение удаляемого элемента: "; cin >> value;
		tree.erase(value);
		tree.print();
#endif // PERFECT_CHECK
		

	}
