#include <stdio.h>
#include <stdlib.h>

extern char** environ;

void DisplayAllEnvironment()
{
    for (int i = 0; environ[i] != NULL; ++i) {
        printf("-> %s\n", environ[i]);
    }
}

int main()
{
    char* cwd = getenv("PWD");
    printf("cwd: %s", cwd);
    return 0;
}