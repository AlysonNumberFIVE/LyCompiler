#include "../inc/compiler.h"




int test_push_token(void)
{
    t_token *head;
    t_lexer *lexer;

    lexer = (t_lexer *)malloc(sizeof(t_lexer));
    lexer->head = NULL;
    lexer->tail = NULL;
    head = NULL;
    if (push_token(lexer, TOKEN_OP_EQ, "=", 10, 14) == false)
        return -1;

     if (push_token(lexer, TOKEN_KW_ELSE, "else", 284, 10) == false)
        return -1;

    push_token(lexer, TOKEN_TYPE_CHAR, "char", 211, 21);
    

    print_tokens(lexer->head);

    free_tokens(lexer->head);

    return 0;
}   

int main(void)
{
    test_push_token();
    return (0);
}