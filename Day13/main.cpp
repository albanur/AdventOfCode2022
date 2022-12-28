#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum ItemType
{
    ValueType,
    ListType
};

class Item
{
public:
    virtual ~Item() { }
    virtual ItemType GetType() const = 0;
};

class ListItem: public Item
{
private:
    vector<Item*> items;

public:
    ListItem() { }
    ~ListItem()
    {
        for(int i = 0; i < items.size(); i++)
        {
            delete items[i];
        }
        items.clear();
    }

    ItemType GetType() const { return ListType; }
    
    void AddItem(Item* item)
    {
        items.push_back(item);
    }

    int GetSize() const 
    {
        return items.size();
    }

    const Item* GetItem(int i) const 
    {
        return items[i];
    }
};

class ValueItem : public Item
{
private:
    int value;

public:
    ValueItem() : value(0) { }
    ValueItem(int value) : value(value) { }

    ItemType GetType() const { return ValueType; }
    int GetValue() const { return value; }

    ListItem* ToList() const
    {
        ListItem* list = new ListItem();
        ValueItem* item = new ValueItem(value);
        list->AddItem(item);
        return list;
    }
};

ListItem* Parse(const char* buffer)
{
    stack<ListItem*> items;
    ListItem* root = NULL;

    for(int i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++)
    {
        if(buffer[i] == '[')
        {
            ListItem* list = new ListItem();
            if (root == NULL)
            {
                root = list;
                items.push(list);
            }
            else
            {
                items.top()->AddItem(list);
                items.push(list);
            }
        }
        else if (buffer[i] == ']')
        {
            items.pop();
        }
        else if (buffer[i] == ',')
        {
            continue;
        }
        else if (isdigit(buffer[i]))
        {
            int value = atoi(buffer+i);
            while(isdigit(buffer[i+1])) { i++; }
            ValueItem* item = new ValueItem(value);
            items.top()->AddItem(item);
        }
    }

    return root;
}

extern int CompareValues(const ValueItem* item1, const ValueItem* item2);
extern int CompareLists(const ListItem* list1, const ListItem* list2);
extern int Compare(const Item* item1, const Item* item2);

bool CompareLess (const ListItem* list1, const ListItem* list2)
{
    return CompareLists(list1, list2) < 0;
}

int CompareValues(const ValueItem* item1, const ValueItem* item2)
{
    if(item1->GetValue() == item2->GetValue())
    {
        return 0;
    }

    return item1->GetValue() < item2->GetValue() ? -1 :  1;
}

int CompareLists(const ListItem* list1, const ListItem* list2)
{
    int size1 = list1->GetSize();
    int size2 = list2->GetSize();
    int size = min(size1, size2);

    for(int i = 0; i < size; i++)
    {
        int result = Compare(list1->GetItem(i), list2->GetItem(i));
        if (result != 0)
        {
            return result;
        }
    }

    if (size1 == size2)
    {
        return 0;
    }

    return (size1 < size2) ? -1 : 1;
}

int Compare(const Item* item1, const Item* item2)
{
    ItemType type1 = item1->GetType();
    ItemType type2 = item2->GetType();

    if(type1 == ValueType && type2 == ValueType)
    {
        return CompareValues((ValueItem*)item1, (ValueItem*)item2);
    }

    if(type1 == ValueType)
    {
        ListItem* list = ((ValueItem*)item1)->ToList();
        int result = CompareLists(list, (ListItem*)item2);
        delete list;
        return result;
    }

    if(type2 == ValueType)
    {
        ListItem* list = ((ValueItem*)item2)->ToList();
        int result =  CompareLists((ListItem*)item1, list);
        delete list;
        return result;
    }

    return CompareLists((ListItem*)item1, (ListItem*)item2);
}

void Part1()
{
    char buffer[1024];
    ListItem* list1;
    ListItem* list2;
    int counter = 1;
    int sum = 0;

    while(NULL != fgets(buffer, 1024, stdin))
    {
        while(buffer[0] != '[')
        {
            fgets(buffer, 1024, stdin);
        }
        list1 = Parse(buffer);
        fgets(buffer, 1024, stdin);
        list2 = Parse(buffer);

        if(CompareLists(list1, list2) < 0) { sum += counter; }

        delete list1;
        delete list2;
        counter++;
    }

    printf("%i\n", sum);
}

void Part2()
{
    char buffer[1024];
    vector<ListItem*> signals;
    ListItem* signal2;
    ListItem* signal6;

    while(NULL != fgets(buffer, 1024, stdin))
    {
        if(buffer[0] != '[') { continue; }
        ListItem* list = Parse(buffer);
        signals.push_back(list);
    }

    signal2 = Parse("[[2]]\n");
    signal6 = Parse("[[6]]\n");

    signals.push_back(signal2);
    signals.push_back(signal6);

    sort(signals.begin(), signals.end(), CompareLess);

    int idx2 = 0, idx6 = 0;
    for(int i = 0; i<signals.size(); i++)
    {
        if(signals[i] == signal2) { idx2 = i+1; }
        if(signals[i] == signal6) { idx6 = i+1; }
    }

    printf("%i\n", idx2 * idx6);
}

int main()
{
    //Part1();
    Part2();

    return 0;
}