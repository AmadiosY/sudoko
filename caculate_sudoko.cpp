//在VS中需要做如下设置
//有一个选择打开和关闭SDL检查的位置就是：项目属性->配置属性->C/C++->SDL检查，选测是或者否。
//这里增加了对角线约束

#define N 9
#include<iostream>
#include <limits.h>
#include <vector>
#include<time.h>
using namespace std;

struct Node {
	Node *up, *down, *left, *right, *rowRoot, *colRoot;
	int Num;//记录行数
	int Size;//记录列元素个数
	Node(int i = -1) :Num(i), Size(0) {}; //初始化
};
Node *Head;
int updates = 0;
vector<int> result;

void init_head() //初始化头节点
{
	Head = new Node;
	Head->up = Head;
	Head->down = Head;
	Head->left = Head;
	Head->right = Head;
}

void init_Dlx(int row, int col)
{
	Node *newnode;
	for (int i = 0; i < col; i++) //列元素初始化
	{
		newnode = new Node;
		newnode->down = newnode;
		newnode->up = newnode;
		newnode->right = Head->right;
		newnode->left = Head;
		newnode->right->left = newnode;
		Head->right = newnode;
	}
	for (int i = 0; i < row; i++)//行元素初始化
	{
		newnode = new Node(row - i);
		newnode->down = Head->down;
		newnode->up = Head;
		newnode->down->up = newnode;
		Head->down = newnode;
	}
}

void link(vector<vector<int> > &matrix , int row , int col)
{
	Node *current_row, *current_col, *newnode, *current;
	current_row = Head;
	for (int r = 0; r < row; r++)
	{
		current_row = current_row->down;
		current_col = Head;
		for (int c = 0; c < col; c++)
		{
			current_col = current_col->right;
			if (matrix[r][c] == 0)
				continue;
			newnode = new Node;

			newnode->rowRoot = current_row;
			newnode->colRoot = current_col;

			newnode->up = current_col->up; //链接到列链表的尾端
			newnode->down = current_col;
			newnode->up->down = newnode;
			current_col->up = newnode;

			if (current_row->Size == 0)
			{
				current_row->right = newnode;
				current_row->Size++;
				newnode->right = newnode;
				newnode->left = newnode;
			}

			current = current_row->right;

			newnode->right = current; //连接到行元素后
			newnode->left = current->left;
			newnode->left -> right = newnode;
			current->left = newnode;

			current_col->Size++;
		}
	}
}

void del_col(Node *cRoot)
{
	updates++;
	cRoot->right->left = cRoot->left;
	cRoot->left->right = cRoot->right;//删除列对象
	Node *i,*j;
	i = cRoot->down;
	while (i != cRoot)
	{
		j = i->right;
		while (j != i)
		{
			j->down->up = j->up;
			j->up->down = j->down;
			j->colRoot->Size--;
			j = j->right;
		}
		i = i->down;
	}
}

void recover(Node *cRoot)
{
	Node *i, *j;
	i = cRoot->up;
	while (i != cRoot)
	{
		j = i->left;
		while (j != i)
		{
			j->colRoot->Size++;
			j->down->up = j;
			j->up->down = j;
			j = j->left;
		}
		i = i->up;
	}
	cRoot->right->left = cRoot;
	cRoot->left->right = cRoot;
}

bool search(int k)
{
	if (Head->right == Head)
		return true;

	Node *cRoot, *c;
	int minSize = INT_MAX;
	for (c = Head->right; c != Head; c = c->right)//每次寻找最小的列进行筛选
	{
		if (c->Size < minSize)
		{
			minSize = c->Size;
			cRoot = c;
			if (minSize == 1)
				break;
			if (minSize == 0)//有一列为空,失败
				return false;
		}
	}
	del_col(cRoot);//覆盖列

	Node *current_row, *current;//
	for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
	{
		result.push_back(current_row->rowRoot->Num);//将该行加入result中
		for (current = current_row->right; current != current_row; current = current->right)
		{
			del_col(current->colRoot);
		}
		if (search(k + 1))
			return true;
		for (current = current_row->left; current != current_row; current = current->left)
			recover(current->colRoot);
		result.pop_back();//发现该行不符合要求,还原result
	}
	recover(cRoot);
	return false;
}

