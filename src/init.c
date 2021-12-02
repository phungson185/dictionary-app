#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <btree.h>
#define MAX 1000

int main()
{
    btinit();
    BTA *dict;
    BTA *note;
    FILE *p;
    char *line = (char *)malloc(sizeof(char) * MAX);
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    dict = btcrt("../db/dict.bt", 0, 0);
    note = btcrt("../db/note.bt", 0, 0);
    if ((p = fopen("../db/dict.txt", "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    while (fgets(line, MAX, p))
    {
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '_')
            {
                strcpy(eng, strtok(line, "_"));
                strcpy(vie, strtok(0, "\n"));
                btins(dict, eng, vie, MAX);
            }
        }
    }
    fclose(p);
    free(line);
    free(eng);
    free(vie);
    btcls(note);
    btcls(dict);
}