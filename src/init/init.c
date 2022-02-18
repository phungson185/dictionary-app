#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#define MAX 1000

int main()
{
    btinit();
    BTA *note;
    BTA *question;
    note = btcrt("../db/note.bt", 0, 0);
    question = btcrt("../db/question.bt", 0, 0);
    
    btcls(note);
    btcls(question);
}