#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <btree.h>
#include <stdlib.h>

#include "utils.h"

#define MAX 1000

GtkBuilder *builder;
GtkWidget *window_main, *textview1, *textview_his;
GtkEntryCompletion *comple;
GtkListStore *list;
GtkTreeIter Iter;
GtkEntry *searchentry;
GtkCssProvider *provider;

GdkDisplay *display;
GdkScreen *screen;
GdkColor red;
GdkColor green;

GError *error;

BTA *dict;
BTA *note;

char dict_path[100];
char note_path[100];
char ui_path[100];
char history_path[100];
char question_path[100];
char game_history_path[100];

char buftrans[MAX];
char his[MAX];

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void translate();
void clear_history();
void add_to_note();
void delete_from_note();

int strremove(char *str, char *sub);
void search_history_handler(char *gettext);
void rewrite_history(char *his);
void get_history();
void add_to_history(char *buf);

#endif