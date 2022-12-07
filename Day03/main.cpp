#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>

using namespace std;

int Priority(char c)
{
    if ('a' <= c && c <= 'z')
    {
        return c - 'a' + 1;
    }

    if ('A' <= c && c <= 'Z')
    {
        return c - 'A' + 27;
    }

    return 0;
}

int main1()
{
    int score = 0;
    char buffer[256];
    int size = 0;
    int count[53];

    while(fgets(buffer, 256, stdin) != NULL)
    {
        size = strlen(buffer) / 2;
        memset(count, 0, 53*sizeof(int));

        for(int i = 0; i < size; i++)
        {
            int p = Priority(buffer[i]);
            count[p]++;
        }

        for(int i = size; i < 2*size; i++)
        {
            int p = Priority(buffer[i]);
            if (count[p] != 0)
            {
                score += p;
                break;
            }
        }
    }

    printf("%i\n", score);

    return 0;
}

int main()
{
    int score = 0;
    int size = 0;
    char buffer[256];
    int count1[53];
    int count2[53];
    int count3[53];

    while(fgets(buffer, 256, stdin) != NULL)
    {
        memset(count1, 0, 53*sizeof(int));
        memset(count2, 0, 53*sizeof(int));
        memset(count3, 0, 53*sizeof(int));

        size = strlen(buffer);
        for(int i = 0; i < size; i++)
        {
            int p = Priority(buffer[i]);
            count1[p]++;
        }

        fgets(buffer, 256, stdin);
        size = strlen(buffer);
        for(int i = 0; i < size; i++)
        {
            int p = Priority(buffer[i]);
            count2[p]++;
        }

        fgets(buffer, 256, stdin);
        size = strlen(buffer);
        for(int i = 0; i < size; i++)
        {
            int p = Priority(buffer[i]);
            count3[p]++;
        }

        for(int i = 1; i < 53; i++)
        {
            if (count1[i] != 0 && count2[i] != 0 && count3[i] != 0)
            {
                score += i;
                break;
            }
        }
    }

    printf("%i\n", score);

    return 0;
}