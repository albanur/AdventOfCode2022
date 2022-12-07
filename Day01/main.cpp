#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>

using namespace std;

class Max3
{
private: 
    int max1;
    int max2;
    int max3;

public:
    Max3()
    {
        max1 = max2 = max3 = 0;
    }

    void UpdateMax(int value)
    {
        if (value > max1)
        {
            max3 = max2;
            max2 = max1;
            max1 = value;
        }
        else if (value > max2)
        {
            max3 = max2;
            max2 = value;
        }
        else if (value > max3)
        {
            max3 = value;
        }
    }

    int Sum()
    {
        return max1 + max2 + max3;
    }
};

int main()
{
    vector<int> calories;
    int calorie;
    char buffer[256];
    Max3 max;
    int current = 0;

    while(fgets(buffer, 256, stdin) != NULL)
    {
        if(strlen(buffer) > 1)
        {
            calorie = atoi(buffer);
            current += calorie;
        }
        else
        {
            max.UpdateMax(current);
            calorie = -1;
            current = 0;
        }

        calories.push_back(calorie);
    }

    max.UpdateMax(current);

    printf("%i\n", max.Sum());

    return 0;
}