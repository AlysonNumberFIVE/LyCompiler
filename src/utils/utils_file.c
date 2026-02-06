#include "utils.h"
#include "types.h"

static t_sourcefile     *new_file(char *path, char *buffer, size_t buffer_size)
{
    t_sourcefile    *new_file;

    new_file = (t_sourcefile *)malloc(sizeof(t_sourcefile));
    if (new_file == NULL)
        return NULL;

    new_file->path = strdup(path);
    if (!new_file->path)
    {
        free(new_file);
        return NULL;
    }
    new_file->buffer = strdup(buffer);
    if (!new_file->buffer)
    {
        free(new_file->path);
        free(new_file);
        return NULL;
    }
    new_file->buffer_size = buffer_size;
    new_file->next = NULL; 
    return (new_file);
}

static bool    add_file(t_sourcefile **head, char *path, char *buffer, size_t buffer_size)
{
    t_sourcefile *traverse;

    traverse = *head;
    while (traverse->next)
        traverse = traverse->next;

    traverse->next = new_file(path, buffer, buffer_size);
    if (traverse->next == NULL)
        return false;

    return true;
}

bool    push_file(t_sourcefile **head, char *path, char *buffer, size_t buffer_size)
{
    if (*head == NULL)
    {
        *head = new_file(path, buffer, buffer_size); 
        return (*head != NULL);
    }

    return add_file(head, path, buffer, buffer_size);
}

void    print_files(t_sourcefile *files)
{
    t_sourcefile *traverse;

    traverse = files;
    while (traverse)
    {
        printf("filename: %s\n", traverse->path);
        printf("content:\n%s\n", traverse->buffer);
        printf("===================================\n");
        traverse = traverse->next;
    }
}

void    free_files(t_sourcefile *files)
{
    t_sourcefile *this;
    t_sourcefile *next;
    
    this = files;
    while (this)
    {
        free(this->path);
        free(this->buffer);
        next = this->next;
        free(this);
        this = next;
    }
}