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
    int currentRoom;
    int destinationRoom;
    int timeLeft;

    Worker() : currentRoom(0), destinationRoom(0), timeLeft(0) {}
    Worker(int room) : currentRoom(room), destinationRoom(room), timeLeft(0) {}
};

struct State2
{
    int time;
    set<int> roomsToVisit;
    int flowPerMinute;
    int releasedPressure;
    Worker worker1;
    Worker worker2;

    State2(int roomId)
    {
        time = 0;
        flowPerMinute = 0;
        releasedPressure = 0;
        worker1 = Worker(roomId);
        worker2 = Worker(roomId);
    }
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

extern int DecideAndMove2(map<int, Room*>& rooms, State2 state, int maxTime);
extern int MoveAndOpenValve2(map<int, Room*>& rooms, State2 state, int maxTime);

int FindMaxReleseadPressure2(map<int, Room*>& rooms, int maxTime)
{
    State2 state(DoubleLetterToId("AA"));
    for(auto& kv : rooms)
    {
        if(kv.second->FlowRate > 0)
        state.roomsToVisit.insert(kv.first);
    }
    return DecideAndMove2(rooms, state, maxTime);
}

inline int Score(map<int, Room*>& rooms, int remainingTime, int startRoom, int endRoom)
{
    int cost = rooms[startRoom]->BestPaths[endRoom] + 1;
    if (cost >= remainingTime)
    {
        return 0;
    }

    return (remainingTime - cost) * rooms[endRoom]->FlowRate;
}

void GenerateSingleDecisions(map<int, Room*>& rooms, State2& state, int maxTime, int startRoom, vector<int>& decisions)
{
    int max = 0;
    vector<int> all;

    for(const auto& d : state.roomsToVisit)
    {
        int score = Score(rooms, maxTime - state.time, startRoom, d);
        if (score > 0)
        {
            all.push_back(d);
            if(score > max) { max = score; }
        }
    }

    if (all.size() == 0)
    {
        decisions.push_back(-1);
        return;
    }

    int threshold = max/2;
    for(int d : all)
    {
        if (Score(rooms, maxTime - state.time, startRoom, d) >= threshold)
        {
            decisions.push_back(d);
        }
    }
}

void GenerateDoubleDecisions0(State2& state, vector<pair<int, int>>& decisions)
{
    if (state.roomsToVisit.size() == 0)
    {
        decisions.push_back(pair<int,int>(-1,-1));
        return;
    }
    if (state.roomsToVisit.size() == 1)
    {
        decisions.push_back(pair<int,int>(*(state.roomsToVisit.begin()) ,-1));
        return;
    }

    vector<int> d(state.roomsToVisit.begin(), state.roomsToVisit.end());

    for(int i = 0; i < d.size(); i++)
    {
        for(int j = i+1; j < d.size(); j++)
        {
            decisions.push_back(pair<int,int>(d[i],d[j]));
        }
    }
}

void GenerateDoubleDecisions(map<int, Room*>& rooms, State2& state, int maxTime, vector<pair<int, int>>& decisions)
{
    if (state.roomsToVisit.size() == 0)
    {
        decisions.push_back(pair<int,int>(-1,-1));
        return;
    }
    if (state.roomsToVisit.size() == 1)
    {
        decisions.push_back(pair<int,int>(*(state.roomsToVisit.begin()) ,-1));
        return;
    }

    //vector<int> d(state.roomsToVisit.begin(), state.roomsToVisit.end());
    vector<int> d1, d2;
    GenerateSingleDecisions(rooms, state, maxTime, state.worker1.currentRoom, d1);
    GenerateSingleDecisions(rooms, state, maxTime, state.worker2.currentRoom, d2);

    for(int i = 0; i < d1.size(); i++)
    {
        for(int j = 0; j < d2.size(); j++)
        {
            if (d1[i] != d2[j])
                decisions.push_back(pair<int,int>(d1[i],d2[j]));
        }
    }

    if (decisions.size() == 0)
    {
        decisions.push_back(pair<int,int>(-1,-1));
    }
}

int DecideAndMove2(map<int, Room*>& rooms, State2 state, int maxTime)
{
    if(state.time == maxTime)
    {
        return state.releasedPressure;
    }

    int max = 0;

    if (state.worker1.destinationRoom == state.worker1.currentRoom
        && state.worker2.destinationRoom == state.worker2.currentRoom)
    {
        vector<pair<int,int>> decisions;
        if(state.time == 0)
        {
            GenerateDoubleDecisions0(state, decisions);
        }
        else
        {
            GenerateDoubleDecisions(rooms, state, maxTime, decisions);
        }

        for(auto& d : decisions)
        {
            if (d.first >= 0)
            {
                state.worker1.destinationRoom = d.first;
                state.worker1.timeLeft = rooms[state.worker1.currentRoom]->BestPaths[state.worker1.destinationRoom] + 1;
            }
            else
            {
                state.worker1.destinationRoom = d.first;
                state.worker1.timeLeft = maxTime + 1;
            }

            if (d.second >= 0)
            {
                state.worker2.destinationRoom = d.second;
                state.worker2.timeLeft = rooms[state.worker2.currentRoom]->BestPaths[state.worker2.destinationRoom] + 1;
            }
            else
            {
                state.worker2.destinationRoom = d.second;
                state.worker2.timeLeft = maxTime + 1;
            }

            state.roomsToVisit.erase(d.first);
            state.roomsToVisit.erase(d.second);

            int value = MoveAndOpenValve2(rooms, state, maxTime);
            if(value > max) { max = value; }

            if(d.first >= 0) { state.roomsToVisit.insert(d.first); }
            if(d.second >= 0) { state.roomsToVisit.insert(d.second); }
        }
    }
    else if (state.worker1.destinationRoom == state.worker1.currentRoom)
    {
        vector<int> decisions;
        GenerateSingleDecisions(rooms, state, maxTime, state.worker1.currentRoom, decisions);

        for(auto& d : decisions)
        {
            if (d >= 0)
            {
                state.worker1.destinationRoom = d;
                state.worker1.timeLeft = rooms[state.worker1.currentRoom]->BestPaths[state.worker1.destinationRoom] + 1;
            }
            else
            {
                state.worker1.destinationRoom = d;
                state.worker1.timeLeft = maxTime + 1;
            }

            state.roomsToVisit.erase(d);

            int value = MoveAndOpenValve2(rooms, state, maxTime);
            if(value > max) { max = value; }

            if(d >= 0) { state.roomsToVisit.insert(d); }
        }
    }
    else if (state.worker2.destinationRoom == state.worker2.currentRoom)
    {
        vector<int> decisions;
        GenerateSingleDecisions(rooms, state, maxTime, state.worker2.currentRoom, decisions);

        for(auto& d : decisions)
        {
            if (d >= 0)
            {
                state.worker2.destinationRoom = d;
                state.worker2.timeLeft = rooms[state.worker2.currentRoom]->BestPaths[state.worker2.destinationRoom] + 1;
            }
            else
            {
                state.worker2.destinationRoom = d;
                state.worker2.timeLeft = maxTime + 1;
            }

            state.roomsToVisit.erase(d);

            int value = MoveAndOpenValve2(rooms, state, maxTime);
            if(value > max) { max = value; }

            if(d >= 0) { state.roomsToVisit.insert(d); }
        }
    }

    return max;
}

int MoveAndOpenValve2(map<int, Room*>& rooms, State2 state, int maxTime)
{
    int time = min(state.worker1.timeLeft, state.worker2.timeLeft);
    time = min(time, maxTime - state.time);

    state.time += time;
    state.releasedPressure += time * state.flowPerMinute;
    state.worker1.timeLeft -= time;
    state.worker2.timeLeft -= time;

    if (state.worker1.timeLeft == 0)
    {
        state.worker1.currentRoom = state.worker1.destinationRoom;
        state.flowPerMinute += rooms[state.worker1.destinationRoom]->FlowRate;
    }
    if (state.worker2.timeLeft == 0)
    {
        state.worker2.currentRoom = state.worker2.destinationRoom;
        state.flowPerMinute += rooms[state.worker2.destinationRoom]->FlowRate;
    }

    return DecideAndMove2(rooms, state, maxTime);
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

    //int value = FindMaxReleseadPressure(rooms, 30);
    int value = FindMaxReleseadPressure2(rooms, 26);

    printf("%i\n", value);

    for(auto& kv : rooms)
    {
        delete kv.second;
        kv.second = NULL;
    }

    return 0;
}