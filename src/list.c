#include <stdlib.h>
#include "list.h"

Node *create_node(void *data, Node *next)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->data = data;
    node->next = next;

    return node;
}


Node *destroy_node(Node *node)
{
    Node *next;

    // save next reference
    next = node->next;

    // delete node
    free(node);

    return next;
}

void list_append(Node **node_ref, void *data)
{
    // traverse the list
    while (*node_ref != NULL) node_ref = &(*node_ref)->next;

    // insert new node at the end
    *node_ref = create_node(data, NULL);
}

int list_remove(Node **node_ref, void *data)
{
    while (*node_ref != NULL && (*node_ref)->data != data) node_ref = &(*node_ref)->next;

    if (*node_ref == NULL) return 0;

    *node_ref = destroy_node(*node_ref);

    return 1;
}

void list_clear(Node **node_ref)
{
    while (*node_ref != NULL) *node_ref = destroy_node(*node_ref);
}
