#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
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
GtkWidget *window_main, *window_advanced, *window_note, *window_about;
GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter Iter;

GtkEntry *searchentry, *entry_newword, *entry_meanword, *entry_del;

GtkWidget *textview1, *textview2, *textview3, *textview4, *textview_his;

BTA *dict;
BTA *note;
FILE *f;
char buftrans[MAX];
char his[MAX];

void set_mean_textview_text(GtkWidget *textview, char *text);
void on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);

void translate();

int main(int argc, char *argv[])
{
    get_history();

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

    searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));

    textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    textview_his = GTK_WIDGET(gtk_builder_get_object(builder, "textview_his"));

    set_mean_textview_text(textview_his, his);

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    list = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
    gtk_entry_set_completion(GTK_ENTRY(searchentry), comple);

    g_signal_connect(searchentry, "key_press_event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window_main, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    dict = btopn("../db/dict.bt", 0, 0);

    g_object_unref(builder);
    gtk_widget_show(window_main);
    gtk_main();
    

    return 0;
}

void get_history(){
    char buffer[MAX];
    char line[MAX];
    if ((f = fopen("../db/history.txt", "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    while (fgets(line, MAX, f))
    {
        memset(buffer, 0, 4);
        strcpy(buffer, line);
        strcat(buffer,his);
        strcpy(his,buffer);
    }
    strcat(his,"\n");
    printf(his);
    fclose(f);
}
void add_to_history(char * buf){
    char line[MAX];
    if ((f = fopen("../db/history.txt", "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    strcat(buf,"\n");
    fprintf(f,"%s",buf);
    fclose(f);
}

void on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    gchar gettext[100];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    int text_length = strlen(gettext);
    int rsize;
    int k = 0, count = 0;
    char kd[2];
    if (event->keyval != GDK_KEY_BackSpace)
    {
        gettext[text_length]=event->keyval;
        gettext[text_length+1]='\0';
    }
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        kd[0]=gettext[0];
        kd[1]='\0';
        gtk_list_store_clear(list);
        btsel(dict, kd, vie, sizeof(char*), &rsize);
        while (!btseln(dict, eng, vie, MAX, &rsize))
        {
            if(eng[0] != gettext[0]) break;
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
    btpos(dict, ZSTART);
    int rsize;
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Không tìm thấy từ bạn cần tìm");
        else
        {
            if (strcmp(buftrans, gettext) != 0)
            {
                strcpy(buftrans, gettext);
                strcpy(buffer, gettext);
                strcat(buffer, "\n");
                strcat(buffer,his);
                strcpy(his,buffer);
                add_to_history(buftrans);
            }
            set_mean_textview_text(textview1, value);
        }
    }

    set_mean_textview_text(textview_his, his);

    free(value);
    free(buffer);
}

void clear_history()
{
    strcpy(his, "");
    set_mean_textview_text(textview_his, his);
    fclose(fopen("../db/history.txt", "w"));
}

void add_to_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    gchar new_word[MAX];
    gchar mean_word[MAX];
    btpos(dict, ZSTART);
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
            if (!btsel(dict, new_word, value, MAX, &rsize))
            {
                gtk_label_set_text(GTK_LABEL(textview2), "Từ này đã có trong từ điển");
            }
            else
            {
                if (!btins(dict, new_word, mean_word, MAX))
                    gtk_label_set_text(GTK_LABEL(textview2), "Đã thêm thành công");
                else
                    gtk_label_set_text(GTK_LABEL(textview2), "Thêm thất bại, chương trình lỗi...");
            }
        }
    }
    free(value);
}

void repair_word()
{
    gchar new_word[MAX];
    gchar mean_word[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    btpos(dict, ZSTART);
    strcpy(new_word, gtk_entry_get_text(GTK_ENTRY(entry_newword)));
    strcpy(mean_word, gtk_entry_get_text(GTK_ENTRY(entry_meanword)));
    if (strcmp(new_word, "") == 0 && strcmp(mean_word, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ và nghĩa cần sửa");
    }
    else
    {
        if (strcmp(new_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ cần sửa");
        }
        else if (strcmp(mean_word, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập nghĩa cần thay thế");
        }
        else if (!btsel(dict, new_word, value, MAX, &rsize))
            if (strcmp(value, mean_word) == 0)
                gtk_label_set_text(GTK_LABEL(textview2), "Nghĩa bạn thay thế giống nghĩa của từ điển");
            else
            {
                if (!btupd(dict, new_word, mean_word, MAX))
                    gtk_label_set_text(GTK_LABEL(textview2), "Đã sửa thành công");
                else
                    gtk_label_set_text(GTK_LABEL(textview2), "Sửa thất bại, chương trình lỗi...");
            }
        else
            gtk_label_set_text(GTK_LABEL(textview2), "Từ bạn cần sửa không có trong từ điển, vui lòng dùng chức năng thêm");
    }
    free(value);
}

void delete_from_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    gchar gettext[MAX];
    btpos(dict, ZSTART);
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(entry_del)));
    if (strcmp(gettext, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(textview2), "Bạn chưa nhập từ cần xóa");
    }
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            gtk_label_set_text(GTK_LABEL(textview2), "Không tìm thấy từ bạn cần xóa");
        else
        {
            if (!btdel(dict, gettext))
                gtk_label_set_text(GTK_LABEL(textview2), "Đã xóa thành công");
            else
                gtk_label_set_text(GTK_LABEL(textview2), "Xóa thất bại, chương trình lỗi...");
        }
    }
    free(value);
}

void add_to_note()
{
    note = btopn("../db/note.bt", 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    btpos(dict, ZSTART);
    btpos(note, ZSTART);

    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "Bạn chưa nhập vào từ cần thêm vào danh sách ghi chú");
    }
    else
    {
        if (!btsel(note, gettext, value, MAX, &rsize))
        {
            set_mean_textview_text(textview1, "Từ này đã có trong danh sách ghi chú");
        }
        else if (btsel(dict, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Từ bạn nhập không có trong từ điển, không thể thêm...");
        else
        {
            if (!btins(note, gettext, value, MAX))
            {
                set_mean_textview_text(textview1, "Đã thêm thành công");
            }
            else
                set_mean_textview_text(textview1, "Không thể thêm, chương trình lỗi...");
        }
    }
    free(value);
    btcls(note);
}

void delete_from_note()
{
    note = btopn("../db/note.bt", 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));

    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "Bạn chưa nhập vào từ cần xóa khỏi danh sách ghi chú");
    }
    else
    {
        if (btsel(note, gettext, value, MAX, &rsize))
            set_mean_textview_text(textview1, "Từ bạn nhập không có trong danh sách ghi chú");
        else
        {
            if (!btdel(note, gettext))
            {
                set_mean_textview_text(textview1, "Đã xóa thành công");
            }
            else
                set_mean_textview_text(textview1, "Không thể xóa, chương trình lỗi...");
        }
    }
    free(value);
    btcls(note);
}

