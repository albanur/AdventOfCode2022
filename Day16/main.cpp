#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <queue>

using namespace std;

struct Room
{
    int Id;
    int FlowRate;
    vector<int> Tunnels;
    map<int, int> BestPaths;

    Room() : Id(0), FlowRate(0) { }
    Room(int id, int flowRate) : Id(id), FlowRate(flowRate) { }
};

struct State
{
    int time;
    set<int> visited;
    int flowPerMinute;
    int releasedPressure;
    int currentRoom;

    State(int roomId)
    {
        time = 0;
        flowPerMinute = 0;
        releasedPressure = 0;
        currentRoom = roomId;
    }
};

struct Worker
{
    int destinationRoom;
    int timeLeft;

    Worker() : destinationRoom(0), timeLeft(0) {}
    Worker(int destination, int cost) : destinationRoom(destination), timeLeft(cost) { }
};

int DoubleLetterToId(const char* letters)
{
    return (int)letters[0]<<8 | (int)letters[1];
}

void FindBestPaths(map<int, Room*>& rooms)
{
    queue<Room*> queue;
    set<int> visited;

    for (auto& kv: rooms)
    {
        Room* room = kv.second;
        queue.push(room);
        room->BestPaths[room->Id] = 0;

        visited.clear();
        visited.insert(room->Id);

        while(!queue.empty())
        {
            Room* r = queue.front();
            queue.pop();
            int cost = room->BestPaths[r->Id] + 1;

            for(int t : r->Tunnels)
            {
                if(visited.count(t))
                {
                    continue;
                }

                visited.insert(t);
                queue.push(rooms[t]);
                room->BestPaths[t] = cost;
            }
        }
    }
}

extern int DecideAndProceed(map<int, Room*>& rooms, State state, vector<Worker>* workers, int maxTime);

int DecideAndProceed(map<int, Room*>& rooms, State state, vector<Worker>* workers, int maxTime)
{
    if(state.time == maxTime)
    {
        return state.releasedPressure;
    }

    for(int i = 0; i < workers->size(); i++)
    {
        
    }
}

int FindMaxReleseadPressure(map<int, Room*>& rooms, int availableWorkers, int maxTime)
{
    State state(DoubleLetterToId("AA"));
    vector<Worker>* workers = new vector<Worker>(2, Worker());
    int result = DecideAndProceed(rooms, state, workers, maxTime);
    delete workers;
    return result;
}

extern int DecideAndMove(map<int, Room*>& rooms, State state, int maxTime);
extern int MoveAndOpenValve(map<int, Room*>& rooms, State state, int roomId, int maxTime);

int FindMaxReleseadPressure(map<int, Room*>& rooms, int maxTime)
{
    State state(DoubleLetterToId("AA"));
    return DecideAndMove(rooms, state, maxTime);
}

int DecideAndMove(map<int, Room*>& rooms, State state, int maxTime)
{
    if(state.time == maxTime)
    {
        return state.releasedPressure;
    }

    int max = 0;

    for(auto& kv : rooms)
    {
        if(state.visited.count(kv.first) > 0 || kv.second->FlowRate <= 0)
        {
            continue;
        }

        if(state.time + rooms[state.currentRoom]->BestPaths[kv.first] + 1 <= maxTime)
        {
            int value = MoveAndOpenValve(rooms, state, kv.first, maxTime);
            if(value > max)
            {
                max = value;
            }
        }
    }

    int value = (maxTime-state.time) * state.flowPerMinute + state.releasedPressure;
    if(value > max)
    {
        max = value;
    }

    return max;
}

int MoveAndOpenValve(map<int, Room*>& rooms, State state, int roomId, int maxTime)
{
    int cost = rooms[state.currentRoom]->BestPaths[roomId] + 1;
    state.currentRoom = roomId;
    state.time += cost;
    state.visited.insert(roomId);
    state.releasedPressure += cost * state.flowPerMinute;
    state.flowPerMinute += rooms[roomId]->FlowRate;

    return DecideAndMove(rooms, state, maxTime);
}

int main()
{
    char id[3];
    char tunnels[256];
    int f;
    map<int, Room*> rooms;

    while(EOF != scanf("Valve %2s has flow rate=%i; tunnel%*[s ]lead%*[s ]to valve%*[s ]", id, &f))
    {
        if (NULL == fgets(tunnels, 256, stdin))
        {
            break;
        }

        Room* r = new Room(DoubleLetterToId(id), f);
        char* token = strtok(tunnels, ", ");
        while(token != NULL)
        {
            r->Tunnels.push_back(DoubleLetterToId(token));
            token = strtok(NULL, ", ");
        }
        rooms[DoubleLetterToId(id)] = r;
    }

    FindBestPaths(rooms);

    int value = FindMaxReleseadPressure(rooms, 30);

    printf("%i\n", value);

    for(auto& kv : rooms)
    {
        delete kv.second;
        kv.second = NULL;
    }

    return 0;
}