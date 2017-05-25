#ifndef _LIST_H_INCLUDED_
#define _LIST_H_INCLUDED_

typedef struct Node Node;

struct Node {
    void *data;
    Node *next;
};

Node *create_node(void *data, Node *next);
Node *destroy_node(Node *node);

void list_append(Node **node_ref, void *data);
int list_remove(Node **node_ref, void *data);
void list_clear(Node **node_ref);

#endif
