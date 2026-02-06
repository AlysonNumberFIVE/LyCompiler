
#include "utils.h"
#include "types.h"

static t_token     *new_token(token_type type, char *value, int line, int column)
{
    t_token *new_token;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    
    new_token->type = type;
    new_token->value = strdup(value);
    if (value && new_token->value == NULL)
    {
        free(new_token);
        return NULL;
    }

    new_token->line = line;
    new_token->column = column;
    new_token->next = NULL;
    return (new_token);
}


bool push_token(t_lexer *lexer, token_type type, char *value, int line, int column)
{
    t_token *node;
    
    node = new_token(type, value, line, column);
    if (!node) 
        return false;

    if (lexer->head == NULL) 
    {
        lexer->head = node;
        lexer->tail = node;
    } 
    else 
    {
        lexer->tail->next = node;
        lexer->tail = lexer->tail->next;
    }

    return true;
}

void    print_tokens(t_token *head)
{
    t_token *traverse;

    traverse = head;
    while (traverse)
    {
        printf("token: %s\n", traverse->value);
        printf("line:  %d\n", traverse->line);
        printf("col:   %d\n", traverse->column);
        printf("=====================================\n");
        traverse = traverse->next;
    }
}

void    free_tokens(t_token *head)
{
    t_token *this;
    t_token *next;

    this = head;
    while (this)
    {
        free(this->value);
        next = this->next;
        free(this);
        this = next;
    }
}