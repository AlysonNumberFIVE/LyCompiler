#include "types.h"
#include "utils.h"

static char     *read_file_to_buffer(char* path) {

    FILE* file = fopen(path, "rb");
    if (!file) 
    {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 


    char* buffer = malloc(size + 1);
    if (!buffer) 
    {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0'; 
    fclose(file);
    return buffer;
}

static bool     check_extension(char *file, char *to_check)
{
    char *dot;

    dot = strrchr(file, '.');
    if (!dot || dot == file + strlen(file) - 1)
        return false;

    return (strcmp(dot + 1, to_check) == 0);
}

t_sourcefile    *parse_cmd(int argc, char **argv)
{
    t_sourcefile *files;

    files = NULL;
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <file1.ly> <file2.ly> ...\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) 
    {
        if (!check_extension(argv[i], "ly")) 
        {
            fprintf(stderr, "Error: File '%s' must have a .ly extension\n", argv[i]);
            exit(1);
        }
        char* content = read_file_to_buffer(argv[i]);
        if (content) {
            push_file(&files, argv[i], content, strlen(content));
        }
    }

    return (files);
}

