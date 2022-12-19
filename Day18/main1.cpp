#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

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
        surfaceOX = surfaceXO = surfaceOY = surfaceYO = surfaceOZ = surfaceZO = 1;
    }

    int UncoveredSurfaces()
    {
        return surfaceOX + surfaceXO + surfaceOY + surfaceYO + surfaceOZ + surfaceZO;
    }
};

void Compare(Droplet& d1, Droplet& d2)
{
    if(abs(d1.x - d2.x) + abs(d1.y - d2.y) + abs(d1.z - d2.z) != 1)
    {
        return;
    }

    if(d1.x != d2.x)
    {
        if(d1.x < d2.x)
        {
            d1.surfaceXO = 0;
            d2.surfaceOX = 0;
        }
        else
        {
            d2.surfaceXO = 0;
            d1.surfaceOX = 0;
        }
    }
    if(d1.y != d2.y)
    {
        if(d1.y < d2.y)
        {
            d1.surfaceYO = 0;
            d2.surfaceOY = 0;
        }
        else
        {
            d2.surfaceYO = 0;
            d1.surfaceOY = 0;
        }
    }
    if(d1.z != d2.z)
    {
        if(d1.z < d2.z)
        {
            d1.surfaceZO = 0;
            d2.surfaceOZ = 0;
        }
        else
        {
            d2.surfaceZO = 0;
            d1.surfaceOZ = 0;
        }
    }
}

int main()
{
    int x, y, z;
    vector<Droplet> droplets;

    while(scanf("%i,%i,%i\n", &x, &y, &z) != EOF)
    {
        droplets.push_back(Droplet(x, y, z));
    }

    for(int i = 0; i < droplets.size(); i++)
    {
        for(int j = i+1; j < droplets.size(); j++)
        {
            Compare(droplets[i], droplets[j]);
        }
    }

    int result = 0;
    for(int i = 0; i < droplets.size(); i++)
    {
        result += droplets[i].UncoveredSurfaces();
    }

    printf("%i\n", result);

    return 0;
}