void extend()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_advanced = GTK_WIDGET(gtk_builder_get_object(builder, "window_advanced"));
    gtk_builder_connect_signals(builder, NULL);

    entry_newword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_newword"));
    entry_meanword = GTK_WIDGET(gtk_builder_get_object(builder, "entry_meanword"));
    entry_del = GTK_WIDGET(gtk_builder_get_object(builder, "entry_del"));
    textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));

    g_object_unref(builder);
    gtk_widget_show(window_advanced);
}

void practice()
{
    note = btopn("../db/note.bt", 0, 0);
    int SIZE_OF_NOTE = 0;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, flag = 0;
    btpos(note, ZSTART);

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_note = GTK_WIDGET(gtk_builder_get_object(builder, "window_note"));
    gtk_builder_connect_signals(builder, NULL);

    textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));

    g_object_unref(builder);
    gtk_widget_show(window_note);

    while (!btseln(note, eng, vie, MAX, &rsize))
    {
        SIZE_OF_NOTE++;
    }
    char *list = (char *)malloc(sizeof(char) * MAX * SIZE_OF_NOTE);
    if (SIZE_OF_NOTE == 0)
        set_mean_textview_text(textview3, "Danh sách trống");
    else
    {
        btpos(note, ZSTART);
        while (!btseln(note, eng, vie, MAX, &rsize))
        {
            if (!flag)
            {
                strcpy(list, eng);
                strcat(list, "\n");
            }
            else
            {
                strcat(list, eng);
                strcat(list, "\n");
            }
            flag = 1;
        }
        set_mean_textview_text(textview3, list);
    }
    free(list);
    btcls(note);
}

void delete_all_note()
{
    note = btcrt("note.bt", 0, 0);
    set_mean_textview_text(textview3, "Danh sách trống");
    btcls(note);
}

void about()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_about = GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window_about);
}