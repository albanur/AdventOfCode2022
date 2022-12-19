#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>

using namespace std;

struct Droplet
{
    int x;
    int y;
    int z;

    signed char surfaceOX;
    signed char surfaceXO;
    signed char surfaceOY;
    signed char surfaceYO;
    signed char surfaceOZ;
    signed char surfaceZO;

    Droplet(int x, int y, int z) : x(x), y(y), z(z)
    {
        surfaceOX = surfaceXO = surfaceOY = surfaceYO = surfaceOZ = surfaceZO = 0;
    }

    int ReachableSurfaces()
    {
        return surfaceOX + surfaceXO + surfaceOY + surfaceYO + surfaceOZ + surfaceZO;
    }

    void MarkOutsideSurface(const Droplet& outside)
    {
        if(abs(x - outside.x) + abs(y - outside.y) + abs(z - outside.z) != 1)
        {
            return;
        }

        if(x != outside.x)
        {
            if(x < outside.x)
            {
                surfaceXO = 1;
            }
            else
            {
                surfaceOX = 1;
            }
        }
        if(y != outside.y)
        {
            if(y < outside.y)
            {
                surfaceYO = 1;
            }
            else
            {
                surfaceOY = 1;
            }
        }
        if(z != outside.z)
        {
            if(z < outside.z)
            {
                surfaceZO = 1;
            }
            else
            {
                surfaceOZ = 1;
            }
        }
    }
};

int main()
{
    const int MinPos = 0;
    const int MaxPos = 21;
    const int Size = 22;

    int x, y, z;
    vector<Droplet> droplets;

    vector<vector<vector<Droplet*>>> area;
    Droplet outside(-1,-1,-1);

    area.resize(Size);
    for(auto& a : area)
    {
        a.resize(Size);
        for(auto& b : a) b.resize(Size);
    }

    while(scanf("%i,%i,%i\n", &x, &y, &z) != EOF)
    {
        droplets.push_back(Droplet(x+1, y+1, z+1));
    }

    for(Droplet& d : droplets)
    {
        area[d.x][d.y][d.z] = &d;
    }

    queue<Droplet> queue;
    queue.push(Droplet(MinPos,MinPos,MinPos));

    while(!queue.empty())
    {
        Droplet d = queue.front();
        queue.pop();

        if(area[d.x][d.y][d.z] != NULL)
        {
            continue;
        }

        area[d.x][d.y][d.z] = &outside;

        Droplet* next;
        if(d.x - 1 >= MinPos)
        {
            next = area[d.x-1][d.y][d.z];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x-1, d.y, d.z));
            }
        }
        if(d.x + 1 <= MaxPos)
        {
            next = area[d.x+1][d.y][d.z];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x+1, d.y, d.z));
            }
        }
        if(d.y - 1 >= MinPos)
        {
            next = area[d.x][d.y-1][d.z];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x, d.y-1, d.z));
            }
        }
        if(d.y + 1 <= MaxPos)
        {
            next = area[d.x][d.y+1][d.z];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x, d.y+1, d.z));
            }
        }
        if(d.z - 1 >= MinPos)
        {
            next = area[d.x][d.y][d.z-1];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x, d.y, d.z-1));
            }
        }
        if(d.z + 1 <= MaxPos)
        {
            next = area[d.x][d.y][d.z+1];
            if(next != NULL && next != &outside)
            {
                next->MarkOutsideSurface(d);
            }
            if(next == NULL)
            {
                queue.push(Droplet(d.x, d.y, d.z+1));
            }
        }
    }

    int result = 0;
    for(int i = 0; i < droplets.size(); i++)
    {
        result += droplets[i].ReachableSurfaces();
    }

    printf("%i\n", result);

    return 0;
}