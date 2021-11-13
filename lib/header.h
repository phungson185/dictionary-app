#ifndef	_HEADER_H_
#define	_HEADER_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <jrb.h>
#include <jval.h>
#include <string.h>
#include <btree.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX 1000
typedef struct {    
    GtkWidget *window_history;
    GtkWidget *window_advanced;
    GtkWidget *window_diffword;
    GtkWidget *window_list_diff;
    GtkWidget *window_game;
    GtkWidget *window_about; 

    GtkEntry *searchentry;
    GtkEntry *entry_newword;
    GtkEntry *entry_meanword;
    GtkEntry *entry_del;
    
    GtkWidget *textview1;
    GtkWidget *textview2;
    GtkWidget *textview3;
    GtkWidget *textview4;

    GtkWidget *lbl_history;
    GtkWidget *lbl_list_diff;
    GtkWidget *lbl_suggest1;
    GtkWidget *lbl_suggest2;
    GtkWidget *lbl_suggest3;
    GtkWidget *lbl_suggest4;
    GtkWidget *lbl_suggest5;
    GtkWidget *lbl_suggest6;
    GtkWidget *lbl_suggest7;
    GtkWidget *lbl_suggest8;
    GtkWidget *lbl_suggest9;
    GtkWidget *lbl_suggest10;
    GtkWidget *lbl_yeucau;
    GtkWidget *lbl_eng;
    GtkWidget *lbl_vie1;
    GtkWidget *lbl_vie2;
    GtkWidget *lbl_vie3;
    GtkWidget *lbl_vie4;

    GtkWidget *check_vie1;
    GtkWidget *check_vie2;
    GtkWidget *check_vie3;
    GtkWidget *check_vie4;
    
} app_widgets;
BTA* tudien;
BTA* diff;
int check_his;
int check_trans;
int key_check;
char htr[MAX];
char buftrans[MAX];
char eng[MAX];
char vie[MAX];
char key[MAX];

#endif