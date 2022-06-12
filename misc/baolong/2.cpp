#include "stdio.h"
#include "stdlib.h"

typedef struct Node
{
    int value;
    struct Node *next;
} ListNode;

ListNode *makenewnode(int x)
{
    ListNode nut = (ListNode)malloc(sizeof(ListNode));
    if (nut == NULL)
        exit(1);
    nut->value = x;
    nut->next = NULL;
    return nut;
}

void printList(ListNode *list)
{
    ListNode *tg = list;
    printf("\n");
    while (tg != NULL)
    {
        printf("%d ", tg->value);
        tg = tg->next;
    }
}

ListNode *findLast(ListNode *list)
{
    ListNode *tg = list;
    while (tg != NULL)
    {
        if (tg->next == NULL)
            return tg;
        tg = tg->next;
    }
    return NULL;
}

ListNode *insertAfter(ListNode *h, ListNode *p, int x)
{
    if (p == NULL)
        return h;
    ListNode *q = makenewnode(x);
    if (h == NULL)
        return q;
    q->next = p->next;
    p->next = q;
    return h;
}

ListNode *insertAtHead(ListNode *list, int x)
{
    if (list == NULL)
    {
        return makenewnode(x);
    }
    ListNode *new = makenewnode(x);
    new->next = list;

    return new;
}

ListNode *insertLast(ListNode *h, int x)
{
    ListNode *q = makenewnode(x);
    if (h == NULL)
        return q;

    ListNode *last = findLast(h);
    last->next = q;

    return h;
}

ListNode *locate(ListNode *h, int x)
{
    ListNode *p = h;
    while (p != NULL)
    {
        if (p->value == x)
            return p;
        p = p->next;
    }
    return NULL;
}

ListNode *prev(ListNode *h, ListNode *p)
{
    ListNode *q = h;
    while (q != NULL)
    {
        if (q->next == p)
            return q;
        q = q->next;
    }

    return NULL;
}

ListNode *insertAt(ListNode *h, ListNode *p, int x)
{
    if (p == NULL)
        return h;
    ListNode *pp = prev(h, p);
    ListNode *q = makenewnode(x);
    if (pp == NULL)
    {
        if (h == NULL)
            return q;
        q->next = h;
        return q;
    }
    q->next = p;
    pp->next = q;
    return h;
}

ListNode *removeNode(ListNode *h, ListNode *p)
{
    if (h == NULL || p == NULL)
        return h;
    if (h == p)
    {
        h = h->next;
        free(p);
        return h;
    }
    h->next = removeNode(h->next, p);
    return h;
}

ListNode *removeNegative(ListNode *h)
{
    if (h == NULL)
        return h;
    ListNode *p = h;

    if (h->value < 0)
    {
        h = h->next;
        free(p);
        h = removeNegative(h);
    }
    else
    {
        h->next = removeNegative(h->next);
    }

    return h;
}

int sum(ListNode *h)
{
    ListNode *p = h;
    int S = 0;
    while (p != NULL)
    {
        S = S + p->value;
        p = p->next;
    }
    return S;
}

int sumRecursive(ListNode *h)
{
    if (h == NULL)
        return 0;
    return h->value + sumRecursive(h->next);
}

int main()
{
    ListNode *head, *list10k, *new, *tg, *lastnode;
    int i;
    head = makenewnode(5);
    head->next = makenewnode(15);
    for (i = 1; i < 10000; i++)
    {
        if (i == 1)
        {
            list10k = makenewnode(i);
            tg = list10k;
        }
        else
        {
            new = makenewnode(i);
            tg->next = new;
            tg = new;
        }
    }

    lastnode = findLast(list10k);
    lastnode->next = makenewnode(10000);

    list10k = insertAfter(list10k, findLast(list10k), 10001);

    list10k = insertAtHead(list10k, 0);
    list10k = insertLast(list10k, 10002);

    list10k = insertAt(list10k, list10k, -1);

    list10k = removeNode(list10k, findLast(list10k));

    printList(list10k);

    printf("\n%d\n", prev(list10k, locate(list10k, 7682))->value);

    printf("\n%d\n", sum(findLast(list10k)));

    ListNode *listnega;
    for (i = 1; i < 10; i++)
    {
        if (i == 1)
        {
            listnega = makenewnode(-i);
            tg = listnega;
        }
        else
        {
            new = makenewnode(-i);
            tg->next = new;
            tg = new;
        }
    }
    listnega = insertAtHead(listnega, 10);
    printList(listnega);
    listnega = removeNegative(listnega);
    printList(listnega);

    return 1;
}
