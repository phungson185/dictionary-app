#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <btree.h>
#include <stdlib.h>
#include <jrb.h>
#include <jval.h>

#include "utils.h"

#define MAX 10000

typedef struct {
    char *option;
    char *mean;
} Option;

GtkBuilder *builder;
GtkWidget *window_main, *textview1, *textview_his, *combo;
GtkEntryCompletion *comple;
GtkListStore *list, *store;
GtkTreeIter Iter, iter;
GtkEntry *searchentry;
GtkCssProvider *provider;
GtkCellRenderer *renderer;

GdkDisplay *display;
GdkScreen *screen;
GdkColor red;
GdkColor green;

GError *error;

GList *cbitems;

BTA *dict;
BTA *note;
JRB search_result;

char dict_path[100];
char note_path[100];
char ui_path[100];
char history_path[100];
char question_path[100];
char game_history_path[100];

char buftrans[MAX];
char his[MAX];
char translate_value[MAX];
static char word_type[26][50] = {
    "danh từ\n",
    "động từ\n",
    "tính từ\n",
    "trạng từ\n",
    "giới từ\n",
    "đại từ\n",
    "liên từ\n",
    "thán từ\n",
    "nội động từ\n",
    "ngoại động từ\n",
    "Lĩnh vực: y học\n",
    "Lĩnh vực: toán & tin\n",
    "Lĩnh vực: điện tử & viễn thông\n",
    "Lĩnh vực: xây dựng\n",
    "Lĩnh vực: điện lạnh\n",
    "Lĩnh vực: hóa học & vật liệu\n",
    "Lĩnh vực: điện\n",
    "Lĩnh vực: cơ khí & công trình\n",
    "Lĩnh vực: ô tô\n",
    "Lĩnh vực: dệt may\n",
    "Lĩnh vực: thực phẩm\n",
    "Lĩnh vực: môi trường\n",
    "Lĩnh vực: vật lý\n",
    "Lĩnh vực: giao thông & vận tải\n",
    "Chuyên ngành kỹ thuật\n",
    "Chuyên ngành kinh tế\n"
};

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void translate();
void split_result(char *s);
void on_combo_changed();
void clear_history();
void add_to_note();
void delete_from_note();

int strremove(char *str, char *sub);
void search_history_handler(char *gettext);
void rewrite_history(char *his);
void get_history();
void add_to_history(char *buf);

#endif