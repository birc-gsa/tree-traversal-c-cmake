#include "gsa.h"
#include <assert.h>

enum op
{
    PROCESS,
    EMIT
};

struct stack_frame
{
    enum op op;
    tree t;
};

static inline struct stack_frame process(tree t)
{
    return (struct stack_frame){.op = PROCESS, .t = t};
}

static inline struct stack_frame emit(tree t)
{
    return (struct stack_frame){.op = EMIT, .t = t};
}

// A linked list stack
typedef struct stack *stack;
struct stack
{
    struct stack_frame frame;
    stack next;
};

static void push(stack *s, struct stack_frame frame)
{
    if (frame.t) // only push if we have a tree...
    {
        stack top = malloc(sizeof *top);
        assert(top);
        *top = (struct stack){.frame = frame, .next = *s};
        *s = top;
    }
}

static struct stack_frame pop(stack *s)
{
    stack top = *s;
    *s = top->next;
    struct stack_frame frame = top->frame;
    free(top);
    return frame;
}

dynarr in_order(tree t)
{
    dynarr a = new_dynarr();
    stack s = NULL;
    push(&s, process(t));
    while (s)
    {
        struct stack_frame frame = pop(&s);
        t = frame.t;
        switch (frame.op)
        {
        case PROCESS:
            push(&s, process(t->right));
            push(&s, emit(t));
            push(&s, process(t->left));
            break;

        case EMIT:
            append(&a, t->val);
            break;
        }
    }
    return a;
}
