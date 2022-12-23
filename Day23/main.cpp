#include <cstdio>
#include <vector>
#include <set>

using namespace std;

enum Direction
{
    N, NE, E, SE, S, SW, W, NW
};

struct Coordinates
{
    int x;
    int y;

    Coordinates(int x, int y) : x(x), y(y) { }

    Coordinates GetNeighbour(Direction d) const
    {
        switch(d)
        {
            case N: return Coordinates(x, y-1);
            case NE: return Coordinates(x+1, y-1);
            case E: return Coordinates(x+1, y);
            case SE: return Coordinates(x+1, y+1);
            case S: return Coordinates(x, y+1);
            case SW: return Coordinates(x-1, y+1);
            case W: return Coordinates(x-1, y);
            case NW: return Coordinates(x-1, y-1);
        }
    }
};

bool operator < (const Coordinates& p1, const Coordinates& p2)
{
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

struct Elf
{
    Coordinates Position;
    bool ShouldMove;
    Coordinates NextPosition;

    Elf(const Coordinates& position)
        : Position(position), ShouldMove(false), NextPosition(Coordinates(0,0)) { }
};

class ElfOrchestrator
{
private:
    vector<Elf> elves;
    set<Coordinates> currentPositions;
    multiset<Coordinates> nextPositions;
    Direction strategy[4];

public:
    ElfOrchestrator(const vector<Coordinates>& positions)
    {
        currentPositions.clear();
        nextPositions.clear();
        strategy[0] = N;
        strategy[1] = S;
        strategy[2] = W;
        strategy[3] = E;

        for(auto& p : positions)
        {
            elves.push_back(Elf(p));
            currentPositions.insert(p);
        }
    }

    int DoOrchestration()
    {
        bool anyoneShouldMove = true;
        int round = 0;

        while(anyoneShouldMove /*&& round < 10*/)
        {
            anyoneShouldMove = FindNextPositions(round);
            MoveElves();
            round++;

            //PrintField();
        }

        return round;
    }

    int CalculateScore()
    {
        int minX, maxX, minY, maxY;

        minX = maxX = elves.front().Position.x;
        minY = maxY = elves.front().Position.y;

        for(auto& elf : elves)
        {
            if(elf.Position.x < minX)
            {
                minX = elf.Position.x;
            }
            if(elf.Position.x > maxX)
            {
                maxX = elf.Position.x;
            }
            if(elf.Position.y < minY)
            {
                minY = elf.Position.y;
            }
            if(elf.Position.y > maxY)
            {
                maxY = elf.Position.y;
            }
        }

        return (maxX-minX+1) * (maxY-minY+1) - elves.size();
    }

    void PrintField()
    {
        int minX, maxX, minY, maxY;

        minX = maxX = elves.front().Position.x;
        minY = maxY = elves.front().Position.y;

        for(auto& elf : elves)
        {
            if(elf.Position.x < minX)
            {
                minX = elf.Position.x;
            }
            if(elf.Position.x > maxX)
            {
                maxX = elf.Position.x;
            }
            if(elf.Position.y < minY)
            {
                minY = elf.Position.y;
            }
            if(elf.Position.y > maxY)
            {
                maxY = elf.Position.y;
            }
        }

        for(int j = minY; j <= maxY; j++)
        {
            for(int i = minX; i <= maxX; i++)
            {
                if(currentPositions.count(Coordinates(i,j)) > 0)
                {
                    printf("#");
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

private:
    bool IsEmpty(const Coordinates& position)
    {
        return currentPositions.count(position) == 0;
    }

    bool ShouldMove(const Coordinates& position)
    {
        return !(IsEmpty(position.GetNeighbour(N))
            && IsEmpty(position.GetNeighbour(NE))
            && IsEmpty(position.GetNeighbour(E))
            && IsEmpty(position.GetNeighbour(SE))
            && IsEmpty(position.GetNeighbour(S))
            && IsEmpty(position.GetNeighbour(SW))
            && IsEmpty(position.GetNeighbour(W))
            && IsEmpty(position.GetNeighbour(NW)));
    }

    bool CanMoveInDirection(const Coordinates& position, Direction direction)
    {
        switch(direction)
        {
            case N:
                return IsEmpty(position.GetNeighbour(N))
                    && IsEmpty(position.GetNeighbour(NE))
                    && IsEmpty(position.GetNeighbour(NW));
            case S:
                return IsEmpty(position.GetNeighbour(S))
                    && IsEmpty(position.GetNeighbour(SE))
                    && IsEmpty(position.GetNeighbour(SW));
            case E:
                return IsEmpty(position.GetNeighbour(E))
                    && IsEmpty(position.GetNeighbour(NE))
                    && IsEmpty(position.GetNeighbour(SE));
            case W:
                return IsEmpty(position.GetNeighbour(W))
                    && IsEmpty(position.GetNeighbour(SW))
                    && IsEmpty(position.GetNeighbour(NW));
            default:
                return false;
        }
    }

    bool FindNextPositions(int round)
    {
        nextPositions.clear();
        bool anyoneShouldMove = false;

        for(auto& elf : elves)
        {
            elf.ShouldMove = false;
            elf.NextPosition = elf.Position;

            if((elf.ShouldMove = ShouldMove(elf.Position)) == false)
            {
                continue;
            }

            anyoneShouldMove = true;

            for(int i = 0; i < 4; i++)
            {
                int k = (round+i)%4;
                if(CanMoveInDirection(elf.Position, strategy[k]))
                {
                    elf.NextPosition = elf.Position.GetNeighbour(strategy[k]);
                    nextPositions.insert(elf.NextPosition);
                    break;
                }
            }
        }

        return anyoneShouldMove;
    }

    void MoveElves()
    {
        for(auto& elf : elves)
        {
            if(elf.ShouldMove && nextPositions.count(elf.NextPosition) == 1)
            {
                if(currentPositions.count(elf.NextPosition) > 0)
                {
                    printf("FAIL\n");
                }
                currentPositions.erase(elf.Position);
                elf.Position = elf.NextPosition;
                currentPositions.insert(elf.Position);
            }
        }
    }
};

int main()
{
    char buffer[1024];
    vector<Coordinates> elves;
    int i, j;

    i = 0;
    while(NULL != fgets(buffer, 1024, stdin))
    {
        for(j = 0; buffer[j] != '\0'; j++)
        {
            if(buffer[j] == '#')
            {
                elves.push_back(Coordinates(j, i));
            }
        }
        i++;
    }

    ElfOrchestrator orchestrator(elves);
    int rounds = orchestrator.DoOrchestration();

    int score = orchestrator.CalculateScore();
    printf("[%i] %i\n", rounds, score);

    return 0;
}