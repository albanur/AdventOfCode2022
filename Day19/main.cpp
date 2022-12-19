#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cstdint>
#include <vector>

using namespace std;

struct Blueprint
{
    int Id;
    int OreToOreRobot;
    int OreToClayRobot;
    int OreToObsidianRobot;
    int ClayToObsidianRobot;
    int OreToGeodeRobot;
    int ObsidianToGeodeRobot;
   
    Blueprint()
    {
        Id = 0;
        OreToOreRobot = numeric_limits<std::int32_t>::max();
        OreToClayRobot = numeric_limits<std::int32_t>::max();
        OreToObsidianRobot = numeric_limits<std::int32_t>::max();
        ClayToObsidianRobot = numeric_limits<std::int32_t>::max();
        OreToGeodeRobot = numeric_limits<std::int32_t>::max();
        ObsidianToGeodeRobot = numeric_limits<std::int32_t>::max();
    }

    bool TryRead()
    {
        char buffer[128];
       
        do
        {
            if(fgets(buffer, 128, stdin) == NULL)
            {
                return false;
            }
        } while (buffer[0] != 'B');
       
        sscanf(buffer, "Blueprint %i:\n", &Id);
        scanf("  Each ore robot costs %i ore.\n", &OreToOreRobot);
        scanf("  Each clay robot costs %i ore.\n", &OreToClayRobot);
        scanf("  Each obsidian robot costs %i ore and %i clay.\n", &OreToObsidianRobot, &ClayToObsidianRobot);
        scanf("  Each geode robot costs %i ore and %i obsidian.\n", &OreToGeodeRobot, &ObsidianToGeodeRobot);
       
        return true;
    }
};

struct State
{
    int Minute;
    Blueprint& blueprint;
    int OreResources;
    int ClayResources;
    int ObsidianResources;
    int GeodeResources;
    int OreRobots;
    int ClayRobots;
    int ObsidianRobots;
    int GeodeRobots;
   
    State(Blueprint& blueprint) : blueprint(blueprint)
    {
        Minute = 0;
        OreResources = ClayResources = ObsidianResources = GeodeResources = 0;
        OreRobots = 1;
        ClayRobots = ObsidianRobots = GeodeRobots = 0;
    }
   
    bool CanCreateOreRobot()
    {
        return OreResources >= blueprint.OreToOreRobot;
    }
   
    bool CanWaitToCreateOreRobot()
    {
        return OreRobots > 0;
    }
   
    void CreateOreRobot()
    {
        CollectResources();
        OreResources -= blueprint.OreToOreRobot;
        OreRobots++;
    }
   
    bool CanCreateClayRobot()
    {
        return OreResources >= blueprint.OreToClayRobot;
    }
   
    bool CanWaitToCreateClayRobot()
    {
        return OreRobots > 0;
    }
   
    void CreateClayRobot()
    {
        CollectResources();
        OreResources -= blueprint.OreToClayRobot;
        ClayRobots++;
    }
   
    bool CanCreateObsidianRobot()
    {
        return OreResources >= blueprint.OreToObsidianRobot
            && ClayResources >= blueprint.ClayToObsidianRobot;
    }
   
    bool CanWaitToCreateObsidianRobot()
    {
        return OreRobots > 0 && ClayRobots > 0;
    }
   
    void CreateObsidianRobot()
    {
        CollectResources();
        OreResources -= blueprint.OreToObsidianRobot;
        ClayResources -= blueprint.ClayToObsidianRobot;
        ObsidianRobots++;
    }
   
    bool CanCreateGeodeRobot()
    {
        return OreResources >= blueprint.OreToGeodeRobot
            && ObsidianResources >= blueprint.ObsidianToGeodeRobot;
    }
   
    bool CanWaitToCreateGeodeRobot()
    {
        return OreRobots > 0 && ObsidianRobots > 0;
    }
   
    void CreateGeodeRobot()
    {
        CollectResources();
        OreResources -= blueprint.OreToGeodeRobot;
        ObsidianResources -= blueprint.ObsidianToGeodeRobot;
        GeodeRobots++;
    }
   
    void CollectResources()
    {
        OreResources += OreRobots;
        ClayResources += ClayRobots;
        ObsidianResources += ObsidianRobots;
        GeodeResources += GeodeRobots;
        Minute++;
    }
};

