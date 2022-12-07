#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<deque>

using namespace std;

int main()
{
    vector<deque<char>> state;
    deque<char> crane;
    char buffer[256];
    int i, j;
    int len;
    int count, start, end;
    char c;

    while(fgets(buffer, 256, stdin) != NULL && buffer[1] != '1')
    {
        len = strlen(buffer);

        if(state.size() == 0)
        {
            state.resize(len/4);
        }

        for(int j = 1; j < len; j+=4)
        {
            if (buffer[j] != ' ')
            {
                i = (j - 1) / 4;
                state[i].push_back(buffer[j]);
            }
        }
    }

    fgets(buffer, 256, stdin);

    while(scanf("move %i from %i to %i\n", &count, &start, &end) != EOF)
    {
        start--;
        end--;

        for(i = 0; i < count; i++)
        {
            c = state[start].front();
            state[start].pop_front();
            crane.push_back(c);
        }

        for(i = 0; i < count; i++)
        {
            c = crane.back();
            crane.pop_back();
            state[end].push_front(c);
        }
    }

    for(i = 0; i < state.size(); i++)
    {
        printf("%c", state[i].front());
    }
    printf("\n");

    return 0;
}