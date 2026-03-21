
#include "../analyser/analyser.h"

static void free_datatype(t_datatype *type)
{
    if (type == NULL)
        return;
    free(type->type_name);
    free(type);
}

static t_symbol *new_symbol(char *name, t_datatype *type,
    int scope_level, int memory_offset, bool initialized)
{
    t_symbol *symbol;

    symbol = (t_symbol *)malloc(sizeof(t_symbol));
    if (symbol == NULL)
        return NULL;
    symbol->name = strdup(name);
    if (name && symbol->name == NULL)
    {
        free(symbol);
        return NULL;
    }
    symbol->type = type;
    symbol->scope_level = scope_level;
    symbol->memory_offset = memory_offset;
    symbol->initialized = initialized;
    symbol->next = NULL;
    return symbol;
}

bool push_symtab(t_symbol **head, char *name, t_datatype *type,
    int scope_level, int memory_offset, bool initialized)
{
    t_symbol *node;
    t_symbol *tail;

    if (head == NULL)
        return false;
    node = new_symbol(name, type, scope_level, memory_offset, initialized);
    if (node == NULL)
        return false;
    if (*head == NULL)
    {
        *head = node;
        return true;
    }
    tail = *head;
    while (tail->next)
        tail = tail->next;
    tail->next = node;
    return true;
}

void free_symtab(t_symbol *head)
{
    t_symbol *current;
    t_symbol *next;

    current = head;
    while (current)
    {
        free(current->name);
        free_datatype(current->type);
        next = current->next;
        free(current);
        current = next;
    }
}