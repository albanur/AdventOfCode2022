#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<set>

using namespace std;

void PrintPixel(int x, int cycle)
{
    cycle = cycle % 40;
    char pixel = (abs(cycle - x) <= 1) ? '#' : '.';

    printf("%c", pixel);
    if(cycle == 39)
    {
        printf("\n");
    }
}

int main()
{
    char buffer[32];
    char command;
    int diff;
    int x = 1;
    int cycle = 0;
    int signal = 0;
    set<int> moments = { 20, 60, 100, 140, 180, 220 };

    while(fgets(buffer, 32, stdin) != NULL)
    {
        command = buffer[0];
        diff = atoi(buffer+5);

        PrintPixel(x, cycle);
        cycle++;
        if (moments.count(cycle))
        {
            signal += cycle*x;
        }

        if (command == 'a')
        {
            PrintPixel(x, cycle);
            cycle++;
            if (moments.count(cycle))
            {
                signal += cycle*x;
            }

            x += diff;
        }
    }

    printf("%i\n", signal);

    return 0;
}