

#include "utils.h"
#include "types.h"

static t_error_chain   *new_error(char *line_message, char *code_block, int line, int column)
{
    t_error_chain *e;

    e = (t_error_chain *)malloc(sizeof(t_error_chain));
    if (!e)
        return NULL;

    e->line_message = strdup(line_message);
    if (!e->line_message)
    {
        free(e);
        return NULL;
    }

    e->code_block = NULL;
    if (code_block != NULL)
    {
        e->code_block = strdup(code_block);
        if (!e->code_block)
        {
            free(e->line_message);
            free(e);
            return NULL;
        }
    }

    e->line = line;
    e->column = column;
    e->chain_index = 0;
    e->next = NULL;
    return e;
}

t_error_control *init_error_control(void)
{
    t_error_control *ec;

    ec = (t_error_control *)malloc(sizeof(t_error_control));
    if (!ec)
        return NULL;
    ec->head  = NULL;
    ec->tail  = NULL;
    ec->count = 0;
    return ec;
}

bool    push_error(t_error_control *error, char *line_message, char *code_block, int line, int column)
{
    t_error_chain *node;
    
    node = new_error(line_message, code_block, line, column);
    if (!node) 
        return false;

    if (error->head == NULL) 
    {
        error->head = node;
        error->tail = node;
    } 
    else 
    {
        error->tail->next = node;
        error->tail = error->tail->next;
    }

    error->count++;
    return true;
}

void    print_errors(t_error_control *errors)
{
    t_error_chain   *node;
    int             i;

    if (!errors || errors->count == 0)
        return;
    node = errors->head;
    i = 1;
    fprintf(stderr, "\n");
    while (node)
    {
        if (node->line > 0)
            fprintf(stderr, "error [%d/%d] (line %d, col %d): %s\n",
                i, errors->count, node->line, node->column, node->line_message);
        else
            fprintf(stderr, "error [%d/%d]: %s\n",
                i, errors->count, node->line_message);
        node = node->next;
        i++;
    }
    fprintf(stderr, "\nparse failed: %d error(s).\n", errors->count);
}
