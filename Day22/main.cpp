#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

enum Direction : int
{
    Right = 0,
    Left = 2,
    Up = 3,
    Down = 1
};

struct State
{
    int x;
    int y;
    Direction direction;

    State() : x(0), y(0), direction(Right) { }
};

bool IsPartOfMap(const char* row)
{
    int len = strlen(row);
    return len>1;
}

bool IsInsideTheMap(vector<string>& map, int x, int y)
{
    if(y < 0 || y >= map.size())
    {
        return false;
    }
    if(x < 0 || x >= map[y].size())
    {
        return false;
    }

    return map[y][x] != ' ' && map[y][x] != '\n';
}

void WrapTheMap(vector<string>& map, int& x, int& y, int dx, int dy)
{
    while(IsInsideTheMap(map, x, y))
    {
        x-=dx;
        y-=dy;
    }

    x+=dx;
    y+=dy;
}

void WrapTheCube(vector<string>& map, int& x, int& y, Direction& dir)
{
    int ly = map.size()/4;
    int lx = map[0].size()/3;

    int sx = x / lx;
    int sy = y / ly;
    
    int ix = x % lx;
    int iy = y % ly;


    if(sx == 2 && sy == 0 &&  dir == Right)
    {
        x = 2*lx - 1;
        y = 3*ly - 1 - iy;
        dir = Left;
        return;
    }
    if(sx == 1 && sy == 2 && dir == Right)
    {
        x = 3*lx - 1;
        y = ly - 1 - iy;
        dir = Left;
        return;
    }

    if(sx == 2 && sy == 0 && dir == Down)
    {
        x = 2*lx - 1;
        y = ly + ix;
        dir = Left;
        return;
    }
    if(sx == 1 && sy == 1 && dir == Right)
    {
        x = 2 * lx + iy;
        y = ly - 1;
        dir = Up;
        return;
    }

    if(sx == 2 && sy == 0 && dir == Up)
    {
        x = ix;
        y = 4*ly - 1;
        dir = Up;
        return;
    }
    if(sx == 0 && sy == 3 && dir == Down)
    {
        x = 2 * lx + ix;
        y = 0;
        dir = Down;
        return;
    }

    if(sx == 1 && sy == 0 && dir == Up)
    {
        x = 0;
        y = 3*ly + ix;
        dir = Right;
        return;
    }
    if(sx == 0 && sy == 3 && dir == Left)
    {
        x = lx + iy;
        y = 0;
        dir = Down;
        return;
    }

    if(sx == 1 && sy == 0 && dir == Left)
    {
        x = 0;
        y = 3*ly - 1 - iy;
        dir = Right;
        return;
    }
    if(sx == 0 && sy == 2 && dir == Left)
    {
        x = lx;
        y = ly - 1 - iy;
        dir = Right;
        return;
    }

    if(sx == 1 && sy == 1 && dir == Left)
    {
        x = iy;
        y = 2*ly;
        dir = Down;
        return;
    }
    if(sx == 0 && sy == 2 && dir == Up)
    {
        x = lx;
        y = ly + ix;
        dir = Right;
        return;
    }

    if(sx == 1 && sy == 2 && dir == Down)
    {
        x = lx - 1;
        y = 3*ly + ix;
        dir = Left;
        return;
    }
    if(sx == 0 && sy == 3 && dir == Right)
    {
        x = lx + iy;
        y = 3*ly - 1;
        dir = Up;
        return;
    }

    return;
}

void MoveBy(vector<string>& map, State& s, int d)
{
    //printf("[%i, %i, %i] move by %i\n", s.x, s.y, s.direction, d);

    int dx = 0;
    int dy = 0;

    while(d > 0)
    {
        dx = 0;
        dy = 0;
        switch(s.direction)
        {
            case Right:
                dx=1; break;
            case Left:
                dx=-1; break;
            case Up:
                dy=-1; break;
            case Down:
                dy=1; break;
        }

        int nx = s.x + dx;
        int ny = s.y + dy;
        Direction ndir = s.direction;

        if(!IsInsideTheMap(map, nx, ny))
        {
            nx = s.x;
            ny = s.y;
            //WrapTheMap(map, nx, ny, dx, dy);
            WrapTheCube(map, nx, ny, ndir);
            if(!IsInsideTheMap(map, nx, ny))
            {
                printf("FAIL to move\n");
                return;
            }
        }

        if (map[ny][nx] == '#')
        {
            break;
        }

        if (map[ny][nx] != '.')
        {
            printf("FAIL to move\n");
            return;
        }

        s.x = nx;
        s.y = ny;
        s.direction = ndir;

        d--;
    }
}

void Turn(State& s, char t)
{
    //printf("[%i, %i, %i] turn %c\n", s.x, s.y, s.direction, t);

    if (t != 'R' && t != 'L')
    {
        printf("FAIL to turn\n");
        return;
    }

    switch(s.direction)
    {
        case Right:
            s.direction = (t == 'R') ? Down : Up; break;
        case Left:
            s.direction = (t == 'L') ? Down : Up; break;
        case Up:
            s.direction = (t == 'R') ? Right : Left; break;
        case Down:
            s.direction = (t == 'L') ? Right : Left; break;
    }
}

int main()
{
    vector<string> map;
    string instruction;
    char buffer[1024];
    State s;

    while(true)
    {
        fgets(buffer, 1024, stdin);
        if(!IsPartOfMap(buffer))
        {
            break;
        }
        map.push_back(buffer);
    }

    while(NULL != fgets(buffer, 1024, stdin))
    {
        instruction.append(buffer);
    }

    while(map[s.y][s.x] != '.') { s.x++; }

    int d = 0;
    int i = 0;
    while(instruction[i] != '\n' && instruction[i] != '\0')
    {
        if(isdigit(instruction[i]))
        {
            d = instruction[i] - '0';
            while(isdigit(instruction[++i]))
            {
                d*=10;
                d += instruction[i] - '0';
            }
            MoveBy(map, s, d);
            continue;
        }
        if(instruction[i] == 'R' || instruction[i] == 'L')
        {
            Turn(s, instruction[i]);
            i++;
            continue;
        }
    }

    int result = (s.y+1)*1000 + (s.x+1)*4 + s.direction;
    printf("%i\n", result);

    return 0;
}