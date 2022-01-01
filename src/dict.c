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
#include <time.h>

#define MAX 10000

GdkColor red;
GdkColor green;

GtkBuilder *builder;
GtkWidget *window_main, *window_advanced, *window_note, *window_about, *window_question_filter, *window_game, *window_game_history;

GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter Iter;

GtkEntry *searchentry, *entry_newword, *entry_meanword, *entry_del, *question_num;

GtkWidget *textview1, *textview2, *textview3, *textview4, *textview5, *textview_his;
GtkWidget *lbl_eng, *check_vie1, *check_vie2, *check_vie3, *check_vie4;
GtkWidget *lv_y_hoc, *lv_toan_tin, *lv_dtvt, *lv_xay_dung, *lv_dien_lanh, *lv_hh_vat_lieu, *lv_dien, *lv_ckct, *cn_kinh_te, *cn_ky_thuat, *tu_ghi_chu;
GtkWidget *question, *ans_1, *ans_2, *ans_3, *ans_4, *total_num, *correct_num;
GtkWidget *btn_next, *btn_back;

BTA *dict;
BTA *note;

BTA *question_tree;

FILE *f;
char buftrans[MAX];
char his[MAX];
int key_check;

char dict_path[] = "../db/AnhViet.dat";
char note_path[] = "../db/note.bt";
char ui_path[] = "../ui/dict-app.glade";
char history_path[] = "../db/history.txt";
char question_path[] = "../db/question.bt";

int y_hoc, toan_tin, dtvt, xay_dung, dien_lanh, hh_vat_lieu, dien, ckct;
int kinh_te, ky_thuat, filter_ghi_chu;
long num_of_ques;
long lv_num, *ques_arr;

// JRB *game_tree;
long game_tree_size;

typedef struct game_result
{
    long total;
    int correct_num;
};
struct game_result game_result;

void set_mean_textview_text(GtkWidget *textview, char *text);
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

void translate();

void on_check_vie1_clicked(GtkButton *button);
void on_check_vie2_clicked(GtkButton *button);
void on_check_vie3_clicked(GtkButton *button);
void on_check_vie4_clicked(GtkButton *button);

int main(int argc, char *argv[])
{
    get_history();
    gdk_color_parse("red", &red);
    gdk_color_parse("green", &green);
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file(ui_path);
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

    dict = btopn(dict_path, 0, 0);

    g_object_unref(builder);
    gtk_widget_show(window_main);
    gtk_main();

    return 0;
}

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
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
        gettext[text_length] = event->keyval;
        gettext[text_length + 1] = '\0';
    }
    if (strcmp(gettext, "") == 0)
    {
        set_mean_textview_text(textview1, "");
    }
    else
    {
        kd[0] = gettext[0];
        kd[1] = '\0';
        gtk_list_store_clear(list);
        btsel(dict, kd, vie, sizeof(char *), &rsize);
        while (!btseln(dict, eng, vie, MAX, &rsize))
        {
            if (eng[0] != gettext[0])
                break;
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

void show_message(GtkWidget *parent, GtkMessageType type, char *mms, char *content)
{
    GtkWidget *mdialog;
    mdialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     type,
                                     GTK_BUTTONS_OK,
                                     "%s", mms);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(mdialog), "%s", content);
    if (type == GTK_MESSAGE_INFO)
        gtk_widget_modify_fg(GTK_MESSAGE_DIALOG(mdialog), GTK_STATE_NORMAL, &green);
    else
        gtk_widget_modify_fg(GTK_MESSAGE_DIALOG(mdialog), GTK_STATE_NORMAL, &red);
    gtk_dialog_run(GTK_DIALOG(mdialog));
    gtk_widget_destroy(mdialog);
}

void translate()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    // char *buffer = (char *)malloc(sizeof(char) * MAX);
    btpos(dict, ZSTART);
    int rsize;
    gchar gettext[MAX];
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
    {
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập từ tìm kiếm.");
    }
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Không tìm thấy từ bạn cần tìm");
        else
        {
            set_mean_textview_text(textview1, value);
            search_history_handler(gettext);
        }
    }

    set_mean_textview_text(textview_his, his);

    free(value);
}

