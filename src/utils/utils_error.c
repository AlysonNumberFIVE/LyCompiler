

#include "utils.h"
#include "types.h"

static t_error_chain   *new_error(char *line_message, char *code_block, int line, int column)
{
    t_error_chain *new_error;

    new_error = (t_error_chain *)malloc(sizeof(t_error_chain));
    if (!new_error)
        return NULL;

    new_error->line_message = strdup(line_message);
    if (!new_error->line_message)
    {
        free(new_error);
        return NULL;
    }

    // TODO: Maybe delete. 
    if (code_block != NULL)
        new_error->code_block = strdup(code_block);
    if (!new_error->code_block)
    {
        free(new_error->line_message);
        free(new_error);
        return NULL;
    }

    new_error->line = line;
    new_error->column = column;
    new_error->next = NULL;
    return (new_error);
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

    return true;
}