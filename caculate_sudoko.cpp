//��VS����Ҫ����������
//��һ��ѡ��򿪺͹ر�SDL����λ�þ��ǣ���Ŀ����->��������->C/C++->SDL��飬ѡ���ǻ��߷�
//���������˶Խ���Լ��

#define N 9
#include<iostream>
#include <limits.h>
#include <vector>
#include<time.h>
using namespace std;

struct Node {
	Node *up, *down, *left, *right, *rowRoot, *colRoot;
	int Num;//��¼����
	int Size;//��¼��Ԫ�ظ���
	Node(int i = -1) :Num(i), Size(0) {}; //��ʼ��
};
Node *Head;
int updates = 0;
vector<int> result;

void init_head() //��ʼ��ͷ�ڵ�
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
	for (int i = 0; i < col; i++) //��Ԫ�س�ʼ��
	{
		newnode = new Node;
		newnode->down = newnode;
		newnode->up = newnode;
		newnode->right = Head->right;
		newnode->left = Head;
		newnode->right->left = newnode;
		Head->right = newnode;
	}
	for (int i = 0; i < row; i++)//��Ԫ�س�ʼ��
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

			newnode->up = current_col->up; //���ӵ��������β��
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

			newnode->right = current; //���ӵ���Ԫ�غ�
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
	cRoot->left->right = cRoot->right;//ɾ���ж���
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
	for (c = Head->right; c != Head; c = c->right)//ÿ��Ѱ����С���н���ɸѡ
	{
		if (c->Size < minSize)
		{
			minSize = c->Size;
			cRoot = c;
			if (minSize == 1)
				break;
			if (minSize == 0)//��һ��Ϊ��,ʧ��
				return false;
		}
	}
	del_col(cRoot);//������

	Node *current_row, *current;//
	for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
	{
		result.push_back(current_row->rowRoot->Num);//�����м���result��
		for (current = current_row->right; current != current_row; current = current->right)
		{
			del_col(current->colRoot);
		}
		if (search(k + 1))
			return true;
		for (current = current_row->left; current != current_row; current = current->left)
			recover(current->colRoot);
		result.pop_back();//���ָ��в�����Ҫ��,��ԭresult
	}
	recover(cRoot);
	return false;
}

vector<vector<int> > sudo2matrix(vector<vector<int> > sudo) //����ת��Ϊ01����
{
	vector<vector<int> > matrix;
	int current_row_num = 0; //��ǰ��
	for (vector<vector<int> >::iterator it = sudo.begin(); it != sudo.end(); ++it)
	{
		for (unsigned int ix = 0; ix<(*it).size(); ix++)
		{
			int val = (*it)[ix];//��õ�ǰ�����ֵ

			if (val != 0)//�����ǰ���������
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

			for (int jx = 0; jx < 9; ++jx)//����ȡ1...9
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

void get_sudo(vector<vector<int> > &sudo)//��������

{
	vector<int> temp;
	int num;
	cout << "��˳������ÿ��Ԫ�أ���Ϊ��������0" << endl;

	sudo.clear();
	for (int i = 0; i < N; i++)
	{
		temp.clear();
		cout << "�������" << i+1 << "��Ԫ��" << endl;
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
	clock_t start_1 = clock();//����ת��Ϊ����
	vector<vector<int> > matrix = sudo2matrix(sudo);
	clock_t end_1 = clock();
	float time_1 = (float)(end_1 - start_1) / CLOCKS_PER_SEC;
	cout << "����01������ʱ: " << time_1 << "s\n";

	clock_t start_2 = clock();//����DLX
	init_head();
	init_Dlx(matrix.size(), 342);
	link(matrix, matrix.size(), 342);
	clock_t end_2 = clock();
	float time_2 = (float)(end_2 - start_2) / CLOCKS_PER_SEC;
	cout << "����������ʱ: " << time_2 << "s\n";

	clock_t start_3 = clock();
	if (!search(0))
	{
		cout << "�������޽�!\n\n";
		return;
	}
	clock_t end_3 = clock();
	float time_3 = (float)(end_3 - start_3) / CLOCKS_PER_SEC;
	cout << "Dancing��ʱ: " << time_3 << "s\n";

	clock_t start_4 = clock();
	vector<int> solution = matrix2sudo(matrix);
	clock_t end_4 = clock();
	float time_4 = (float)(end_4 - start_4) / CLOCKS_PER_SEC;

	for (int ix = 0; ix < 81; ++ix)
		cout << solution[ix] << ((ix + 1) % 9 ? '\0' : '\n');

	cout << "Dancing���´���: " << updates << "��\n";
	cout << "��������ʱ: " << time_4 << "s\n" << endl;
}

int main()
{
	vector<vector<int> > sudo;
	get_sudo(sudo);
	get_result(sudo);
	return 0;
}