vector<vector<int> > sudo2matrix(vector<vector<int> > sudo) //数独转换为01矩阵
{
	vector<vector<int> > matrix;
	int current_row_num = 0; //当前行
	for (vector<vector<int> >::iterator it = sudo.begin(); it != sudo.end(); ++it)
	{
		for (unsigned int ix = 0; ix<(*it).size(); ix++)
		{
			int val = (*it)[ix];//获得当前格的数值

			if (val != 0)//如果当前格存在数字
			{
				vector<int> current_row(342, 0);
				current_row[ix + N * current_row_num] = 1;
				current_row[81 + current_row_num * 9 + val - 1] = 1;
				current_row[162 + ix * 9 + val - 1] = 1;
				current_row[243 + (current_row_num / 3 * 3 + ix / 3) * 9 + val - 1] = 1;

				if (ix == current_row_num)
					current_row[324 + val - 1] = 1;
				if (ix + current_row_num == N - 1)
					current_row[333 + val - 1] = 1;

				matrix.push_back(current_row);
				continue;
			}

			for (int jx = 0; jx < 9; ++jx)//否则取1...9
			{
				vector<int> current_row2(342, 0);
				current_row2[ix + N * current_row_num] = 1;
				current_row2[81 + current_row_num * 9 + jx] = 1;
				current_row2[162 + ix * 9 + jx] = 1;
				current_row2[243 + (current_row_num / 3 * 3 + ix / 3) * 9 + jx] = 1;

				if (ix == current_row_num)
					current_row2[324 + jx] = 1;
				if (ix + current_row_num == N - 1)
					current_row2[333 + jx] = 1;

				matrix.push_back(current_row2);
			}
		}
		current_row_num++;
	}

	return matrix;
}

vector<int> matrix2sudo(vector<vector<int> > matrix)
{
	vector<int> solution(81);
	for (int ix = 0; ix < 81; ++ix)
	{
		vector<int> current = matrix[result[ix] - 1];
		int pos = 0, val = 0;
		for (int jx = 0; jx < 81; ++jx)
		{
			if (current[jx] == 1)
				break;
			++pos;
		}
		for (int kx = 81; kx < 162; ++kx)
		{
			if (current[kx] == 1)
				break;
			++val;
		}
		solution[pos] = val % 9 + 1;
	}
	return solution;
}

void get_sudo(vector<vector<int> > &sudo)//数独输入

{
	vector<int> temp;
	int num;
	cout << "请顺序输入每行元素，如为空请填入0" << endl;

	sudo.clear();
	for (int i = 0; i < N; i++)
	{
		temp.clear();
		cout << "请输入第" << i+1 << "行元素" << endl;
		for (int j = 0; j < N; j++)
		{
			cin >> num;
			temp.push_back(num);
		}
		sudo.push_back(temp);
	}
}

void get_result(vector<vector<int> > sudo)
{
	clock_t start_1 = clock();//数独转化为矩阵
	vector<vector<int> > matrix = sudo2matrix(sudo);
	clock_t end_1 = clock();
	float time_1 = (float)(end_1 - start_1) / CLOCKS_PER_SEC;
	cout << "构造01矩阵用时: " << time_1 << "s\n";

	clock_t start_2 = clock();//构造DLX
	init_head();
	init_Dlx(matrix.size(), 342);
	link(matrix, matrix.size(), 342);
	clock_t end_2 = clock();
	float time_2 = (float)(end_2 - start_2) / CLOCKS_PER_SEC;
	cout << "构造链表用时: " << time_2 << "s\n";

	clock_t start_3 = clock();
	if (!search(0))
	{
		cout << "该数独无解!\n\n";
		return;
	}
	clock_t end_3 = clock();
	float time_3 = (float)(end_3 - start_3) / CLOCKS_PER_SEC;
	cout << "Dancing用时: " << time_3 << "s\n";

	clock_t start_4 = clock();
	vector<int> solution = matrix2sudo(matrix);
	clock_t end_4 = clock();
	float time_4 = (float)(end_4 - start_4) / CLOCKS_PER_SEC;

	for (int ix = 0; ix < 81; ++ix)
		cout << solution[ix] << ((ix + 1) % 9 ? '\0' : '\n');

	cout << "Dancing更新次数: " << updates << "次\n";
	cout << "翻译结果用时: " << time_4 << "s\n" << endl;
}

int main()
{
	vector<vector<int> > sudo;
	get_sudo(sudo);
	get_result(sudo);
	return 0;
}
