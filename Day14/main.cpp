#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <set>

using namespace std;

struct Coordinates
{
    int x;
    int y;

    Coordinates(): x(0), y(0) { }
    Coordinates(int x, int y) : x(x), y(y) { }
};

bool operator < (const Coordinates& c1, const Coordinates& c2)
{
    return c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y);
}

class Cave
{
private:
    set<Coordinates> rock;
    int lowRock;
    Coordinates sandSource;

public:
    Cave()
    {
        lowRock = 0;
        sandSource = Coordinates(500,0);
    }

    void ParseRock(char* scan)
    {
        Coordinates c1, c2;

        char* pointer = strtok(scan, " ->");
        sscanf(pointer, "%i,%i", &(c1.x), &(c1.y));
        pointer = strtok(NULL, " ->");

        while(pointer != NULL)
        {
            sscanf(pointer, "%i,%i", &(c2.x), &(c2.y));
            AddRock(c1, c2);
            c1 = c2;
            pointer = strtok(NULL, " ->");
        }
    }

    int SimulateSand()
    {
        set<Coordinates> cave(rock.begin(), rock.end());
        int sandCount = 0;

        while(true)
        {
            Coordinates s = sandSource;

            while(s.y <= lowRock)
            {
                if(cave.count(Coordinates(s.x, s.y+1)) == 0) { s.y++; continue; }
                if(cave.count(Coordinates(s.x-1, s.y+1)) == 0) { s.x--; s.y++; continue; }
                if(cave.count(Coordinates(s.x+1, s.y+1)) == 0) { s.x++; s.y++; continue; }
                break;
            }

            if (s.y > lowRock)
            {
                break;
            }

            cave.insert(s);
            sandCount++;
        }

        return sandCount;
    }

    int SimulateSandWithFloor()
    {
        set<Coordinates> cave(rock.begin(), rock.end());
        int sandCount = 0;

        while(true)
        {
            Coordinates s = sandSource;

            while(s.y < (lowRock+1))
            {
                if(cave.count(Coordinates(s.x, s.y+1)) == 0) { s.y++; continue; }
                if(cave.count(Coordinates(s.x-1, s.y+1)) == 0) { s.x--; s.y++; continue; }
                if(cave.count(Coordinates(s.x+1, s.y+1)) == 0) { s.x++; s.y++; continue; }
                break;
            }

            cave.insert(s);
            sandCount++;

            if (s.x == sandSource.x && s.y == sandSource.y)
            {
                break;
            }
        }

        return sandCount;
    }

private:
    inline int Sign(int i)
    {
        return (i == 0)
                ? 0
                : (i < 0) ? -1 : 1;
    }

    inline void UpdateLowRock(int y)
    {
        if(lowRock < y)
        {
            lowRock = y;
        }
    }

    void AddRock(Coordinates& c1, Coordinates& c2)
    {
        //printf("(%i,%i) -> (%i,%i)\n", c1.x, c1.y, c2.x, c2.y);

        int dx = Sign(c2.x - c1.x);
        int dy = Sign(c2.y - c1.y);
        Coordinates c = c1;

        rock.insert(c);
        while(c.x != c2.x || c.y != c2.y)
        {
            c.x += dx;
            c.y += dy;
            rock.insert(c);
        }

        UpdateLowRock(c1.y);
        UpdateLowRock(c2.y);
    }
};

int main()
{
    char buffer[1024];
    Cave cave;

    while(NULL != fgets(buffer, 1024, stdin))
    {
        cave.ParseRock(buffer);
    }

    //int result = cave.SimulateSand();
    int result = cave.SimulateSandWithFloor();
    printf("%i\n", result);

    return 0;
}