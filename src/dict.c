#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <jrb.h>
#include <jval.h>
#include <string.h>
#include <btree.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 1000

GtkBuilder *builder;
GtkWidget *window;
GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter Iter;

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

BTA *tudien;
BTA *diff;
int check_his;
int check_trans;
int key_check;
char htr[MAX];
char buftrans[MAX];
// char eng[MAX];
// char vie[MAX];
char key[MAX];

void datainit();
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void translate();

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("../src/dict-app.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));

    textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    list = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
    gtk_entry_set_completion(GTK_ENTRY(searchentry), comple);

    g_signal_connect(searchentry, "key_press_event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    datainit();
    tudien = btopn("dict.bt", 0, 0);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void datainit()
{
    btinit();
    BTA *tudien;
    BTA *diff;
    FILE *p;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);

    diff = btcrt("favourite_word.bt", 0, 0);
    tudien = btcrt("dict.bt", 0, 0);
    if ((p = fopen("../src/data.txt", "r")) == NULL)
    {
        printf("Loi khong the mo file.\n");
        return;
    }
    while (fgets(key, MAX, p))
    {
        for (int i = 0; i < strlen(key); i++)
        {
            if (key[i] == '_')
            {
                strcpy(eng, strtok(key, "_"));
                strcpy(vie, strtok(0, "\n"));
                btins(tudien, eng, vie, MAX);
            }
        }
    }
    fclose(p);
    free(eng);
    free(vie);
    btcls(diff);
    btcls(tudien);
}

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    gchar gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    int rsize;
    int k = 0, count = 0;
    btpos(tudien, ZSTART);
    gtk_list_store_clear(list);
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview1_text("");
    }
    else
    {
        while (!btseln(tudien, eng, vie, MAX, &rsize))
        {
            for (int i = 0; i < strlen(gettext); i++)
            {
                if (eng[i] != gettext[i])
                {
                    k = 1;
                    break;
                }
            }
            if (k == 0)
            {
                gtk_list_store_append(list, &Iter);
                gtk_list_store_set(list, &Iter, 0, eng, -1);
            }
            k = 0;
        }
    }
    free(eng);
    free(vie);
}

void set_mean_textview1_text(char *text)
{
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview1));
    if (buffer == NULL)
    {
        printf("Get buffer fail!");
        buffer = gtk_text_buffer_new(NULL);
    }
    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview1), buffer);
}

void translate()
{
    char value[MAX];
    char buffer[MAX];
    int rsize;
    gchar gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview1_text("");
    }
    else
    {
        check_his = 1;
        if (strcmp(buftrans, gettext) != 0)
        {
            strcpy(buftrans, gettext);
            strcpy(buffer, gettext);
            strcat(buffer, "\n");
            strcat(buffer, htr);
            strcpy(htr, buffer);
        }
        if (btsel(tudien, gettext, value, MAX, &rsize))
            set_mean_textview1_text("Không tìm thấy từ bạn cần tìm");
        else
        {
            printf("%s\n",value);
            set_mean_textview1_text(value);
            check_trans = 100;
        }
    }
    btcls(tudien);
}