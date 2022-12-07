#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>

using namespace std;

typedef unsigned int uint;

struct Range
{
    uint begin;
    uint end;

    Range() { begin = end = 0; }
    Range(uint b, uint e) { begin = b; end = e; }

    bool Contains(const Range& range)
    {
        return begin <= range.begin && end >= range.end;
    }

    bool Overlaps(const Range& range)
    {
        return (range.begin <= begin && begin <= range.end)
            || (range.begin <= end && end <= range.end)
            || (begin <= range.begin && range.begin <= end)
            || (begin <= range.end && range.end <= end);
    }
};

int main()
{
    uint count = 0;
    Range r1, r2;

    while(scanf("%u-%u,%u-%u", &(r1.begin), &(r1.end), &(r2.begin), &(r2.end)) != EOF)
    {
        if(r1.Overlaps(r2))
        {
            count++;
        }
    }

    printf("%i\n", count);

    return 0;
}