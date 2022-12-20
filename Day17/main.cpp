#include <cstdio>
#include <cinttypes>
#include <vector>
#include <deque>
#include <map>

using namespace std;

// .... #... .... #...
#define CAVE_ROW 0x0808

// .... #### #### #...
#define CAVE_FLOOR 0x0FF8

// .... |..# ###. |...
#define ROCK_D 0x01E0ULL

// .... |... #... |...
// .... |..# ##.. |...
// .... |... #... |...
#define ROCK_P 0x008001C00080ULL

// .... |... .#.. |...
// .... |... .#.. |...
// .... |..# ##.. |...
#define ROCK_L 0x0040004001C0ULL

// .... |..# .... |...
// .... |..# .... |...
// .... |..# .... |...
// .... |..# .... |...
#define ROCK_I 0x0100010001000100ULL

// .... |..# #... |...
// .... |..# #... |...
#define ROCK_S 0x01800180ULL

enum JetSide : char
{
    Left = '<',
    Right = '>'
};

class Simulation
{
private:
    const int64_t CaveWindow = 1048576LL;
    const int64_t CaveTreshold = 1048560LL; 

    vector<uint64_t> rocks;
    int rockId;
    vector<JetSide> jets;
    int jetId;
    deque<uint16_t> cave;
    int64_t caveHeight;
    int64_t lowHeight;

public:
    Simulation(const vector<JetSide>& inputJets)
    {
        rocks = {ROCK_D, ROCK_P, ROCK_L, ROCK_I, ROCK_S};
        rockId = 0;
        jets = vector<JetSide>(inputJets);
        jetId = 0;
        cave = deque<uint16_t>(CaveWindow, CAVE_ROW);
        cave[0] = CAVE_FLOOR;
        caveHeight = 0LL;
        lowHeight = 0LL;
    }

    int64_t GetCaveHeight()
    {
        return caveHeight;
    }

    int64_t DoSimulation(int64_t steps)
    {
        int64_t treshold = (int64_t)(rocks.size() * jets.size());

        map<int64_t, int64_t> stateToSteps;
        map<int64_t, int64_t> stateToHeight;

        for(int64_t i = 1LL; i <= steps; i++)
        {                
            SimulateFallingRock();

            int64_t state = jetId * 8 + rockId;
            if (i < treshold || stateToSteps.count(state) == 0)
            {
                stateToSteps[state] = i;
                stateToHeight[state] = caveHeight;
            }
            else
            {
                int64_t initialSteps = stateToSteps[state];
                int64_t initialHeight = stateToHeight[state];
                int64_t cycleSteps = i - initialSteps;
                int64_t cycleHeight = caveHeight - initialHeight;
                int64_t reminderSteps = (steps-initialSteps)%cycleSteps;

                for(int64_t j = 1LL; j <= reminderSteps; j++)
                {
                    SimulateFallingRock();
                }

                int64_t reminderHeight = caveHeight - cycleHeight - initialHeight;

                return initialHeight + ((steps-initialSteps)/cycleSteps) * cycleHeight + reminderHeight;
            }
        }
        return caveHeight;
    }

private:
    void SimulateFallingRock()
    {
        uint64_t rock = GetNextRock();
        int64_t height = caveHeight + 5;

        bool canFall = true;
        while(canFall)
        {
            height--;
            uint64_t cavePart = GetCaveAtHeight(height);

            JetSide jet = GetNextJet();
            if(jet == Left)
            {
                rock<<=1;
                if ((cavePart & rock) != 0) rock>>=1;
            }
            else
            {
                rock>>=1;
                if ((cavePart & rock) != 0) rock<<=1;
            }

            cavePart = GetCaveAtHeight(height - 1);
            canFall = (cavePart & rock) == 0;
        }

        AddRockAtHeight(rock, height);
    }

    uint64_t GetNextRock()
    {
        uint64_t rock = rocks[rockId];
        rockId = (rockId+1)%rocks.size();
        return rock;
    }

    JetSide GetNextJet()
    {
        JetSide jet = jets[jetId];
        jetId = (jetId+1)%jets.size();
        return jet;
    }

    uint64_t GetCaveAtHeight(int64_t height)
    {
        height -= lowHeight;
        return ((uint64_t)cave[height+3])<<48
            | ((uint64_t)cave[height+2])<<32
            | ((uint64_t)cave[height+1])<<16
            | ((uint64_t)cave[height]);
    }

    void AddRockAtHeight(uint64_t rock, int64_t height)
    {
        uint64_t cavePart = GetCaveAtHeight(height);
        height -= lowHeight;

        cavePart |= rock;
        cave[height+3] = uint16_t((cavePart>>48) & 0xFFFFULL);
        cave[height+2] = uint16_t((cavePart>>32) & 0xFFFFULL);
        cave[height+1] = uint16_t((cavePart>>16) & 0xFFFFULL);
        cave[height] = uint16_t(cavePart & 0xFFFFULL);

        while(cave[caveHeight-lowHeight+1] != CAVE_ROW)
        {
            caveHeight++;
            if(caveHeight-lowHeight > CaveTreshold)
            {
                cave.pop_front();
                cave.push_back(CAVE_ROW);
                lowHeight++;
            }
        }
    }
};

int main()
{
    vector<JetSide> jets;
    char c;

    while((c = getchar()) != EOF)
    {
        jets.push_back((JetSide)c);
    }

    Simulation s1(jets);
    int64_t result1 = s1.DoSimulation(2022);
    printf("%i: %lld\n", 2022, result1);

    Simulation s2(jets);
    int64_t result2 = s2.DoSimulation(1000000000000LL);
    printf("%lld: %lld\n", 1000000000000LL, result2);

    return 0;
}
