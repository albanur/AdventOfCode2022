#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<deque>
#include<string>
#include<map>

using namespace std;

void updatePath(deque<string>& current, map<string,int>& sizes, int fileSize)
{
    string result;

    sizes[result] += fileSize;

    for(int i = 0; i < current.size(); i++)
    {
        result += "/";
        result += current[i];
        sizes[result] += fileSize;
    }
}

int main()
{
    deque<string> current;
    map<string,int> sizes;
    int sum = 0;
    int size;
    int len;
    int toFree;
    int min = INT32_MAX;

    char buffer[1024];

    while(fgets(buffer, 1024, stdin) != NULL)
    {
        if(buffer[0] == '$')
        {
            if(buffer[2] == 'c' && buffer[3] == 'd')
            {
                if(buffer[5] == '/')
                {
                    current.clear();
                }
                else if(buffer[5] == '.' && buffer[6] == '.')
                {
                    current.pop_back();
                }
                else
                {
                    len = strlen(buffer + 5);
                    current.push_back(string(buffer + 5, len-1));
                }
            }
            else if (buffer[2] == 'l' && buffer[3] == 's')
            {
                continue;
            }
        }
        else
        {
            if(buffer[0] == 'd')
            {
                continue;
            }

            size = atoi(buffer);
            updatePath(current, sizes, size);
        }
    }

    sum = 0;
    toFree = sizes[string()] - 40000000;
    for (const auto& kv : sizes)
    {
        if (kv.second <= 100000)
        {
            sum += kv.second;
        }
        if (kv.second >= toFree && kv.second < min)
        {
            min = kv.second;
        }
        //printf("%s : %i\n", kv.first.c_str(), kv.second);
    }

    printf("%i\n", sum);
    printf("%i\n", min);

    return 0;
}