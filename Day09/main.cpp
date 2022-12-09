#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<set>
#include<vector>

using namespace std;

struct Point
{
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) { }
};

bool operator < (const Point& p1, const Point& p2)
{
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

inline int Sign(int a)
{
    return (0 < a) - (a < 0);
}

void UpdateKnot(const Point& prev, Point& current)
{
    if(abs(prev.x - current.x) == 2 || abs(prev.y - current.y) == 2)
    {
        current.x += Sign(prev.x - current.x);
        current.y += Sign(prev.y - current.y);
    }
}

void UpdateRope(vector<Point>& rope)
{
    for(int i = 1; i < rope.size(); i++)
    {
        UpdateKnot(rope[i-1], rope[i]);
    }
}

int main()
{
    char buffer[32];
    char direction;
    int step, dx, dy;
    set<Point> history;
    vector<Point> rope;
    int ropeSize = 10;

    for(int i = 0; i < ropeSize; i++)
    {
        rope.push_back(Point());
    }

    history.insert(rope.back());

    while(fgets(buffer, 32, stdin) != NULL)
    {
        direction = buffer[0];
        step = atoi(buffer+2);

        switch(direction)
        {
            case 'U':
                dx = 0; dy = 1; break;
            case 'D':
                dx = 0; dy = -1; break;
            case 'R':
                dx = 1; dy = 0; break;
            case 'L':
                dx = -1; dy = 0; break;
        }

        for (int i = 0; i < step; i++)
        {
            rope.front().x += dx;
            rope.front().y += dy;
            UpdateRope(rope);
            history.insert(rope.back());
        }
    }

    printf("%i\n", history.size());

    return 0;
}