/*
 * 禁忌搜索求解TSP
 */

#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

constexpr int CITY_SIZE = 31;       // 城市规模
constexpr int TABU_SIZE = 25;       // 禁忌长度
constexpr int ITERATION = 1000;     // 迭代次数
constexpr int NEIGHBOR_SIZE = 200;  // 邻域解大小
constexpr double INF = INT_MAX;     // 最大值 

struct City
{
    int x;
    int y;
};
struct Tabu
{
    int i;
    int j;
    double delta;
}candidates[NEIGHBOR_SIZE];

int solution[CITY_SIZE];
int tabuList[CITY_SIZE][CITY_SIZE];
City neighborAll[500];
double adjDistance[CITY_SIZE][CITY_SIZE] = {0.0};
double bestPathDistance;
City cityPosition[CITY_SIZE] = {{1304, 2312}, {3639, 1315}, {4177, 2244}, {3712, 1399}, 
                                {3488, 1535}, {3326, 1556}, {3238, 1229}, {4196, 1004}, 
                                {4312, 790}, {4386, 570}, {3007, 1970}, {2562, 1756}, 
                                {2788, 1491},{2381, 1676}, {1332, 695}, {3715, 1678}, 
                                {3918, 2179}, {4061, 2370}, {3780, 2212}, {3676, 2578}, 
                                {4029, 2838}, {4263, 2931}, {3429, 1908}, {3507, 2367},  
                                {3394, 2643},{3439, 3201}, {2935, 3240}, {3140, 3550},  
                                {2545, 2357}, {2778, 2826},{2370, 2975}};

// 打印数组数据
void printData();
// 交换两个位置的数字
void swap(int &num1, int &num2);
// 数组复制
void arrayCopy(int *arr1, int *arr2);
// 计算两城市之间的距离矩阵
void cityDistance();
// 计算路径总长度
double pathDistance(int *cities);
// 对首尾两个城市分别就前一和后一城市进行调整
int getEdge(int i);
// 计算从solution->neighbor改变的路径长度
double deltaDistance(int &i, int &j, int *cities);
// 产生初始解
void initSolution();
// 产生给定数量的邻域解
void generateNeighbor(const int &totalNum);
// 更新禁忌表
void updateTabuList();
// 更新最优解
void updateSolution(double &solutionDist);
// 禁忌搜索
void tabuSearch(double &solutionDist);

int main()
{
     time_t start, end;
     start = clock();   // 开始计时

	// 计算城市距离矩阵
	cityDistance();
	// 产生初始解
	initSolution();
    // 计算初始解总路径长度
    bestPathDistance = pathDistance(solution);
    // 禁忌搜索
    double solutionDist = bestPathDistance;
    tabuSearch(double &solutionDist);

    end = clock();  // 结束计时
    double duration = static_cast<double>((end - start) / CLOCKS_PER_SEC);
    
    // 打印结果
	return 0;
}

// 打印数组数据
void printData()
{
	for (int i = 0; i < CITY_SIZE; ++i)
	{
        cout << solution[i] << " ";
	}
    cout << endl;
}

