#include <iostream>

struct Node {
    int data;
    Node *next;
};

Node *append(Node *head, int data) {
    Node *newNode;
    newNode->data = data;
    newNode->next = nullptr;
    if (head == nullptr) {
        return newNode;
    }
    Node *tmp = head;
    while (tmp->next != nullptr) {
        tmp = tmp->next;
    }
    tmp->next = newNode;
    return head;
}

Node *reverse(Node *head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    Node *prev = nullptr, *curr = head, *next;
    while (curr != nullptr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}