enum Decision
{
    WaitAndCreateGeodeRobot,
    WaitAndCreateObsidianRobot,
    WaitAndCreateClayRobot,
    WaitAndCreateOreRobot,
    None,
};

int DecideAndProceed(State& state, Decision decision, int simulationTime)
{
    if(state.Minute == simulationTime)
    {
        // printf("result: {ore: %i, clay: %i, obsidian: %i, geode: %i}\n",
        //     state.OreResources, state.ClayResources, state.ObsidianResources, state.GeodeResources);
        return state.GeodeResources;
    }
   
    if(decision == WaitAndCreateGeodeRobot)
    {
        if(state.CanCreateGeodeRobot())
        {
            state.CreateGeodeRobot();
            return DecideAndProceed(state, None, simulationTime);
        }
        else
        {
            state.CollectResources();
            return DecideAndProceed(state, WaitAndCreateGeodeRobot, simulationTime);
        }
    }
   
    if(decision == WaitAndCreateObsidianRobot)
    {
        if(state.CanCreateObsidianRobot())
        {
            state.CreateObsidianRobot();
            return DecideAndProceed(state, None, simulationTime);
        }
        else
        {
            state.CollectResources();
            return DecideAndProceed(state, WaitAndCreateObsidianRobot, simulationTime);
        }
    }
   
    if(decision == WaitAndCreateClayRobot)
    {
        if(state.CanCreateClayRobot())
        {
            state.CreateClayRobot();
            return DecideAndProceed(state, None, simulationTime);
        }
        else
        {
            state.CollectResources();
            return DecideAndProceed(state, WaitAndCreateClayRobot, simulationTime);
        }
    }
   
    if(decision == WaitAndCreateOreRobot)
    {
        if(state.CanCreateOreRobot())
        {
            state.CreateOreRobot();
            return DecideAndProceed(state, None, simulationTime);
        }
        else
        {
            state.CollectResources();
            return DecideAndProceed(state, WaitAndCreateOreRobot, simulationTime);
        }
    }
   
    if(decision == None)
    {
        int maxResult = 0;
       
        if(state.CanWaitToCreateGeodeRobot())
        {
            State copy = state;
            int result = DecideAndProceed(copy, WaitAndCreateGeodeRobot, simulationTime);
            if (result > maxResult)
            {
                maxResult = result;
            }
        }
       
        if(state.CanWaitToCreateObsidianRobot()
            && state.ObsidianRobots < state.blueprint.ObsidianToGeodeRobot)
        {
            State copy = state;
            int result = DecideAndProceed(copy, WaitAndCreateObsidianRobot, simulationTime);
            if (result > maxResult)
            {
                maxResult = result;
            }
        }
       
        if(state.CanWaitToCreateClayRobot()
            && state.ClayRobots < state.blueprint.ClayToObsidianRobot)
        {
            State copy = state;
            int result = DecideAndProceed(copy, WaitAndCreateClayRobot, simulationTime);
            if (result > maxResult)
            {
                maxResult = result;
            }
        }
       
        if(state.CanWaitToCreateOreRobot()
            && (state.OreRobots < state.blueprint.OreToOreRobot
                || state.OreRobots < state.blueprint.OreToClayRobot
                || state.OreRobots < state.blueprint.OreToObsidianRobot
                || state.OreRobots < state.blueprint.OreToGeodeRobot))
        {
            State copy = state;
            int result = DecideAndProceed(copy, WaitAndCreateOreRobot, simulationTime);
            if (result > maxResult)
            {
                maxResult = result;
            }
        }
       
        return maxResult;
    }
   
    return 0;
}

int Simulate(Blueprint& blueprint)
{
    State state(blueprint);
    return DecideAndProceed(state, None, 32);
}

int main()
{
    vector<Blueprint> blueprints;
    Blueprint temp;
   
    while(temp.TryRead())
    {
        blueprints.push_back(temp);
    }
   
    //int total = 0;
    int total = 1;
    for(Blueprint& b : blueprints)
    {
        int result = Simulate(b);
        //total += result * b.Id;
        total *= result;
        printf("%i : %i\n", b.Id, result);
    }
    printf("Total: %i\n", total);

    return 0;
}