#include <cstdio>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <set>

using namespace std;

struct Node
{
    Node* left;
    Node* right;
    Node* parent;
    vector<string> children;
    string address;
    int cost = 0;

    Node(Node* parent = NULL)
        : left(NULL), right(NULL), parent(parent) {}

    Node(Node* parent, const string& address)
        : left(NULL), right(NULL), parent(parent), address(string(address)) {}

    void UpdateCost()
    {
        cost = (left != NULL && right != NULL) || (!children.empty()) ? 1 : 0;
    }
};

typedef pair<Node*, int> PriorityNode;

bool operator < (const PriorityNode& node1, const PriorityNode& node2)
{
    return (node1.second > node2.second) ||
        (node1.second == node2.second && node1.first->address > node2.first->address);
}

class Tree
{
private:
    Node* root;

public:
    Tree()
    {
        root = new Node();
    }

    ~Tree()
    {
        stack<Node*> toRemove;
        toRemove.push(root);

        while(!toRemove.empty())
        {
            Node* node = toRemove.top();
            toRemove.pop();
            
            if(node->left != NULL)
            {
                toRemove.push(node->left);
            }
            if(node->right != NULL)
            {
                toRemove.push(node->right);
            }

            delete node;
        }

        root = NULL;
    }

    void AddPathToChild(string child, const char* path)
    {
        Node* node = root;
        string address;

        while(*path != '\0')
        {
            address += *path;

            if(*path == 'L')
            {
                node = GetOrCreateLeft(node, address);
            }
            else if(*path == 'R')
            {
                node = GetOrCreateRight(node, address);
            }

            path++;
        }

        node->children.push_back(child);
        node->UpdateCost();
    }

    void SearchAndCalculateScore()
    {
        priority_queue<PriorityNode> scores;
        set<Node*> visited;
        //scores.push(PriorityNode(root, 0));
        bool foundChild = true;
        int cost = 0;
        Node* start = root;

        while (foundChild)
        {
            foundChild = false;

            while(!scores.empty())
            {
                scores.pop();
            }
            visited.clear();

            scores.push(PriorityNode(start, 0));

            while(!scores.empty())
            {
                PriorityNode current = scores.top();
                scores.pop();

                if (visited.count(current.first))
                {
                    continue;
                }
                else
                {
                    visited.insert(current.first);
                }

                if (current.first->children.size() > 0)
                {
                    if(cost == 0)
                    {
                        for(const auto& child : current.first->children)
                        {
                            printf("First visited %s at %s\n", child.c_str(), current.first->address.c_str());
                        }
                    }

                    foundChild = true;
                    start = current.first;
                    current.first->children.clear();
                    cost += current.second;
                    break;
                }

                if(current.first->parent != NULL && !visited.count(current.first->parent))
                {
                    scores.push(PriorityNode(current.first->parent, current.second + current.first->parent->cost));
                }
                if(current.first->right != NULL && !visited.count(current.first->right))
                {
                    scores.push(PriorityNode(current.first->right, current.second + current.first->right->cost));
                }
                if(current.first->left != NULL && !visited.count(current.first->left))
                {
                    scores.push(PriorityNode(current.first->left, current.second + current.first->left->cost));
                }
            }
        }

        printf("Total cost: %d\n", cost);
    }

private:
    Node* GetOrCreateLeft(Node* current, const string& address)
    {
        if(current->left == NULL)
        {
            current->left = new Node(current, address);
            current->UpdateCost();
        }

        return current->left;
    }

    Node* GetOrCreateRight(Node* current, const string& address)
    {
        if(current->right == NULL)
        {
            current->right = new Node(current, address);
            current->UpdateCost();
        }

        return current->right;
    }
};

int main()
{
    char buffer[1024];
    char* pointer;
    char* name;
    char* path;
    char* delimiter;
    Tree tree;

    while(fgets(buffer, 1024, stdin) != NULL)
    {
        name = buffer;
        delimiter = strchr(buffer, '-');
        *(delimiter-1) = '\0';
        path = delimiter+2;
        delimiter = strchr(delimiter, '\n');
        *(delimiter) = '\0';

        tree.AddPathToChild(string(name), path);
        //printf("%s:%s\n", name.c_str(), path.c_str());
    }

    tree.SearchAndCalculateScore();

    return 0;
}