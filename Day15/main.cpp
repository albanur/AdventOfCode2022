#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <queue>

using namespace std;

inline int Distance(int x1, int y1, int x2, int y2)
{
    return abs(x1-x2) + abs(y1-y2);
}

struct Sensor
{
    int sx;
    int sy;
    int bx;
    int by;

    Sensor()
    {
        sx = sy = bx = by = 0;
    }

    void Parse(const char* s)
    {
        sscanf(s, "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i", &sx, &sy, &bx, &by);
    }

    int GetRange() const
    {
        return Distance(sx, sy, bx, by);
    }
};

struct Range
{
    int start;
    int end;

    Range() : start(0), end(0) {}
    Range(int start, int end) : start(start), end(end) { }
};

bool operator < (const Range& r1, const Range& r2)
{
    return r1.start > r2.start || (r1.start == r2.start && r1.end > r2.end);
}

int ScanRow(vector<Sensor>& sensors, int row, int low, int high)
{
    priority_queue<Range> queue;

    for(const Sensor& s : sensors)
    {
        int range = s.GetRange();
        int rowDist = abs(row-s.sy);
        int diff = range - rowDist;
        
        if(rowDist <= range)
        {
            Range r = Range(max(low, s.sx-diff), min(high, s.sx+diff));
            if(r.end - r.start >= 0)
            {
                queue.push(r);
            }
        }
    }

    if(queue.size() == 0)
    {
        return 0;
    }

    Range current = queue.top();
    queue.pop();

    while(queue.size() >= 1)
    {
        Range r = queue.top();
        queue.pop();

        if (r.start <= (current.end + 1))
        {
            current.end = max(current.end, r.end);
        }
        else
        {
            return current.end + 1;
        }
    }

    return current.end + 1;
}

int main()
{
    const int row = 2000000;
    char buffer[256];
    vector<Sensor> sensors;
    set<int> free;

    while(NULL != fgets(buffer, 256, stdin))
    {
        Sensor s;
        s.Parse(buffer);
        sensors.push_back(s);
    }

    for(const Sensor& s : sensors)
    {
        int range = s.GetRange();
        int rowDist = abs(row-s.sy);
        
        if(rowDist > range)
        {
            continue;
        }

        free.insert(s.sx);
        for(int i = 1; i <= range - rowDist; i++)
        {
            free.insert(s.sx-i);
            free.insert(s.sx+i);
        }
    }

    for(const Sensor& s : sensors)
    {
        if(s.by == row)
        {
            free.erase(s.bx);
        }
    }

    printf("%i\n", free.size());

    int low = 0;
    int high = 4000000;

    for(int y = low; y <= high; y++)
    {
        int x = ScanRow(sensors, y, low, high);
        if(x >= low && x <= high)
        {
            long long result = (long long)x * 4000000LL + (long long)y;
            printf("%lld\n", result);

            return 0;
        }
    }

    return 0;
}