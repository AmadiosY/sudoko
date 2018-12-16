#define N 9
#define NUM_OF_HOLES 60
#include <iostream>
#include <stdlib.h>
#include <time.h> 
using namespace std;

int copy_dig[N][N];
int sudo[N][N] = { 0 }; //数独终盘
int min_num[N][N] = { 0 }; //搜索数据存储
int max_num[N][N] = { 0 }; 
int num_ini,num,i,j,k,r= 0;
bool flags[N+1] = { false }; //确定最优候选数
int dig[N][N] = { 0 }; //作为挖洞的数独终盘的拷贝
int remaining[N] = { 2 }; //用来随机确定每行保留的数量
bool dig_a_hole[N][N] = { false }; //用来随机确定挖洞的情况


void print_sudo(int list[N][N])
{
	for (i = 0; i
		< N; i++)
	{
		for (j = 0; j < N; j++)
		{
			cout << list[i][j] << " ";
		}
		cout << endl;
	}
}

void init_flag(int i) //初始化当前空格的候选数
{
	for (j = 0; j < i; j++)
	{
		flags[j] = false;
	}
	for (j = i; j < N + 1; j++)
	{
		flags[j] = true;
	}
}

void judge(int list[N][N],int row,int col,int l)
{
	for (j = 0; j < N; j++)//行判断
	{
		k = list[row][j];
		flags[k] = false;
	}
	for (j = 0; j < N; j++)//列判断
	{
		k = list[j][col];
		flags[k] = false;
	}
	j = 3 * (row / 3);
	k = 3 * (col / 3);
	for (int p = j; p < j + 3; p++)//九宫格判断
	{
		for (int q = k; q < k + 3; q++)
		{
			int pq = list[p][q];
			flags[pq] = false;
		}
	}
	if (row == col)//主对角线
	{
		for (int p = 0; p < N; p++)
		{
			int q = list[p][p];
			flags[q] = false;
		}
	}
	if (row + col == N - 1)//副对角线
	{
		for (int p = 0; p < N; p++)
		{
			int q = list[p][N - 1 - p];
			flags[q] = false;
		}
	}
}

bool accepted_final(int num)//判断当前空格是否可以填入
{
	int row = num / N;
	int col = num % N;
	i = min_num[row][col];//i是当前空格中允许的最小数
	init_flag(i);
	judge(sudo, row, col,0);
	for (j = i; j < N + 1; j++)//判断是否可填入
	{
		if (flags[j] == true)
		{
			sudo[row][col] = j;
			min_num[row][col] = j;
			return true;
		}
	}
	return false;
}

inline void back_track()
{
	min_num[num / 9][num % 9] = 1;
	sudo[num / 9][num % 9] = 0;
	num--;
	min_num[num / 9][num % 9]++;
}

void init_max()
{
	for (int h = 0; h < N*N; h++)
	{
		max_num[h / 9][h % 9] = 10;
	}
}

inline void copy_sudo()
{
	memcpy(dig, sudo, N*N*sizeof(int));
	init_max();
}

void get_random_num()//获得每行应该保留的数量即初始化Remaining
{
	for (i = 0; i < N; i++)
	{
		remaining[i] = 2;
	}
	for (i = 0; i < 3; i++)
		remaining[i * 3 + rand() % 3]++;
}

void get_remaining_place(int row,int n)//初始化行n的dig_a_hole的数值
{
	for (int w = 0; w < N; w++)
	{
		dig_a_hole[row][w] = false;
	}
	int p = 0;
	while (p < n)
	{
		int col = rand() % 9;
		if (dig_a_hole[row][col] == false)
		{
			dig_a_hole[row][col] = true;
			p++;
		}
	}
}

bool accepted_first(int n)
{
	int row = n / N;
	int col = n % N;
	int max = max_num[row][col];
	for (int s = 0; s <= max; s++)
	{
		flags[s] = true;
	}
	for (int s = max + 1; s < N + 1; s++)
	{
		flags[s] = false;
	} 
	judge(copy_dig, row, col,1);
	for (int s = N; s > 0; s--)
	{
		if (flags[s] == true)
		{
			copy_dig[row][col] = s;
			max_num[row][col] = s;
			return true;
		}
	}
	return false;
}

bool accept_hole()
{
	r = 0;
	memcpy(copy_dig, dig, N*N * sizeof(int));

	while (r <= num_ini) //从头搜索看是否满足唯一解
	{
		int row = r / 9;
		int col = r % 9;
		if (dig_a_hole[row][col] == true) //判断是否为空格
		{
			r++;
		}
		else
		{
			if (accepted_first(r))//如果位置r满足条件那么进行下一个挑选
			{
				r++;
			}
			else
			{
				max_num[r / 9][r % 9] = 10;
				copy_dig[r / 9][r % 9] = 0;
				r--;
				while (dig_a_hole[r / 9][r % 9] != 0)
				{
					r--;
					if (r < 0)
					{
						return false;
					}
				}
				max_num[r / 9][r % 9]--;
			}//回溯
		}
	}
	for (int s = 0; s < num_ini; s++)
	{
		int row = s / N;
		int col = s % N;
		if (copy_dig[row][col] != sudo[row][col])
		{
			return false;
		}
	}
	return true;
}

void get_sudo_initial()
{
	int mam=0;
	while (num_ini != N * N )
	{
		if (num_ini > mam)
		{
			mam = num_ini;
		}
		//初始化
		int row = num_ini / 9;
		int col = num_ini % 9;
		if (num_ini == 0)
		{
			get_random_num();
			copy_sudo();
		}
		if (col == 0)
		{
			get_remaining_place(row, remaining[row]);
		}
		//开始搜索
		if (dig_a_hole[row][col] == true) 
		{
			num_ini++;
		}
		else//遇到空格
		{
			dig[row][col] = 0;
			max_num[row][col] = 10;
			if (accept_hole())//求解器可以求解到唯一解
			{
				num_ini++;
				init_max();
				
			}
			else //否则销毁初盘重新计算
			{	
				num_ini = 0;
			}
		}
	}
	print_sudo(dig);
}

void get_sudo_final()
{
	int a = clock();
	while (num != N * N)
	{
		if (accepted_final(num))
		{
			num++;
		}
		else
		{
			back_track();
		}
	}
	cout << "终盘为" << endl;
	print_sudo(sudo);
	cout << endl;
	int b = clock();
	cout << "终盘生成时间 " << b - a << " ms" << endl;
	cout << "初盘为" << endl;
	get_sudo_initial();
	int c = clock();
	cout << "初盘生成时间 " << c - a << " ms" << endl;
}

int main()
{
	srand((unsigned)time(NULL));
	get_sudo_final();
	char n;
	while (cout << "请按n获得新终盘,任意键继续" << endl && cin >> n && n == 'n')
	{
		num--;
		num_ini = 0;
		get_sudo_final();
	}
	return 0;
}

