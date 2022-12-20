#include <cstdio>
#include <cinttypes>
#include <vector>

using namespace std;

class ListNode
{
private:
    int64_t value;
    ListNode* next;
    ListNode* prev;
    
public:
    ListNode(int64_t value) : value(value), next(NULL), prev(NULL) { }
    
    void SetAsHead()
    {
        next = prev = this;
    }
    
    void PlaceAfter(ListNode* predecessor)
    {
        next = predecessor->next;
        next->prev = this;
        
        prev = predecessor;
        predecessor->next = this;
    }
    
    ListNode* GoForward()
    {
        return next;
    }
    
    ListNode* GoBackward()
    {
        return prev;
    }
    
    int64_t GetValue()
    {
        return value;
    }
    
    void Swap(int64_t listSize)
    {
        int64_t steps = value;
        
        if (steps < 0)
        {
            int64_t m = (-steps) / (listSize-1);
            steps += m*(listSize-1);
            
            if(steps < -(listSize-1)/2)
            {
                steps += (listSize-1);
            }
        }

        if (steps > 0)
        {
            int64_t m = steps / (listSize-1);
            steps -= m*(listSize-1);
            
            if(steps > (listSize-1)/2)
            {
                steps -= (listSize-1);
            }
        }

        if (steps < 0)
        {
            for(int64_t i = 0; i < -steps; i++)
            {
                SwapBackward();
            }
        }
        
        if (steps > 0)
        {
            for(int64_t i = 0; i < steps; i++)
            {
                SwapForward();
            }
        }
    }
    
private:
    void SwapForward()
    {
        ListNode* node = next;
        
        prev->next = node;
        node->next->prev = this;
        
        next = node->next;
        node->prev = prev;
        node->next = this;
        prev = node;
    }
    
    void SwapBackward()
    {
        ListNode* node = prev;
        
        node->prev->next = this;
        next->prev = node;
        
        prev = node->prev;
        node->next = next;
        node->prev = this;
        next = node;
    }
};

int main()
{
    vector<int64_t> input;
    vector<ListNode> nodes;
    int n;
    
    while(scanf("%i", &n) != EOF)
    {
        int64_t value = (int64_t)n*811589153LL;
        input.push_back(value);
        nodes.push_back(ListNode(value));
    }
    
    nodes[0].SetAsHead();
    for(int i = 1; i < input.size(); i++)
    {
        nodes[i].PlaceAfter(&nodes[i-1]);
    }
    
    for(int i = 0; i < 10; i++)
    {
        for(ListNode& node: nodes)
        {
            node.Swap(input.size());
        }
    }
    
    ListNode* node = &nodes.front();
    int64_t sum = 0;
    while(node->GetValue() != 0)
    {
        node = node->GoForward();
    }
    
    for(int i = 1; i <= 3000; i++)
    {
        node = node->GoForward();
        if(i % 1000 == 0)
        {
            sum += node->GetValue();
        }
    }
    
    printf("%lld\n", sum);

    return 0;
}
