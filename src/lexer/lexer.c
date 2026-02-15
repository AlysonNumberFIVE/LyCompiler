
#include "types.h"
#include "utils.h"

char peek(t_lexer *lx, char *buffer, size_t length)
{
    if (lx->current_position >= length)
        return '\0';
    return buffer[lx->current_position];
}

char peek_next(t_lexer *lx, char *buffer, size_t length)
{
    if (lx->current_position + 1 >= length)
        return '\0';
    return buffer[lx->current_position + 1];
}

char advance(t_lexer *lx, char *buffer, size_t length)
{
    char c;

    c = peek(lx, buffer, length);
    lx->current_position++;
    if (c == '\n')
    {
        lx->line++;
        lx->column = 1;
    }
    else
    {
        lx->column++;
    }
    return c;
}



void skip_whitespace(t_lexer *lx, char *buffer, size_t length)
{
    while (lx->current_position < length)
    {
        char c = buffer[lx->current_position];
        if (c == ' ' || c== '\t')
            lx->column++;
        else if (c == '\n')
        {
            lx->line++;
            lx->column = 1;
        }
        else 
            break;

        lx->current_position++;
    }
}

void skip_comments(t_lexer *lx, char *buffer, size_t length)
{
    if (peek(lx, buffer, length) != '/')
        return;

    if (peek_next(lx, buffer, length) == '/')
    {
        advance(lx, buffer, length);
        advance(lx, buffer, length);
        while (peek(lx, buffer, length) != '\0' && peek(lx, buffer, length) != '\n')
            advance(lx, buffer, length);
    }

    else if (peek_next(lx, buffer, length) == '*')
    {
        advance(lx, buffer, length);
        advance(lx, buffer, length);

        while (peek(lx, buffer, length) != '\0')
        {
            if (peek(lx, buffer, length) == '*' && peek_next(lx, buffer, length) == '/')
            {
                advance(lx, buffer, length); 
                advance(lx, buffer, length); 
                return;
            }
            advance(lx, buffer, length);
        }
    }
}

void lexer(t_lexer *lx, char *content, size_t length)
{
    while (lx->current_position < length)
    {
        skip_whitespace(lx, content, length);
        if (lx->current_position >= length)
            break;


        if (peek(lx, content, length) == '/' && (peek_next(lx, content, length) == '/' || peek_next(lx, content, length) == '*')) {
            skip_comments(lx, content, length);
            continue; 
        }
        
        scan_token(lx, content, length);
    }
}

static t_lexer *init_lexer(void)
{
    t_lexer *lx = (t_lexer *)malloc(sizeof(t_lexer));
    if (!lx)
        return NULL;
    lx->head = NULL;
    lx->tail = NULL;
    lx->line = 1;
    lx->column = 1;
    lx->current_position = 0;
    lx->errors = NULL;
    lx->token = NULL;
    return (lx);
}

t_lexer *lex_files(t_sourcefile *files)
{
    t_sourcefile *traverse;
    t_lexer *lx;

    traverse = files;
    lx = init_lexer();
    if (!lx)
        return NULL;

    while (traverse)
    {
        lx->file_path = strdup(traverse->path);

        lexer(lx,  traverse->buffer, traverse->buffer_size);
        traverse = traverse->next; 

        free(lx->file_path);
    }
    lx->tail->next = NULL;
    return lx;
}

