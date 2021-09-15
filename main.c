#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM " \t\n"

/*int process()
{

}*/

char **split(char *cmd)
{
    int max_len = 50, pos = 0;
    char *piece = strtok(cmd, DELIM);
    char **tokens = malloc(sizeof(char*) * max_len);

    if (!tokens) {
        perror("Error: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (piece != NULL)
    {
        tokens[pos++] = piece;
        piece = strtok(NULL, DELIM);

        if (pos >= max_len)
        {
            max_len += max_len;
            tokens = realloc(tokens, sizeof(char*) * max_len);

            if (!tokens) {
                perror("Error: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[pos] = NULL;
    return tokens;
}

char *read_cmd()
{
    char *cmd = NULL;
    size_t len = 0;

    if (getline(&cmd, &len, stdin) == -1)
    {
        if (feof(stdin))              // No se muy bien qué hace esto,
            exit(EXIT_SUCCESS);       // pero si nos preguntan estaría
        else {                        // bien saberlo.    -R
            perror("Error read_cmd");
            exit(EXIT_FAILURE);
        }
    }

    return cmd;
}

void loop()
{
    char *cmd;
    char **args;
    int pos=50;

    do
    {
        printf("$ ");
        cmd = read_cmd();
        args = split(cmd);
        //process(args);


        for (int i = 0; i != pos; ++i)
            printf("La palabra %d es '%s'.\n", i, args[i]);

    } while (1);
}

int main()
{
    loop();

    return EXIT_SUCCESS;
}