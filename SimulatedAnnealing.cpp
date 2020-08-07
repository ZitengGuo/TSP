/*
 * 模拟退火求解TSP
 */
#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

struct City 
{
    int x;
    int y;
};

constexpr double START_T = 50000.0;			// 初始温度
constexpr double END_T = 1e-8;				// 终止温度
constexpr double ANNEALING_PARAM = 0.98;	// 退火系数
constexpr int ITER = 1000;					// 每个温度下的迭代次数，即链长
constexpr int CITY_SIZE = 31;				// 城市数目

int cityList[CITY_SIZE];
City cityPos[CITY_SIZE] = {
    {1304,2312},{3639,1315},{4177,2244},{3712,1399},
    {3488,1535},{3326,1556},{3238,1229},{4196,1004},
    {4312,790},{4386,570},{3007,1970},{2562,1756},
    {2788,1491},{2381,1676},{1332,695},
    {3715,1678},{3918,2179},{4061,2370},
    {3780,2212},{3676,2578},{4029,2838},
    {4263,2931},{3429,1908},{3507,2367},
    {3394,2643},{3439,3201},{2935,3240},
    {3140,3550},{2545,2357},{2778,2826},
    {2370,2975} };  // 我国31个城市坐标

// 计算两城市之间的距离
double distanceCal(City& city1, City& city2);
// 计算路径长度
double pathLenCal(int* cities);
// 数组复制
void arrCopy(int* arr1, int* arr2, int num);
// 初始化函数
void init();
// 产生新的解:交换随机两个位置
void createNew();

int main()
{
    srand((int)time(NULL));                  // 初始化随机数流
    time_t start, finish;                    // 记录程序运行时间
    start = clock();                         // 程序运行开始计时
    double currentT = START_T;               // 开始时为初始温度
    int counter = 0;                         // 计数器
    init();                                  // 初始化一个解
    int cityListCopy[CITY_SIZE];             // 用于保存原始解
    double fitness1, fitness2, difference2F; // 记录目标值及差
    double randNum;                          // 0-1的随机数，决定是否选择新解

    // 当温度低于结束温度时，退火结束
    while (currentT > END_T)
    {
        for (int i = 0; i < ITER; ++i)
        {
            arrCopy(cityList, cityListCopy, CITY_SIZE);
            createNew();
            fitness1 = pathLenCal(cityListCopy);
            fitness2 = pathLenCal(cityList);
            difference2F = fitness2 - fitness1;
            if (difference2F >= 0)
            {
                randNum = rand() / (RAND_MAX + 1.0);
                if (exp(-difference2F / currentT) <= randNum)
                {
                    arrCopy(cityListCopy, cityList, CITY_SIZE);
                }
            }
        }
        currentT *= ANNEALING_PARAM; // 降温
        ++counter;
    }
    finish = clock();
    double duration = static_cast<double>((finish - start)/CLOCKS_PER_SEC);
    cout << "模拟退火算法" << endl;
    cout << "初始温度：" << START_T << endl;
    cout << "降温系数：" << ANNEALING_PARAM << endl;
    cout << "每个温度迭代次数：" << ITER << endl;
    cout << "共降温" << counter << "次" << endl;
    cout << "得到的最优TSP路径为" << endl;
    for (int i : cityList)
        cout << i << "--->";
    cout << cityList[0] << endl;
    double len = pathLenCal(cityList);
    cout << "最优路径长度：" << len << endl;
    cout << "程序运行时间：" << duration << "秒" <<endl;
    
    return 0;
}

// 计算两城市之间的距离
double distanceCal(City& city1, City& city2)
{
    return sqrt((city1.x - city2.x) * (city1.x - city2.x) +
        (city1.y - city2.y) * (city1.y - city2.y));
}

// 计算路径长度
double pathLenCal(int* cities)
{
    double distanceTotal = 0.0;
    int i, j, k;
    for (i = 0; i < CITY_SIZE - 1; ++i)
    {
        j = cities[i];
        k = cities[i+1];
        distanceTotal += distanceCal(cityPos[j], cityPos[k]);
    }
    j = cities[CITY_SIZE - 1];
    k = cities[0];
    distanceTotal += distanceCal(cityPos[j], cityPos[k]);
    return distanceTotal;
}

// 数组复制
void arrCopy(int* arr1, int* arr2, int num)
{
    for (int i = 0; i < num; ++i)
    {
        arr2[i] = arr1[i];
    }
}

// 初始化函数
void init()
{
    for (int i = 0; i < CITY_SIZE; ++i)
    {
        cityList[i] = i;
    }
}

// 产生新的解:交换随机两个位置
void createNew()
{
    int r1 = rand() % CITY_SIZE;
    int r2 = rand() % CITY_SIZE;
    while (r2 == r1)
    {
        r2 = rand() % CITY_SIZE; // 保证两个位置不同
    }
    int temp = cityList[r1];
    cityList[r1] = cityList[r2];
    cityList[r2] = temp;
}