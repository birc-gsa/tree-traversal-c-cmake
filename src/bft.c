#include "gsa.h"
#include <assert.h>
#include <stdbool.h>

// linked list queue
struct link
{
    tree t;
    struct link *prev, *next;
};

static void insert_link(tree t, struct link *prev, struct link *next)
{
    struct link *link = malloc(sizeof *link);
    assert(link);
    *link = (struct link){.t = t, .prev = prev, .next = next};
    prev->next = link;
    next->prev = link;
}

static void delete_link(struct link *link)
{
    link->prev->next = link->next;
    link->next->prev = link->prev;
    free(link);
}

// A queue is just a header (dummy) link
typedef struct link *queue;

static inline bool empty(queue q)
{
    return q->next == q;
}

static inline void init_queue(queue q)
{
    q->next = q->prev = q;
}

static inline void enqueue(queue q, tree t)
{
    if (t)
    {
        insert_link(t, q, q->next);
    }
}

static inline tree dequeue(queue q)
{
    tree t = q->prev->t;
    delete_link(q->prev);
    return t;
}

dynarr bf_order(tree t)
{
    dynarr a = new_dynarr();

    struct link q;
    init_queue(&q);
    enqueue(&q, t);

    while (!empty(&q))
    {
        t = dequeue(&q);
        append(&a, t->val);
        enqueue(&q, t->left);
        enqueue(&q, t->right);
    }

    return a;
}
