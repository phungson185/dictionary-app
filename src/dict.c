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

GtkWidget *window_advanced;

GtkEntry *searchentry, *entry_newword, *entry_meanword, *entry_del;

GtkWidget *textview1, *textview2, *textview3, *textview4, *textview_his;

BTA *tudien;
BTA *diff;
int check_his;
int check_trans;
int key_check;
char htr[MAX];
char buftrans[MAX];
// char eng[MAX];
// char vie[MAX];

void datainit();
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void set_mean_textview_text(GtkWidget *textview, char *text);
void translate();

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("../src/dict-app.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

    searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));
    entry_newword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_newword"));
    entry_meanword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_meanword"));

    textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));
    textview_his = GTK_WIDGET(gtk_builder_get_object(builder, "textview_his"));

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    list = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
    gtk_entry_set_completion(GTK_ENTRY(searchentry), comple);

    g_signal_connect(searchentry, "key_press_event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window_advanced, "destroy", G_CALLBACK(gtk_main_quit), NULL);

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
    FILE *p;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    char *key = (char *)malloc(sizeof(char) * MAX);

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
    free(key);
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
        set_mean_textview_text(textview1, "");
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
                count++;
                gtk_list_store_append(list, &Iter);
                gtk_list_store_set(list, &Iter, 0, eng, -1);
            }
            k = 0;
            if (count > 20)
                break;
        }
    }
    free(eng);
    free(vie);
}

void set_mean_textview_text(GtkWidget *textview, char *text)
{
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    if (buffer == NULL)
    {
        printf("Get buffer fail!");
        buffer = gtk_text_buffer_new(NULL);
    }
    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview), buffer);
}

void translate()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    btpos(tudien, ZSTART);
    int rsize;
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        //check_his = 1;
        if (strcmp(buftrans, gettext) != 0)
        {
            strcpy(buftrans, gettext);
            strcpy(buffer, gettext);
            strcat(buffer, "\n");
            strcat(buffer, htr);
            strcpy(htr, buffer);
        }
        if (btsel(tudien, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Không tìm thấy từ bạn cần tìm");
        else
        {
            set_mean_textview_text(textview1, value);
            // check_trans = 100;
        }
    }

    set_mean_textview_text(textview_his, htr);

    free(value);
    free(buffer);
}

void add_to_dict()
{
    printf("%d\n",1);
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    gchar new_word[MAX];
    gchar mean_word[MAX];
    btpos(tudien, ZSTART);
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 && strcmp(mean_word, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ mới và nghĩa cần thêm");
    }
    else
    {
        if (strcmp(new_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ mới cần thêm");
        }
        else if (strcmp(mean_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập nghĩa của từ cần thêm");
        }
        else
        {
            if (!btsel(tudien, new_word, value, MAX, &rsize))
            {
                gtk_label_set_text(GTK_LABEL(textview2), "Từ này đã có trong từ điển");
            }
            else
            {
                if (!btins(tudien, new_word, mean_word, MAX))
                    gtk_label_set_text(GTK_LABEL(textview2), "Đã thêm thành công");
                else
                    gtk_label_set_text(GTK_LABEL(textview2), "Thêm thất bại, chương trình lỗi...");
            }
        }
    }
    free(value);
}

void extend()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../src/dict-app.glade");

    window_advanced = GTK_WIDGET(gtk_builder_get_object(builder, "window_advanced"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window_advanced);
}