#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

struct Node
{
    int x;
    int y;
    int cost;

    Node(int x, int y, int cost): x(x), y(y), cost(cost) {}
};

bool operator < (const Node& node1, const Node& node2)
{
    return node1.cost > node2.cost;
}

int main()
{
    char buffer[128];
    char terrain[128][128];
    int len, sizeX, sizeY;
    Node start(0,0,0);
    Node end(-1,-1,0);
    int i;
    priority_queue<Node> toVisit;

    sizeY = 0;
    while(fgets(buffer, 128, stdin) != NULL)
    {
        len = strlen(buffer) - 1;
        if(len == 0)
        {
            continue;
        }

        sizeX = len;
        for (i = 0; i < len; i++)
        {
            if(buffer[i] == 'S')
            {
                start.x = i;
                start.y = sizeY;
                buffer[i] = 'a';
            }
            if(buffer[i] == 'E')
            {
                end.x = i;
                end.y = sizeY;
                buffer[i] = 'z';
            }
            terrain[i][sizeY] = buffer[i];
        }

        sizeY++;
    }

    toVisit.push(end);

    while(!toVisit.empty())
    {
        Node node = toVisit.top();
        toVisit.pop();

        if(terrain[node.x][node.y] == 'V')
        {
            continue;
        }

        //if(node.x == start.x && node.y == start.y) // Part 1
        if(terrain[node.x][node.y] == 'a') // Part 2
        {
            printf("%i\n", node.cost);
            break;
        }

        if(node.x - 1 >= 0
            && terrain[node.x - 1][node.y] != 'V'
            && terrain[node.x][node.y] - terrain[node.x - 1][node.y] <= 1)
        {
            toVisit.push(Node(node.x - 1, node.y, node.cost+1));
        }
        if(node.x + 1 < sizeX
            && terrain[node.x + 1][node.y] != 'V'
            && terrain[node.x][node.y] - terrain[node.x + 1][node.y] <= 1)
        {
            toVisit.push(Node(node.x + 1, node.y, node.cost+1));
        }
        if(node.y - 1 >= 0
            && terrain[node.x][node.y - 1] != 'V'
            && terrain[node.x][node.y] - terrain[node.x][node.y - 1] <= 1)
        {
            toVisit.push(Node(node.x, node.y - 1, node.cost+1));
        }
        if(node.y + 1 <= sizeY
            && terrain[node.x][node.y + 1] != 'V'
            && terrain[node.x][node.y] - terrain[node.x][node.y + 1] <= 1)
        {
            toVisit.push(Node(node.x, node.y + 1, node.cost+1));
        }

        terrain[node.x][node.y] = 'V';
    }

    return 0;
}