void clear_history()
{
    strcpy(his, "");
    set_mean_textview_text(textview_his, his);
    fclose(fopen(history_path, "w"));
    show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa lịch sử tra từ thành công.");
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
    if (strcmp(new_word, "") == 0 || strcmp(mean_word, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin thêm từ còn thiếu.");
    else
    {

        if (!btsel(dict, new_word, value, MAX, &rsize))
        {
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Từ này đã có trong từ điển.");
        }
        else
        {
            if (!btins(dict, new_word, mean_word, MAX))
                show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Đã thêm thành công.");
            else
                show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Thêm từ thất bại, chương trình lỗi.");
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
    if (strcmp(new_word, "") == 0 || strcmp(mean_word, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin sửa từ còn thiếu.");
    else
    {
        if (!btsel(dict, new_word, value, MAX, &rsize))
            if (strcmp(value, mean_word) == 0)
                show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Nghĩa bạn thay thế giống nghĩa của từ điển.");

            else
            {
                if (!btupd(dict, new_word, mean_word, MAX))
                    show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Đã sửa thành công.");
                else
                    show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Sửa thất bại, chương trình lỗi.");
            }
        else
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Từ bạn cần sửa không có trong từ điển, vui lòng dùng chức năng thêm");
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
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập từ cần xóa.");
    else
    {
        if (btsel(dict, gettext, value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Không tìm thấy từ bạn cần xóa.");

        else
        {
            if (!btdel(dict, gettext))
                show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Đã xóa thành công.");
            else
                show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Xóa thất bại, chương trình lỗi.");
        }
    }
    free(value);
}

void add_to_note()
{
    note = btopn(note_path, 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    btpos(dict, ZSTART);
    btpos(note, ZSTART);

    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));
    if (strcmp(gettext, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập vào từ cần thêm vào danh sách ghi chú.");
    else
    {
        if (!btsel(note, gettext, value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Từ này đã có trong danh sách ghi chú.");
        else if (btsel(dict, gettext, value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Từ bạn nhập không có trong từ điển, không thể thêm.");
        else
        {
            if (!btins(note, gettext, value, MAX))
                show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Đã thêm thành công.");
            else
                show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Không thể thêm, chương trình lỗi.");
        }
    }
    free(value);
    btcls(note);
}

void delete_from_note()
{
    note = btopn(note_path, 0, 0);
    char gettext[MAX];
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    strcpy(gettext, gtk_entry_get_text(GTK_ENTRY(searchentry)));

    if (strcmp(gettext, "") == 0)
        show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Bạn chưa nhập vào từ cần xóa khỏi danh sách ghi chú");
    else
    {
        if (btsel(note, gettext, value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Từ bạn nhập không có trong danh sách ghi chú");
        else
        {
            if (!btdel(note, gettext))
                show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Đã xóa thành công.");
            else
                show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Không thể xóa, chương trình lỗi.");
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
    note = btopn(note_path, 0, 0);
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

typedef struct
{
    char *eng;
    char *vie;
} word;

word *make_word(char *eng, char *vie)
{
    word *w = (word *)malloc(sizeof(word));
    w->eng = strdup(eng);
    w->vie = strdup(vie);
    return w;
}
void on_btn_game_clicked()
{
    question_tree = btcrt(question_path, 0, 0);
    btcls(question_tree);

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_question_filter = GTK_WIDGET(gtk_builder_get_object(builder, "window_question_filter"));

    lv_y_hoc = GTK_WIDGET(gtk_builder_get_object(builder, "lv_y_hoc"));
    lv_toan_tin = GTK_WIDGET(gtk_builder_get_object(builder, "lv_toan_tin"));
    lv_dtvt = GTK_WIDGET(gtk_builder_get_object(builder, "lv_dtvt"));
    lv_xay_dung = GTK_WIDGET(gtk_builder_get_object(builder, "lv_xay_dung"));
    lv_dien_lanh = GTK_WIDGET(gtk_builder_get_object(builder, "lv_dien_lanh"));
    lv_hh_vat_lieu = GTK_WIDGET(gtk_builder_get_object(builder, "lv_hh_vat_lieu"));
    lv_dien = GTK_WIDGET(gtk_builder_get_object(builder, "lv_dien"));
    lv_ckct = GTK_WIDGET(gtk_builder_get_object(builder, "lv_ckct"));
    cn_kinh_te = GTK_WIDGET(gtk_builder_get_object(builder, "cn_kinh_te"));
    cn_ky_thuat = GTK_WIDGET(gtk_builder_get_object(builder, "cn_ky_thuat"));
    tu_ghi_chu = GTK_WIDGET(gtk_builder_get_object(builder, "tu_ghi_chu"));
    question_num = GTK_WIDGET(gtk_builder_get_object(builder, "question_num"));

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window_question_filter);
}
void get_filter()
{
    y_hoc = gtk_toggle_button_get_active(lv_y_hoc);
    toan_tin = gtk_toggle_button_get_active(lv_toan_tin);
    dtvt = gtk_toggle_button_get_active(lv_dtvt);
    xay_dung = gtk_toggle_button_get_active(lv_xay_dung);
    dien_lanh = gtk_toggle_button_get_active(lv_dien_lanh);
    hh_vat_lieu = gtk_toggle_button_get_active(lv_hh_vat_lieu);
    dien = gtk_toggle_button_get_active(lv_dien);
    ckct = gtk_toggle_button_get_active(lv_ckct);
    kinh_te = gtk_toggle_button_get_active(cn_kinh_te);
    ky_thuat = gtk_toggle_button_get_active(cn_ky_thuat);
    filter_ghi_chu = gtk_toggle_button_get_active(tu_ghi_chu);
    num_of_ques = atoi(gtk_entry_get_text(GTK_ENTRY(question_num)));
    if (num_of_ques == 0)
        num_of_ques = 30;
}
void print_question_tree(JRB question_tree)
{
    JRB ptr;
    jrb_traverse(ptr, question_tree)
    {
        word *w = (word *)ptr->val.v;
        printf("%d: ", ptr->key.i);
        printf("%s, ", w->eng);
        printf("%s\n", w->vie);
    }
}

char *make_long_to_string(long l)
{
    char *s = (char *)malloc(sizeof(char) * 10);
    sprintf(s, "%ld", l);
    return s;
}

void insert_game_tree(BTA *source, char filter[][50])
{
    long i = 0;
    char *start;
    char *end;
    char *target;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    char *lv = (char *)malloc(sizeof(char) * MAX);
    char *mean = (char *)malloc(sizeof(char) * MAX);

    int rsize, check_lv = 0;

    btpos(source, ZSTART);
    // btsel(source, "change", vie, MAX, &rsize);
    while (!btseln(source, eng, vie, MAX, &rsize))
    {
        strcpy(mean, "");
        check_lv = 0;

        for (int j = 0; j < 10; j++)
        {
            if (strlen(filter[j]) > 0)
            {

                // puts(filter[j]);
                strcpy(lv, filter[j]);
                if (start = strstr(vie, lv))
                {
                    check_lv++;

                    if (start = strtok(start, "@"))
                    {
                        strcat(mean, "@");
                        strcat(mean, start);
                    }
                    else if (start = strtok(0, "\n"))
                    {
                        strcat(mean, "@");
                        strcat(mean, start);
                    }

                    // check_lv++;
                    // start += strlen(lv);
                    // if (end = strstr(start, "@"))
                    // {
                    //     target = (char *)malloc(end - start + 1);
                    //     memcpy(target, start, end - start);
                    //     target[end - start] = '\0';
                    //     if (strlen(target) > 0)
                    //     {
                    //         strcat(mean, lv);
                    //         strcat(mean, target);
                    //     }
                    // }
                    // else if (end = strstr(start, "\0"))
                    // {
                    //     if (strlen(start) > 0)
                    //     {
                    //         strcat(mean, lv);
                    //         strcat(mean, start);
                    //     }
                    // }
                }
                // else
                //     break;
            }
        }
        if (strlen(mean) > 0 && check_lv == lv_num)
        {
            i++;
            // printf("id: %d eng: %s, mean: %s\n ", i, eng, mean);
            // jrb_insert_int(game_tree, i, (Jval){.v = make_word(eng, mean)});
            // strcpy(eng, "change");
            strcat(eng, "^");
            strcat(eng, mean);
            printf("%s\n", eng);
            btins(question_tree, make_long_to_string(i), eng, strlen(eng) + 1);
            // break;
        }
    }
    game_tree_size = i;
    printf("game size: %ld\n", game_tree_size);
    // free(eng);
    // free(vie);
    // free(lv);
    // free(mean);
    // free(start);
    // free(end);
    // free(target);
    btcls(note);
    btcls(question_tree);
}
void swap(long *a, long *b)
{
    long temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(long arr[], long n)
{
    for (long i = 0; i < n; i++)
        printf("%ld ", arr[i]);
    printf("\n");
}

void randomize(long arr[], long n)
{
    srand(time(NULL));

    for (long i = n - 1; i > 0; i--)
    {
        long j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

void create_radom_arr()
{
    // game_tree_size
    for (int i = 0; i < num_of_ques; i++)
    {
        ques_arr[i] = i;
    }
    for (int i = game_tree_size - 1; i >= 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&ques_arr[i], &ques_arr[j]);
    }
}
void new_game()
{
    get_filter();
    gtk_widget_destroy(window_question_filter);

    // game_tree = make_jrb();
    note = btopn(note_path, 0, 0);
    question_tree = btopn(question_path, 0, 0);
    // question_tree = btopn(question_path, 0, 0);

    char filter[10][50];
    lv_num = 0;
    memset(filter, '\0', sizeof(filter));

    if (y_hoc)
    {
        strcpy(filter[0], "@Lĩnh vực: y học\n");
        lv_num++;
    }
    if (toan_tin)
    {
        strcpy(filter[1], "@Lĩnh vực: toán & tin\n");
        lv_num++;
    }
    if (dtvt)
    {
        strcpy(filter[2], "@Lĩnh vực: điện tử & viễn thông\n");
        lv_num++;
    }
    if (xay_dung)
    {
        strcpy(filter[3], "@Lĩnh vực: xây dựng\n");
        lv_num++;
    }
    if (dien_lanh)
    {
        strcpy(filter[4], "@Lĩnh vực: điện lạnh\n");
        lv_num++;
    }
    if (hh_vat_lieu)
    {
        strcpy(filter[5], "@Lĩnh vực: hóa học & vật liệu\n");
        lv_num++;
    }
    if (dien)
    {
        strcpy(filter[6], "@Lĩnh vực: điện\n");
        lv_num++;
    }
    if (ckct)
    {
        strcpy(filter[7], "@Lĩnh vực: cơ khí & công trình\n");
        lv_num++;
    }
    if (ky_thuat)
    {
        strcpy(filter[8], "@Chuyên ngành kỹ thuật\n");
        lv_num++;
    }
    if (kinh_te)
    {
        strcpy(filter[9], "@Chuyên ngành kinh tế\n");
        lv_num++;
    }
    if (filter_ghi_chu)
        insert_game_tree(note, filter);
    else
        insert_game_tree(dict, filter);

    if (game_tree_size < num_of_ques)
        num_of_ques = game_tree_size;
    if (game_tree_size == 0)
    {
        show_message(window_question_filter, GTK_MESSAGE_ERROR, "ERROR", "Danh sách từ trống, không thể chơi trò chơi");
        return;
    }
    else if (game_tree_size > 0 && game_tree_size < 4)
    {
        show_message(window_question_filter, GTK_MESSAGE_ERROR, "ERROR", "Danh sách từ cần có 4 từ trở lên");
        return;
    }

    ques_arr = (long *)malloc((sizeof(long) * game_tree_size));

    for (long i = 0; i < game_tree_size; i++)
        ques_arr[i] = i + 1;
    randomize(ques_arr, game_tree_size);

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_game = GTK_WIDGET(gtk_builder_get_object(builder, "window_game"));

    ans_1 = GTK_WIDGET(gtk_builder_get_object(builder, "ans_1"));
    ans_2 = GTK_WIDGET(gtk_builder_get_object(builder, "ans_2"));
    ans_3 = GTK_WIDGET(gtk_builder_get_object(builder, "ans_3"));
    ans_4 = GTK_WIDGET(gtk_builder_get_object(builder, "ans_4"));
    question = GTK_WIDGET(gtk_builder_get_object(builder, "question"));
    total_num = GTK_WIDGET(gtk_builder_get_object(builder, "total_num"));
    correct_num = GTK_WIDGET(gtk_builder_get_object(builder, "correct_num"));

    btn_back = GTK_WIDGET(gtk_builder_get_object(builder, "btn_back"));
    btn_next = GTK_WIDGET(gtk_builder_get_object(builder, "btn_next"));

    g_signal_connect(ans_1, "button-press-event", G_CALLBACK(on_check_vie1_clicked), NULL);
    g_signal_connect(ans_2, "button-press-event", G_CALLBACK(on_check_vie2_clicked), NULL);
    g_signal_connect(ans_3, "button-press-event", G_CALLBACK(on_check_vie3_clicked), NULL);
    g_signal_connect(ans_4, "button-press-event", G_CALLBACK(on_check_vie4_clicked), NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window_game);
    new_record_result_of_game();
    question_tree = btopn(question_path, 0, 0);
    new_question();
}

char *make_int_to_string(int i)
{
    char *str = (char *)malloc(sizeof(char) * 10);
    sprintf(str, "%d", i);
    return str;
}

void new_question()
{
    long i = game_tree_size;
    time_t t;
    long key_word_correct = 0;
    long key_word2;
    long key_word3;
    int rsize = 0;
    char *buffer_correct = (char *)malloc(sizeof(char) * MAX);
    char *buffer1 = (char *)malloc(sizeof(char) * MAX);
    char *buffer2 = (char *)malloc(sizeof(char) * MAX);
    char *buffer3 = (char *)malloc(sizeof(char) * MAX);
    // srand((unsigned)time(&t));

    is_answed(FALSE);

    if (game_result.total + 1 >= num_of_ques)
    {
        gtk_widget_set_sensitive(btn_back, FALSE);
        gtk_widget_set_sensitive(btn_next, FALSE);
    }

    game_result.total++;

    key_word_correct = ques_arr[game_result.total - 1];

    int key_word1 = key_word2 = key_word3 = key_word_correct;
    key_check = rand() % 4 + 1;
    while (key_word1 == key_word_correct)
    {
        key_word1 = 1 + rand() % game_tree_size;
    }
    while (key_word2 == key_word_correct || key_word2 == key_word1)
    {
        key_word2 = 1 + rand() % game_tree_size;
    }
    while (key_word3 == key_word_correct || key_word3 == key_word1 || key_word3 == key_word2)
    {
        key_word3 = 1 + rand() % game_tree_size;
    }
    printf("%ld %ld %ld %ld\n", key_word_correct, key_word1, key_word2, key_word3);
    char *word = (char *)malloc(sizeof(char) * MAX);
    char *get_eng;
    if (!btsel(question_tree, make_long_to_string(key_word_correct), word, MAX, &rsize))
    {

        gtk_label_set_text(question, strtok(word, "^"));
        strcpy(buffer_correct, strtok(NULL, "^"));
    }

    if (!btsel(question_tree, make_long_to_string(key_word1), word, MAX, &rsize))
    {
        get_eng = strtok(word, "^");
        strcpy(buffer1, strtok(NULL, "^"));
    }

    if (!btsel(question_tree, make_long_to_string(key_word2), word, MAX, &rsize))
    {
        get_eng = strtok(word, "^");
        strcpy(buffer2, strtok(NULL, "^"));
    }

    if (!btsel(question_tree, make_long_to_string(key_word3), word, MAX, &rsize))
    {
        get_eng = strtok(word, "^");
        strcpy(buffer3, strtok(NULL, "^"));
    }

    char *str = (char *)malloc(sizeof(char) * 10);
    sprintf(str, "%d/%d", game_result.total, num_of_ques);
    gtk_label_set_text(total_num, str);
    sprintf(str, "%d", game_result.correct_num);
    gtk_label_set_text(correct_num, str);
    if (key_check == 1)
    {
        set_mean_textview_text(ans_1, buffer_correct);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else if (key_check == 2)
    {
        set_mean_textview_text(ans_2, buffer_correct);
        set_mean_textview_text(ans_1, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else if (key_check == 3)
    {
        set_mean_textview_text(ans_3, buffer_correct);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_1, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else
    {
        set_mean_textview_text(ans_4, buffer_correct);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_1, buffer3);
    }

    // free(str);
    // free(word);
    // free(buffer_correct);
    // free(buffer1);
    // free(buffer2);
    // free(buffer3);
}

void new_record_result_of_game()
{
    game_result.total = 0;
    game_result.correct_num = 0;
}
void out_game()
{
    btcls(question_tree);
    gtk_widget_destroy(window_game);
    char *end_info = (char *)malloc(sizeof(char) * 100);
    strcpy(end_info, "Bạn đã hoàn thành bài thi!\n");
    strcat(end_info, "Số câu đúng: ");
    strcat(end_info, make_int_to_string(game_result.correct_num));
    strcat(end_info, "/");
    strcat(end_info, make_int_to_string(num_of_ques));
    show_message(window_main, GTK_MESSAGE_INFO, "KẾT THÚC", end_info);
    free(end_info);
    save_record_result_of_game();
}

void save_record_result_of_game()
{
    char end_time[30];
    char *buf = (char *)malloc(sizeof(char) * MAX);

    if ((f = fopen("../db/game_history.txt", "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strcpy(end_time, asctime(tm));
    end_time[strlen(end_time) - 1] = '\0';
    sprintf(buf, "%s-%ld-%ld", end_time, game_result.correct_num, num_of_ques);
    fprintf(f, "%s\n", buf);
    fclose(f);
}
void is_answed(int bool)
{
    show_correct(bool);
    gtk_widget_set_sensitive(ans_1, !bool);
    gtk_widget_set_sensitive(ans_2, !bool);
    gtk_widget_set_sensitive(ans_3, !bool);
    gtk_widget_set_sensitive(ans_4, !bool);
}
void show_correct(int bool)
{
    GdkColor color;

    if (bool)
        gdk_color_parse("red", &color);
    else
        return hide_correct();

    if (key_check == 1)
    {
        gtk_widget_modify_fg(GTK_WIDGET(ans_1), GTK_STATE_NORMAL, &color);
    }
    else if (key_check == 2)
    {
        gtk_widget_modify_fg(GTK_WIDGET(ans_2), GTK_STATE_NORMAL, &color);
    }
    else if (key_check == 3)
    {
        gtk_widget_modify_fg(GTK_WIDGET(ans_3), GTK_STATE_NORMAL, &color);
    }
    else if (key_check == 4)
    {
        gtk_widget_modify_fg(GTK_WIDGET(ans_4), GTK_STATE_NORMAL, &color);
    }
}
void hide_correct()
{
    gtk_widget_modify_fg(GTK_WIDGET(ans_1), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_2), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_3), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_4), GTK_STATE_NORMAL, NULL);
}
void on_check_vie1_clicked(GtkButton *button)
{
    if (key_check == 1)
    {
        game_result.correct_num++;
    }
    is_answed(TRUE);
}
void on_check_vie2_clicked(GtkButton *button)
{
    if (key_check == 2)
    {
        game_result.correct_num++;
    }

    is_answed(TRUE);
}
void on_check_vie3_clicked(GtkButton *button)
{
    if (key_check == 3)
    {
        game_result.correct_num++;
    }

    is_answed(TRUE);
}
void on_check_vie4_clicked(GtkButton *button)
{
    if (key_check == 4)
    {
        game_result.correct_num++;
    }

    is_answed(TRUE);
}
void show_game_his(GtkButton *button)
{
    gtk_widget_destroy(window_game_history);
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file(ui_path);

    gtk_builder_connect_signals(builder, NULL);

    window_game_history = GTK_WIDGET(gtk_builder_get_object(builder, "window_game_history"));

    textview5 = GTK_WIDGET(gtk_builder_get_object(builder, "textview5"));

    GtkTextBuffer *buffer;
    GtkTextIter translation_iter, start, end;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview5));
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
    gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "red", NULL);
    gtk_text_buffer_create_tag(buffer, "yellow_fg", "foreground", "yellow", NULL);
    gtk_text_buffer_create_tag(buffer, "green_fg", "foreground", "green", NULL);
    gtk_text_buffer_get_iter_at_offset(buffer, &translation_iter, 0);

    char *get_end_time;
    char *get_correct_num;
    char *get_game_tree_size;
    FILE *fd;
    int ch;
    int count;
    char line[100];
    char len[100];
    char buf[100];
    int index = 0;
        int i;

    if ((fd = fopen("../db/game_history.txt", "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }

    fseek(fd, 0, SEEK_END);
    while (ftell(fd) > 1)
    {
        fseek(fd, -2, SEEK_CUR);
        if (ftell(fd) <= 2)
            break;
        ch = fgetc(fd);
        count = 0;
        while (ch != '\n')
        {
            len[count++] = ch;
            if (ftell(fd) < 2)
                break;
            fseek(fd, -2, SEEK_CUR);
            ch = fgetc(fd);
        }
        for ( i = count - 1; i >= 0 && count > 0; i--)
            buf[index++] = len[i];

        get_end_time = strtok(buf, "-");
        get_correct_num = strtok(NULL, "-");
        get_game_tree_size = strtok(NULL, "-");
        sprintf(line, "%-30s%-10s%-5s\n", get_end_time, get_correct_num, get_game_tree_size);
        float lv = (atof(get_correct_num) / atof(get_game_tree_size)) * 10;
        if (lv >= 0 && lv <= 3)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, line, -1, "red_fg", NULL);
        else if (lv >= 4 && lv <= 6)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, line, -1, "yellow_fg", NULL);
        else if (lv >= 7 && lv <= 10)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &translation_iter, line, -1, "green_fg", NULL);

        index = 0;
        memset(buf, '\0', 100);
    }

    fclose(fd);

    g_object_unref(builder);
    gtk_widget_show(window_game_history);
}

void delelte_all_game_history() {
    FILE *fd;
    if ((fd = fopen("../db/game_history.txt", "w")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    fclose(fd);
    set_mean_textview_text(textview5, "Danh sách trống !");
}

void delete_all_note()
{
    note = btcrt(note_path, 0, 0);
    set_mean_textview_text(textview3, "Danh sách trống");
    btcls(note);
}

void about()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file(ui_path);

    window_about = GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window_about);
}
void get_history()
{
    char buffer[MAX];
    char line[MAX];
    if ((f = fopen(history_path, "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    while (fgets(line, MAX, f))
    {
        memset(buffer, 0, 4);
        strcpy(buffer, line);
        strcat(buffer, his);
        strcpy(his, buffer);
    }
    strcat(his, "\n");
    // printf(his);
    fclose(f);
}
void add_to_history(char *buf)
{
    char line[MAX];
    if ((f = fopen(history_path, "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    fprintf(f, "%s", buf);
    fclose(f);
}

void rewrite_history(char *his)
{
}
void search_history_handler(char *gettext)
{
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    sprintf(buftrans, "%s\n", gettext);
    int i = strremove(his, buftrans);
    strcpy(buffer, buftrans);
    strcat(buffer, his);
    strcpy(his, buffer);
    free(buffer);
    if (i == 0)
        add_to_history(buftrans);
    else
        rewrite_history(his);
}
int strremove(char *str, char *sub)
{
    size_t len = strlen(sub);
    if (len > 0)
    {
        char *p = str;
        if ((p = strstr(p, sub)) != NULL)
        {
            memmove(p, p + len, strlen(p + len) + 1);
            return 1;
        }
    }
    return 0;
}