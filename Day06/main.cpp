#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>

using namespace std;

bool hasDuplicates(vector<char>& code, int start, int length)
{
    for(int i = start; i<(start+length-1); i++)
    {
        for(int j = i+1; j<(start+length); j++)
        {
            if(code[i] == code[j])
            {
                return true;
            }
        }
    }

    return false;
}

int main()
{
    vector<char> code;
    int i, j;
    int count;

    while((i = getchar()) != EOF)
    {
        code.push_back((char)i);
    }

    i = 0;
    while(hasDuplicates(code, i, 14))
    {
        i++;
    }

    printf("%i\n", i+14);

    return 0;
}