// 交换两个位置的数字
void swap(int &num1, int &num2)
{
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

// 数组复制
void arrayCopy(int *arr1, int *arr2)
{
    for (int i = 0; i < CITY_SIZE; ++i)
    {
        arr2[i] = arr1[i];
    }
}

// 计算城市的距离矩阵
void cityDistance()
{
    int x1, x2, y1, y2;
    for (int i = 0; i < CITY_SIZE - 1; ++i)
    {
        for (int j = i + 1; j < CITY_SIZE; ++j)
        {
            x1 = cityPosition[i].x;
            x2 = cityPosition[j].x;
            y1 = cityPosition[i].y;
            y2 = cityPosition[j].y;
            adjDistance[i][j] = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            adjDistance[j][i] = adjDistance[i][j];
        }
        adjDistance[i][i] = INF;
    }
    adjDistance[CITY_SIZE - 1][CITY_SIZE - 1] = INF;
}

// 计算路径总长度
double pathDistance(int *cities)
{
    double distance = adjDistance[cities[CITY_SIZE - 1]][cities[0]];
    for (int i = 0; i < CITY_SIZE - 1; ++i)
    {
        distance += adjDistance[cities[i]][cities[i + 1]];
    }
    return distance;
}

// 对首尾两个城市分别就前一和后一城市进行调整
int getEdge(int i)
{
    // 计算delta时默认i<j
    if (i == 0)
        return CITY_SIZE - 1;
    else if (i == CITY_SIZE - 1)
        return 0;
    else
        return i;
}

// 计算从solution->neighbor改变的路径长度
double deltaDistance(int &i, int &j, int *cities)
{
    double delta = 0.0;
    // 首尾两城市交换路径没有变化
    if (i == 0 && j == CITY_SIZE - 1)
    {
        double d1, d2, d3, d4;
        d1 = adjDistance[cities[i]][cities[i+1]];
        d2 = adjDistance[cities[j-1]][cities[j]];
        d3 = adjDistance[cities[j]][cities[i+1]];
        d4 = adjDistance[cities[j-1]][cities[j]];
        delta = d3 + d4 - d1 - d2;
    }   
    else if (j - i <= 2)
    {
        double d1, d2, d3, d4;
        int i0 = getEdge(i);
        int j1 = getEdge(j);
        d1 = adjDistance[cities[i0]][cities[i]];
        d2 = adjDistance[cities[j]][cities[j1]];
        d3 = adjDistance[cities[i0]][cities[j]];
        d4 = adjDistance[cities[i]][cities[j1]];
        delta = d3 + d4 - d1 - d2;
    }
    else
    {
        double d1, d2, d3, d4, d5, d6, d7, d8;
        int i0 = getEdge(i);
        int j1 = getEdge(j);
        d1 = adjDistance[cities[i0]][cities[i]];
        d2 = adjDistance[cities[i]][cities[i + 1]];
        d3 = adjDistance[cities[j - 1]][cities[j]];
        d4 = adjDistance[cities[j]][cities[j1]];
        d5 = adjDistance[cities[i0]][cities[j]];
        d6 = adjDistance[cities[j]][cities[i + 1]];
        d7 = adjDistance[cities[j - 1]][cities[i]];
        d8 = adjDistance[cities[i]][cities[j1]];
        delta = d5 + d6 + d6 + d8 - d1 - d2 - d3 - d4;
    }
    return delta;
}

// 产生初始解
void initSolution()
{
    int temp[CITY_SIZE];
    for (int i = 0; i < CITY_SIZE; ++i)
    {
        temp[i] = i;
    }
    int r, n = CITY_SIZE - 1;
    while(n != 0)
    {
        r = rand() % n;
        solution[n] = temp[r];
        swap(temp[n],temp[r]);
        --n;
    }
    solution[0] = temp[0];
}

// 产生给定数量的邻域解
void generateNeighbor(const int &totalNum)
{
    int i,j,k = 0;
    int temp[totalNum];
    while (k != NEIGHBOR_SIZE)
    {
        r = rand() % totalNum;
        candidates[k].i = neighborAll[temp[r]].x;
        candidates[k].j = neighborAll[temp[r]].y;
        candidates[k].delta = deltaDistance(candidates[k].i, candidates[k].j, solution);
        swap(temp[k],temp[r]);
        ++k;
    }
}

// 更新禁忌表
void updateTabuList()
{
    for (int i = 0; i < CITY_SIZE - 1; ++i)
    {
        for (int j = i + 1; j < CITY_SIZE; ++j)
            if (tabuList[i][j] > 0)
                tabuList[i][j]--;
    }
}

// 更新最优解
void updateSolution(double &solutionDist)
{
    int bestI = candidates[0].i;
    int bestJ = candidates[0].j;
    double delta, bestD = candidates[0].delta;
    int i, j, flag = 0;
    for (int k = 0; k < NEIGHBOR_SIZE; ++i)
    {
        i = candidates[k].i;
        j = candidates[k].j;
        delta = candidates[k].delta;
        // 如果邻域解不在禁忌表中
        if (tabuList[i][j] <= 0 && delta < bestD)
        {
            bestI = i;
            bestJ = j;
            bestD = delta;
            flag = 0; 
        }
        // 特赦准则：当前邻域解优于历史最优则特赦
        else if (delta < (bestPathDistance - solutionDist))
        {
            bestI = i;
            bestJ = j;
            bestD = delta;
            flag = 1;
        }
    }
    // 更新禁忌表
    if (flag == 0)
    {
        tabuList[bestI][bestJ] = TABU_SIZE;
        updateTabuList();
    }
    else
    {
        tabuList[bestI][bestJ] = 0;
    }
    // 更新当前解
    swap(solution[bestI],solution[bestJ]);
    // 更新最优解的长度
    solutionDist = bestL;
}

// 禁忌搜索
void tabuSearch(double &solutionDist)
{
    // 2-opt所有邻域解
    int totalNum = 0; // 邻域解总数量
    for (int i = 0; i < CITY_SIZE - 1; ++i)
    {
        for (int j = i + 1; j < CITY_SIZE; ++j)
        {
            neighborAll[totalNum].x = i;
            neighborAll[totalNum].y = j;
            ++totalNum;
        }
    }

    //迭代开始
    int iter = ITERATION;
    while (--iter >= 0)
    {
        cout << "当前迭代次数：" << (ITERATION - iter) << endl;
        updateSolution(solutionDist);
        // 更新全局最优
        if (solutionDist < bestPathDistance)
        {
            bestPathDistance = solutionDist;
        }
    }
}
