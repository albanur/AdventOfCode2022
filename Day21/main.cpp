#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Monkey
{
protected:
    string name;
    
public:
    Monkey(const char* name) : name(name) { }
    virtual bool HasValue() = 0;
    virtual int64_t GetValue() = 0;
    virtual bool RequestValue(int64_t value) = 0;
    virtual bool ResolveNames(const map<string, Monkey*>& mapping) = 0;
};

class ValueMonkey : public Monkey
{
private:
    int64_t value;
    
public:
    ValueMonkey(const char* name, int64_t value) : Monkey(name), value(value) { }
    
    bool HasValue() { return true; }
    int64_t GetValue() { return value; }
    bool RequestValue(int64_t value) { return false; }
    bool ResolveNames(const map<string, Monkey*>& mapping) { return true; }
};

class HumanMonkey : public Monkey
{
private:
    bool hasValue;
    int64_t value;
    
public:
    HumanMonkey(const char* name) : Monkey(name), value(0LL), hasValue(false) { }
    
    bool HasValue() { return hasValue; }
    int64_t GetValue() { return value; }
    bool RequestValue(int64_t value) { hasValue = true; this->value = value; return true; }
    bool ResolveNames(const map<string, Monkey*>& mapping) { return true; }
};

class OperationMonkey : public Monkey
{
protected:
    string firstArgumentName;
    string secondArgumentName;
    Monkey* firstArgumentMonkey;
    Monkey* secondArgumentMonkey;
    
public:
    OperationMonkey(const char* name, const char* firstName, const char* secondName) : Monkey(name)
    {
        firstArgumentName = firstName;
        secondArgumentName = secondName;
        firstArgumentMonkey = secondArgumentMonkey = NULL;
    }
    
    bool HasValue()
    {
        return CanCalculate();
    }
    
    int64_t GetValue()
    {
        return DoOperation();
    }
    
    bool RequestValue(int64_t value)
    {
        if (CanCalculate())
        {
            return false;
        }
        
        return PassRequest(value);
    }
    
    bool ResolveNames(const map<string, Monkey*>& mapping)
    {
        if (mapping.count(firstArgumentName) > 0)
        {
            firstArgumentMonkey = mapping.at(firstArgumentName);
        }
        
        if (mapping.count(secondArgumentName) > 0)
        {
            secondArgumentMonkey = mapping.at(secondArgumentName);
        }
        
        return firstArgumentMonkey != NULL && secondArgumentMonkey != NULL;
    };
    
protected:
    bool CanCalculate()
    {
        return firstArgumentMonkey != NULL  && firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && secondArgumentMonkey->HasValue();
    }

    virtual int64_t DoOperation() = 0;
    
    virtual bool PassRequest(int64_t value) = 0;
};

class AdditionMonkey : public OperationMonkey
{
public:
    AdditionMonkey(const char* name, const char* firstName, const char* secondName)
        : OperationMonkey(name, firstName, secondName) { }
    
protected:
    int64_t DoOperation()
    {
        return firstArgumentMonkey->GetValue() + secondArgumentMonkey->GetValue();
    }
    
    bool PassRequest(int64_t value)
    {
        if (firstArgumentMonkey != NULL  && !firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && secondArgumentMonkey->HasValue())
        {
            int64_t a2 = secondArgumentMonkey->GetValue();
            return firstArgumentMonkey->RequestValue(value - a2);
        }
        if (firstArgumentMonkey != NULL  && firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && !secondArgumentMonkey->HasValue())
        {
            int64_t a1 = firstArgumentMonkey->GetValue();
            return secondArgumentMonkey->RequestValue(value - a1);
        }
        return false;
    }
};

class SubstractionMonkey : public OperationMonkey
{
public:
    SubstractionMonkey(const char* name, const char* firstName, const char* secondName)
        : OperationMonkey(name, firstName, secondName) { }
    
protected:
    int64_t DoOperation()
    {
        return firstArgumentMonkey->GetValue() - secondArgumentMonkey->GetValue();
    }
    
