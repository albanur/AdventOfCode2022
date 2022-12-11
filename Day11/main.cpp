#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

enum Operation : char
{
    Addition = '+',
    Multiplication = '*',
    Square = '^',
};

class Monkey
{
private:
    int id;
    queue<int64_t> items;
    Operation operation;
    int operationArgument;
    int64_t operationDivisor;
    int levelReducer;
    int decisionDivisor;
    int positiveDecisionMonkeyId;
    int negativeDecisionMonkeyId;
    int inspectionCounter;

    function<void(int, int64_t)> throwItemTo;

public:
    Monkey(int id = 0)
    {
        operationDivisor = 0;
        inspectionCounter = 0;
    }

    void UpdateOperation(Operation operation, int operationArgument = 0)
    {
        this->operation = operation;
        this->operationArgument = operationArgument;
    }

    void UpdateOperationDivisor(int64_t operationDivisor)
    {
        this->operationDivisor = operationDivisor;
    }

    void SetLevelReducer(int value)
    {
        levelReducer = value;
    }

    void UpdateThrowDecision(int decisionDivisor, int positiveMonkeyId, int negativeMonkeyId)
    {
        this->decisionDivisor = decisionDivisor;
        this->positiveDecisionMonkeyId = positiveMonkeyId;
        this->negativeDecisionMonkeyId = negativeMonkeyId;
    }

    void UpdateThrowOperation(function<void(int, int64_t)> throwItemTo)
    {
        this->throwItemTo = throwItemTo;
    }

    void ExecuteTurn()
    {
        while(HasItems())
        {
            int64_t item = TakeItem();
            ExecuteOperation(item);
            ReduceLevel(item);
            ThrowItem(item);
            inspectionCounter++;
        }
    }

    void AddItem(int64_t item)
    {
        items.push(item);
    }

    int GetDecisionDivisor()
    {
        return decisionDivisor;
    }

    int GetInspectionCounter()
    {
        return inspectionCounter;
    }

private:

    bool HasItems()
    {
        return !items.empty();
    }

    int64_t TakeItem()
    {
        int64_t item = items.front();
        items.pop();
        return item;
    }

    void ExecuteOperation(int64_t& item)
    {
        switch(operation)
        {
            case Addition:
                item += operationArgument;
                break;
            case Multiplication:
                item *= operationArgument;
                break;
            case Square:
                item *= item;
                break;
        }

        if (operationDivisor != 0)
        {
            int64_t divisor = ((operationDivisor % levelReducer) == 0)
                ? operationDivisor
                : operationDivisor * levelReducer;
            item %= divisor;
        }
    }

    void ReduceLevel(int64_t& item)
    {
        item /= levelReducer;
    }

    void ThrowItem(int64_t item)
    {
        int nextMonkeyId = (item % decisionDivisor) == 0
            ? positiveDecisionMonkeyId
            : negativeDecisionMonkeyId;

        throwItemTo(nextMonkeyId, item);
    }
};

class MonkeyParser
{
private:
    const static int BufferSize = 256;
    char buffer[BufferSize];

public:
    MonkeyParser() {}

    bool FindNextMonkey()
    {
        while(ReadLine())
        {
            if (IsFirstMonkeyLine())
            {
                return true;
            }
        }
        return false;
    }

    Monkey ReadMonkey()
    {
        Monkey monkey = ParseMonkeyHeader();
        ParseStartingItems(monkey);
        ParseOperation(monkey);
        ParseTest(monkey);

        return monkey;
    }

private:
    bool ReadLine()
    {
        return fgets(buffer, BufferSize, stdin) != NULL;
    }

    bool IsFirstMonkeyLine()
    {
        return buffer[0] == 'M';
    }

    Monkey ParseMonkeyHeader()
    {
        int id = atoi(buffer + 7);

        return Monkey(id);
    }

    void ParseStartingItems(Monkey& monkey)
    {
        ReadLine();

        char* token = strtok(buffer, ":,");
        token = strtok(NULL, ":,");

        while (token != NULL)
        {
            int item = atoi(token);
            monkey.AddItem(item);
            token = strtok(NULL, ":,");
        }
    }

    void ParseOperation(Monkey& monkey)
    {
        ReadLine();

        Operation operation = (Operation)buffer[23];
        if(buffer[25] == 'o')
        {
            monkey.UpdateOperation(Square);
        }
        else
        {
            int argument = atoi(buffer+25);
            monkey.UpdateOperation(operation, argument);
        }
    }

    void ParseTest(Monkey& monkey)
    {
        ReadLine();
        int divider = atoi(buffer+21);

        ReadLine();
        int positiveMonkeyId = atoi(buffer+29);

        ReadLine();
        int negativeMonkeyId = atoi(buffer+30);

        monkey.UpdateThrowDecision(divider, positiveMonkeyId, negativeMonkeyId);
    }
};

class KeepAwayGame
{
private:
    vector<Monkey> monkeys;
    int levelReducer;
    int numberOfRounds;

public:
    KeepAwayGame(int levelReducer, int numberOfRounds)
        : levelReducer(levelReducer), numberOfRounds(numberOfRounds) { }

    void Initialize()
    {
        MonkeyParser parser;
        int64_t commonDenominator = 1;

        while(parser.FindNextMonkey())
        {
            Monkey monkey = parser.ReadMonkey();

            monkeys.push_back(monkey);

            if ((commonDenominator % monkey.GetDecisionDivisor()) != 0)
            {
                commonDenominator *= monkey.GetDecisionDivisor();
            }
        }

        for (int i = 0; i < monkeys.size(); i++)
        {
            Monkey& current = monkeys[i];
            current.UpdateOperationDivisor(commonDenominator);
            current.SetLevelReducer(levelReducer);
            current.UpdateThrowOperation(
                [&](int monkeyId, int64_t item)
                {
                    this->monkeys[monkeyId].AddItem(item);
                });
        }
    }

    void PlayRounds()
    {
        for (int round = 1; round <= numberOfRounds; round++)
        {
            for (int i = 0; i < monkeys.size(); i++)
            {
                monkeys[i].ExecuteTurn();
            }
        }
    }

    void PrintInspectionCounters()
    {
        for (int i = 0; i < monkeys.size(); i++)
        {
            printf("Monkey[%i]: %i\n", i, monkeys[i].GetInspectionCounter());
        }
    }
};

int main()
{
    KeepAwayGame game(1, 10000);

    game.Initialize();
    game.PlayRounds();
    game.PrintInspectionCounters();

    return 0;
}