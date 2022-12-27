#include <cstdio>
#include <vector>
#include <set>
#include <queue>

using namespace std;

#define EMPTY 0x00
#define WALL 0x10
#define BLIZZARD_UP 0x08
#define BLIZZARD_DOWN 0x04
#define BLIZZARD_LEFT 0x02
#define BLIZZARD_RIGHT 0x01
#define BLIZZARD_MASK 0x0F

enum Direction
{
    Up, Down, Left, Right
};

struct Coordinates
{
    int x;
    int y;

    Coordinates() : x(0), y (0) { }
    Coordinates(int x, int y) : x(x), y(y) { }

    Coordinates GetNeighbour(Direction d) const
    {
        switch(d)
        {
            case Up: return Coordinates(x, y-1);
            case Right: return Coordinates(x+1, y);
            case Down: return Coordinates(x, y+1);
            case Left: return Coordinates(x-1, y);
        }
    }
};

bool operator < (const Coordinates& c1, const Coordinates& c2)
{
    return c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y);
}

class Field
{
private:
    char state;

public:
    Field() : state(0) { }
    Field(char state) : state(state) { }

    inline bool IsWall() { return (state & WALL) != 0; }
    inline bool IsEmpty() {return state == 0; }
    inline bool HasAnyBlizzard() { return (state & BLIZZARD_MASK) != 0; }
    inline bool HasBlizzard(char blizzard) { return (state & blizzard) != 0; }

    void AddBlizzard(char blizzard)
    {
        if(!IsWall())
        {
            state |= blizzard;
        }
    }
};

class Valley
{
private:
    vector<vector<Field>> area;
    int width;
    int height;
    Coordinates entryPoint;
    Coordinates exitPoint;

public:
    Valley()
    {
        width = height = 0;
        entryPoint = exitPoint = Coordinates(0, 0);
    }

    void Parse()
    {
        char buffer[1024];
        int row = 0;
        int i;

        while(NULL != fgets(buffer, 1024, stdin))
        {
            area.push_back(vector<Field>());
            for(i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++)
            {
                area[row].push_back(ParseCharacter(buffer[i]));
            }

            if (i > width) { width = i; }
            row++;
        }

        height = row;

        FindEntryPoint();
        FindExitPoint();
    }

    int FindShortestRouteOneWay()
    {
        return FindShortestRoute(entryPoint, exitPoint);
    }

    int FindShortestRouteBackAndForth()
    {
        int route1 = FindShortestRoute(entryPoint, exitPoint);
        int route2 = FindShortestRoute(exitPoint, entryPoint);
        int route3 = FindShortestRoute(entryPoint, exitPoint);

        return route1 + route2 + route3;
    }

private:
    Field ParseCharacter(char c)
    {
        switch(c)
        {
            case '#':
                return Field(WALL);
            case '.':
                return Field(EMPTY);
            case '<':
                return Field(BLIZZARD_LEFT);
            case '>':
                return Field(BLIZZARD_RIGHT);
            case '^':
                return Field(BLIZZARD_UP);
            case 'v':
                return Field(BLIZZARD_DOWN);
            default:
                return Field(WALL);
        }
    }

    void FindEntryPoint()
    {
        for(int i = 1; i < width; i++)
        {
            if (area[0][i].IsEmpty())
            {
                entryPoint = Coordinates(i, 0);
                return;
            }
        }
    }

    void FindExitPoint()
    {
        for(int i = 1; i < width; i++)
        {
            if (area[height-1][i].IsEmpty())
            {
                exitPoint = Coordinates(i, height-1);
                return;
            }
        }
    }

    void MoveBlizzardsForward()
    {
        vector<vector<Field>> newArea(height, vector(width, Field(EMPTY)));

        for(int j = 0; j < height; j++)
        {
            for(int i = 0; i < width; i++)
            {
                if(area[j][i].IsWall())
                {
                    newArea[j][i] = Field(WALL);
                    continue;
                }
                if(area[j][i].IsEmpty())
                {
                    continue;
                }
                if(area[j][i].HasBlizzard(BLIZZARD_UP))
                {
                    int k = (j == 1) ? (height - 2) : (j - 1);
                    newArea[k][i].AddBlizzard(BLIZZARD_UP);
                }
                if(area[j][i].HasBlizzard(BLIZZARD_DOWN))
                {
                    int k = (j == (height - 2)) ? (1) : (j + 1);
                    newArea[k][i].AddBlizzard(BLIZZARD_DOWN);
                }
                if(area[j][i].HasBlizzard(BLIZZARD_LEFT))
                {
                    int k = (i == 1) ? (width - 2) : (i - 1);
                    newArea[j][k].AddBlizzard(BLIZZARD_LEFT);
                }
                if(area[j][i].HasBlizzard(BLIZZARD_RIGHT))
                {
                    int k = (i == (width - 2)) ? (1) : (i + 1);
                    newArea[j][k].AddBlizzard(BLIZZARD_RIGHT);
                }
            }
        }

        area = newArea;
    }

    int FindShortestRoute(Coordinates start, Coordinates end)
    {
        int steps = 0;
        set<Coordinates> places;
        queue<Coordinates> queue;

        places.insert(start);
        while(places.count(end) != 1)
        {
            MoveBlizzardsForward();

            for(auto c : places)
            {
                queue.push(c);
                queue.push(c.GetNeighbour(Right));
                queue.push(c.GetNeighbour(Down));
                queue.push(c.GetNeighbour(Left));
                queue.push(c.GetNeighbour(Up));
            }
            places.clear();

            while(!queue.empty())
            {
                Coordinates c = queue.front();
                queue.pop();

                if(c.x < 0 || c.x >= width || c.y < 0 || c.y >= height)
                {
                    continue;
                }

                if(area[c.y][c.x].IsEmpty())
                {
                    places.insert(c);
                }
            }

            steps++;
        }

        places.clear();
        return steps;
    }
};

int main()
{
    Valley valley;

    valley.Parse();
    int result = valley.FindShortestRouteBackAndForth();

    printf("%i\n", result);

    return 0;
}