    bool PassRequest(int64_t value)
    {
        if (firstArgumentMonkey != NULL  && !firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && secondArgumentMonkey->HasValue())
        {
            int64_t a2 = secondArgumentMonkey->GetValue();
            return firstArgumentMonkey->RequestValue(value + a2);
        }
        if (firstArgumentMonkey != NULL  && firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && !secondArgumentMonkey->HasValue())
        {
            int64_t a1 = firstArgumentMonkey->GetValue();
            return secondArgumentMonkey->RequestValue(a1 - value);
        }
        return false;
    }
};

class MultiplicationMonkey : public OperationMonkey
{
public:
    MultiplicationMonkey(const char* name, const char* firstName, const char* secondName)
        : OperationMonkey(name, firstName, secondName) { }
    
protected:
    int64_t DoOperation()
    {
        return firstArgumentMonkey->GetValue() * secondArgumentMonkey->GetValue();
    }
    
    bool PassRequest(int64_t value)
    {
        if (firstArgumentMonkey != NULL  && !firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && secondArgumentMonkey->HasValue())
        {
            int64_t a2 = secondArgumentMonkey->GetValue();
            return firstArgumentMonkey->RequestValue(value/a2);
        }
        if (firstArgumentMonkey != NULL  && firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && !secondArgumentMonkey->HasValue())
        {
            int64_t a1 = firstArgumentMonkey->GetValue();
            return secondArgumentMonkey->RequestValue(value/a1);
        }
        return false;
    }
};

class DivisionMonkey : public OperationMonkey
{
public:
    DivisionMonkey(const char* name, const char* firstName, const char* secondName)
        : OperationMonkey(name, firstName, secondName) { }
    
protected:
    int64_t DoOperation()
    {
        return firstArgumentMonkey->GetValue() / secondArgumentMonkey->GetValue();
    }
    
    bool PassRequest(int64_t value)
    {
        if (firstArgumentMonkey != NULL  && !firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && secondArgumentMonkey->HasValue())
        {
            int64_t a2 = secondArgumentMonkey->GetValue();
            return firstArgumentMonkey->RequestValue(value*a2);
        }
        if (firstArgumentMonkey != NULL  && firstArgumentMonkey->HasValue()
            && secondArgumentMonkey != NULL  && !secondArgumentMonkey->HasValue())
        {
            int64_t a1 = firstArgumentMonkey->GetValue();
            return secondArgumentMonkey->RequestValue(a1/value);
        }
        return false;
    }
};

int main()
{
    char buffer[128];
    char nameM[8];
    char nameA1[8];
    char nameA2[8];
    char o;
    int value;
    map<string, Monkey*> monkeys;
    HumanMonkey* human = NULL;
    
    while(NULL != fgets(buffer, 128, stdin))
    {
        if(isdigit(buffer[6]))
        {
            //dbpl: 5
            sscanf(buffer, "%4s: %i\n", nameM, &value);
            
            if (string(nameM) == string("humn"))
            {
                human =  new HumanMonkey(nameM);
                monkeys[nameM] = human;
            }
            else
            {
                monkeys[nameM] = new ValueMonkey(nameM, value);
            }
        }
        else
        {
            //root: pppw + sjmn
            sscanf(buffer, "%4s: %4s %c %4s\n", nameM, nameA1, &o, nameA2);
            Monkey* monkey = NULL;
            
            if (string(nameM) == string("root"))
            {
                monkey = new SubstractionMonkey(nameM, nameA1, nameA2);
            }
            else
            {
                switch(o)
                {
                    case '+':
                        monkey = new AdditionMonkey(nameM, nameA1, nameA2);
                        break;
                    case '-':
                        monkey = new SubstractionMonkey(nameM, nameA1, nameA2);
                        break;
                    case '*':
                        monkey = new MultiplicationMonkey(nameM, nameA1, nameA2);
                        break;
                    case '/':
                        monkey = new DivisionMonkey(nameM, nameA1, nameA2);
                        break;
                }
            }
            
            if (monkey != NULL)
            {
                monkeys[nameM] = monkey;
            }
            else
            {
                printf("FAIL\n");
                return -1;
            }
        }
    }
    
    for(auto& kv : monkeys)
    {
        kv.second->ResolveNames(monkeys);
    }
    
    //printf("%lld\n", monkeys["root"]->GetValue());
    
    monkeys["root"]->RequestValue(0LL);
    printf("%lld\n", human->GetValue());
    
    for(auto& kv : monkeys)
    {
        delete kv.second;
        kv.second = NULL;
    }

    return 0;
}
