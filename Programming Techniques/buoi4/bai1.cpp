#include <iostream>
using namespace std;
struct Node
{
    int data;
    Node *next;

    Node(int data)
    {
        this->data = data;
        next = NULL;
    }
};

// push a new element to the beginning of the list
Node *prepend(Node *head, int data)
{
    Node *res = (Node *)calloc(1, sizeof(Node));
    res->data = data;
    res->next = head;

    return res;
}

// print the list content on a line
void print(Node *head)
{
    Node *tmp = head;
    while (tmp != nullptr)
    {
        cout << tmp->data << " ";
        tmp = tmp->next;
    }
    cout << endl;
}

// return the new head of the reversed list
Node *reverse(Node *head)
{
    if (head == nullptr && head->next == nullptr)
    {
        return head;
    }

    Node *tmpHead = head->next, *tmpTail = head;
    head->next = nullptr;
    while (tmpHead != nullptr)
    {
        Node *tmp = tmpHead->next;
        tmpHead->next = tmpTail;
        tmpTail = tmpHead;
        tmpHead = tmp;
    }

    return tmpTail;
}

int main()
{
    int n, u;
    cin >> n;
    Node *head = NULL;
    for (int i = 0; i < n; ++i)
    {
        cin >> u;
        head = prepend(head, u);
    }

    cout << "Original list: ";
    print(head);

    head = reverse(head);

    cout << "Reversed list: ";
    print(head);

    return 0;
}
