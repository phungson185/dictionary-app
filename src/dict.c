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

#define MAX 10000

GdkColor red;
GdkColor green;

GtkBuilder *builder;
GtkWidget *window_main, *window_advanced, *window_note, *window_about,*window_question_filter, *window_game, *window_game_history;

GtkEntryCompletion *comple;

GtkListStore *list;
GtkTreeIter Iter;

GtkEntry *searchentry, *entry_newword, *entry_meanword, *entry_del;

GtkWidget *textview1, *textview2, *textview3, *textview4, *textview5, *textview_his;
GtkWidget *lbl_eng, *check_vie1, *check_vie2, *check_vie3, *check_vie4;
GtkWidget *lv_y_hoc, *lv_toan_tin, *lv_dtvt, *lv_xay_dung, *lv_dien_lanh, *lv_hh_vat_lieu, *lv_dien, *lv_ckct, *cn_kinh_te, *cn_ky_thuat, *tu_ghi_chu, *question_num;
GtkWidget *question, *ans_1, *ans_2, *ans_3, *ans_4, *total_num, *correct_num;

BTA *dict;
BTA *note;
FILE *f;
char buftrans[MAX];
char his[MAX];
int key_check;

char dict_path[] = "../db/AnhViet.dat";
char note_path[] = "../db/note.bt";
char ui_path[] = "../ui/dict-app.glade";
char history_path[] = "../db/history.txt";

int y_hoc,toan_tin, dtvt, xay_dung, dien_lanh, hh_vat_lieu, dien, ckct;
int kinh_te, ky_thuat;
int filter_ghi_chu;
int num_of_ques=30;

JRB *game_tree;
int game_tree_size;

typedef struct game_result
{
    int total;
    int correct_num;
};
struct game_result game_result;

void set_mean_textview_text(GtkWidget *textview, char *text);
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

void translate();

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

typedef struct {
  char *eng;
  char *vie;
} word;

word *make_word(char *eng, char *vie) {
  word *w = (word*)malloc(sizeof(word));
  w->eng = strdup(eng);
  w->vie = strdup(vie);
  return w;
}
void on_btn_game_clicked()
{
    gtk_widget_destroy(window_note);
    GtkBuilder *builder;
 
    builder = gtk_builder_new_from_file("../ui/dict-app.glade");

    window_question_filter = GTK_WIDGET(gtk_builder_get_object(builder, "window_question_filter"));
    
    lv_y_hoc=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_y_hoc"));
    lv_toan_tin=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_toan_tin"));
    lv_dtvt=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_dtvt"));
    lv_xay_dung=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_xay_dung"));
    lv_dien_lanh=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_dien_lanh"));
    lv_hh_vat_lieu=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_hh_vat_lieu"));
    lv_dien=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_dien"));
    lv_ckct=   GTK_WIDGET(gtk_builder_get_object(builder, "lv_ckct"));
    cn_kinh_te=   GTK_WIDGET(gtk_builder_get_object(builder, "cn_kinh_te"));
    cn_ky_thuat=   GTK_WIDGET(gtk_builder_get_object(builder, "cn_ky_thuat"));
    tu_ghi_chu=   GTK_WIDGET(gtk_builder_get_object(builder, "tu_ghi_chu"));
    question_num=   GTK_WIDGET(gtk_builder_get_object(builder, "question_num"));

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window_question_filter);


}
void get_filter(){
    y_hoc = gtk_toggle_button_get_active(lv_y_hoc); 
    toan_tin=   gtk_toggle_button_get_active(lv_toan_tin);
    dtvt=   gtk_toggle_button_get_active(lv_dtvt);
    xay_dung=   gtk_toggle_button_get_active(lv_xay_dung);
    dien_lanh=   gtk_toggle_button_get_active(lv_dien_lanh);
    hh_vat_lieu=   gtk_toggle_button_get_active(lv_hh_vat_lieu);
    dien=   gtk_toggle_button_get_active(lv_dien);
    ckct=   gtk_toggle_button_get_active(lv_ckct);
    kinh_te=   gtk_toggle_button_get_active(cn_kinh_te);
    ky_thuat=   gtk_toggle_button_get_active(cn_ky_thuat);
    filter_ghi_chu=   gtk_toggle_button_get_active(tu_ghi_chu);
}
void print_question_tree(JRB question_tree) {
  JRB ptr;
  jrb_traverse(ptr, question_tree) {
    word *w = (word*)ptr->val.v;
    printf("%d: ", ptr->key.i);
    printf("%s, ", w->eng);
    printf("%s\n", w->vie);

  }
  
}
char * get_mean(char* vie, char* lv)
{
    char *start, *end;
    char *target=NULL;
    // printf("lv: %s", lv);
    // return vie;
    start=strstr(vie,lv);
    if(start!=NULL){

        start += strlen(lv);
        if (end = strstr( start, "@" ))
            {
            target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
            printf("target: %s\n", target);
            return target;
        // i++;
        // jrb_insert_int(game_tree, i, (Jval){.v = make_word(eng,target)});
            }
            else if( end = strstr( start, "\0" )) {
                printf("start: %s\n", start);
            return start;
            // i++;
            // jrb_insert_int(game_tree, i, (Jval){.v = make_word(eng,start)});
            }
    }
    strcpy(start, "empty");
    return start;
}
void new_game(){
    int i=0;
    get_filter();
    gtk_widget_destroy(window_question_filter);

    int SIZE_OF_NOTE = 0;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, flag = 0;


    game_tree = make_jrb();

    char *start, *end;
    char *target=NULL;
    char *lv= (char *)malloc(sizeof(char) * MAX);
    btpos(dict,ZSTART);
    // toan_tin, dtvt, xay_dung, dien_lanh, hh_vat_lieu, dien, ckct
    if(y_hoc){
        strcpy(lv, "@Lĩnh vực: y học\n");
    }
    if(toan_tin){
        strcpy(lv, "@Lĩnh vực: toán & tin\n");
    }
    if(dtvt){
        strcpy(lv, "@Lĩnh vực: điện tử & viễn thông\n");
    }
    if(xay_dung){
        strcpy(lv, "@Lĩnh vực: xây dựng\n");
    }
    if(dien_lanh){
        strcpy(lv, "@Lĩnh vực: điện lạnh\n");
    }
    if(hh_vat_lieu){
        strcpy(lv, "@Lĩnh vực: hóa học & vật liệu\n");
    }
    if(dien){
        strcpy(lv, "@Lĩnh vực: điện\n");
    }
    if(ckct){
        strcpy(lv, "@Lĩnh vực: cơ khí & công trình\n");
    }
    if(ky_thuat){
        strcpy(lv,"@Chuyên ngành kỹ thuật\n");
    }
    if(kinh_te){
        strcpy(lv,"@Chuyên ngành kinh tế\n");
    }
    printf("%s",lv);
    while (!btseln(dict, eng, vie, MAX, &rsize))
    {
        //tach lay nghia
        if(i>num_of_ques) break;
        start=strstr(vie,lv);
        if(start!=NULL){
            start += strlen(lv);
            if ( end = strstr( start, "@" ))
             { target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
            if(strlen(target)>0){
                i++;
                jrb_insert_int(game_tree, i, (Jval){.v = make_word(eng,target)});
            }
             }
             else if( end = strstr( start, "\0" )) {
                if(strlen(start)>0){
                    i++;
                    jrb_insert_int(game_tree, i, (Jval){.v = make_word(eng,start)});
                }
             }
        }
    } 
    printf("i= %d\n", i);

    game_tree_size=i;
    if (i == 0)
    {
        jrb_free_tree(game_tree);
        show_message(window_question_filter,GTK_MESSAGE_ERROR, "ERROR", "Danh sách từ trống, không thể chơi trò chơi" );
        return;
    }
    else if (i > 0 && i < 4){
        jrb_free_tree(game_tree);
        show_message(window_question_filter,GTK_MESSAGE_ERROR, "ERROR", "Danh sách từ cần có 4 từ trở lên" );
        return;
    }
    btcls(note);

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
    

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window_game);
    new_record_result_of_game();
    new_question();
    

}
void new_question(){
    int i =game_tree_size;
    time_t t;
    int key_word_correct = 0;
    int key_word2;
    int key_word3;
    int  rsize=0;
    char buffer1[MAX];
    char buffer2[MAX];
    char buffer3[MAX];
    srand((unsigned)time(&t));
    
    is_answed(FALSE);

    key_word_correct = rand() % i + 1;
    int key_word1 = key_word2 = key_word3 = key_word_correct;
    key_check = rand() % 4 + 1;
    while (key_word1 == key_word_correct)
    {
        key_word1 = rand() % i + 1;
    }
    while (key_word2 == key_word_correct || key_word2 == key_word1)
    {
        key_word2 = rand() % i + 1;
    }
    while (key_word3 == key_word_correct || key_word3 == key_word1 || key_word3 == key_word2)
    {
        key_word3 = rand() % i + 1;
    }

    word* w= jrb_find_int(game_tree, key_word_correct)->val.v;
    word* w1= jrb_find_int(game_tree, key_word1)->val.v;
    strcpy(buffer1,w1->vie) ;
    word* w2= jrb_find_int(game_tree, key_word2)->val.v;
    strcpy(buffer2,w2->vie) ;
    word* w3= jrb_find_int(game_tree, key_word3)->val.v;
    strcpy(buffer3,w3->vie) ;
 
    char* str= (char *)malloc(sizeof(char) * MAX);
    sprintf(str,"%d/%d", game_result.total, num_of_ques);
    gtk_label_set_text(total_num, str);
    sprintf(str,"%d", game_result.correct_num);
    gtk_label_set_text(correct_num, str);
    
    gtk_label_set_text(question, w->eng );
    if (key_check == 1)
    {

        set_mean_textview_text(ans_1, w->vie);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else if (key_check == 2)
    {
        set_mean_textview_text(ans_2, w->vie);
        set_mean_textview_text(ans_1, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else if (key_check == 3)
    {
        set_mean_textview_text(ans_3, w->vie);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_1, buffer2);
        set_mean_textview_text(ans_4, buffer3);
    }
    else
    {
        set_mean_textview_text(ans_4, w->vie);
        set_mean_textview_text(ans_2, buffer1);
        set_mean_textview_text(ans_3, buffer2);
        set_mean_textview_text(ans_1, buffer3);
    }
    game_result.total++;
}

void new_record_result_of_game()
{
    game_result.total = 0;
    game_result.correct_num = 0;
}
void out_game()
{
    gtk_widget_hide(window_game);
    save_record_result_of_game();
    jrb_free_tree(game_tree);

}
void save_record_result_of_game()
{
    char line[MAX];
    char *buf = (char *)malloc(sizeof(char) * MAX);

    if ((f = fopen("../db/game_history.txt", "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(buf, "%s\n%s%d\n%s%d\n%s", asctime(tm), "Total: ", game_result.total, "Correct: ", game_result.correct_num, "-------------------------------");

    fprintf(f, "%s\n", buf);
    fclose(f);
}
void new_question_fix()
{

    int i = 0;

    is_answed(FALSE);
    note = btopn(note_path, 0, 0);
    btpos(note, ZSTART);
    int SIZE_OF_NOTE = 0;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, flag = 0;

    JRB game_tree = make_jrb();
    while (!btseln(note, eng, vie, MAX, &rsize))
    {
        i++;
        jrb_insert_int(game_tree, i, (Jval){.s = strdup(eng)});
    }

    if (i == 0)
    {
        btcls(note);
        jrb_free_tree(game_tree);
        set_mean_textview_text(textview4, "Danh sách từ khó trống, không thể chơi trò chơi");
        return -1;
    }
    else if (i > 0 && i < 4)
    {
        btcls(note);
        jrb_free_tree(game_tree);
        set_mean_textview_text(textview4, "Danh sách từ khó cần có 4 từ trở lên");
        return -1;
    }
    else
    {
        set_mean_textview_text(textview4, "");
    }

    time_t t;
    int key_word_correct = 0;
    int key_word2;
    int key_word3;
    char buffer1[MAX];
    char buffer2[MAX];
    char buffer3[MAX];
    srand((unsigned)time(&t));

    key_word_correct = rand() % i + 1;
    int key_word1 = key_word2 = key_word3 = key_word_correct;
    gtk_label_set_text(GTK_LABEL(lbl_eng), jrb_find_int(game_tree, key_word_correct)->val.s);
    btsel(note, jrb_find_int(game_tree, key_word_correct)->val.s, vie, MAX, &rsize);
    key_check = rand() % 4 + 1;
    while (key_word1 == key_word_correct)
    {
        key_word1 = rand() % i + 1;
    }
    while (key_word2 == key_word_correct || key_word2 == key_word1)
    {
        key_word2 = rand() % i + 1;
    }
    while (key_word3 == key_word_correct || key_word3 == key_word1 || key_word3 == key_word2)
    {
        key_word3 = rand() % i + 1;
    }
    btsel(note, jrb_find_int(game_tree, key_word1)->val.s, buffer1, MAX, &rsize);
    btsel(note, jrb_find_int(game_tree, key_word2)->val.s, buffer2, MAX, &rsize);
    btsel(note, jrb_find_int(game_tree, key_word3)->val.s, buffer3, MAX, &rsize);
    if (key_check == 1)
    {

        gtk_button_set_label(check_vie1, vie);
        gtk_button_set_label(check_vie2, buffer1);
        gtk_button_set_label(check_vie3, buffer2);
        gtk_button_set_label(check_vie4, buffer3);
    }
    else if (key_check == 2)
    {
        gtk_button_set_label(check_vie2, vie);
        gtk_button_set_label(check_vie1, buffer1);
        gtk_button_set_label(check_vie3, buffer2);
        gtk_button_set_label(check_vie4, buffer3);
    }
    else if (key_check == 3)
    {
        gtk_button_set_label(check_vie3, vie);
        gtk_button_set_label(check_vie2, buffer1);
        gtk_button_set_label(check_vie1, buffer2);
        gtk_button_set_label(check_vie4, buffer3);
    }
    else
    {
        gtk_button_set_label(check_vie4, vie);
        gtk_button_set_label(check_vie2, buffer1);
        gtk_button_set_label(check_vie3, buffer2);
        gtk_button_set_label(check_vie1, buffer3);
    }
    btcls(note);
    jrb_free_tree(game_tree);
    game_result.total++;
}
void is_answed(int bool)
{
    show_correct(bool);
    gtk_widget_set_sensitive(ans_1, !bool);
    gtk_widget_set_sensitive(ans_2, !bool);
    gtk_widget_set_sensitive(ans_3, !bool);
    gtk_widget_set_sensitive(ans_4, !bool);
}
void show_correct(int bool){
        GdkColor color;

        if(bool) gdk_color_parse("red", &color);
        else return hide_correct();

        if(key_check==1){
      gtk_widget_modify_fg( GTK_WIDGET(ans_1), GTK_STATE_NORMAL, &color );
        }
        else if (key_check==2)
        {
      gtk_widget_modify_fg( GTK_WIDGET(ans_2), GTK_STATE_NORMAL, &color );
            
        }
         else if (key_check==3)
        {
      gtk_widget_modify_fg( GTK_WIDGET(ans_3), GTK_STATE_NORMAL, &color );
            
        }
         else if (key_check==4)
        {
      gtk_widget_modify_fg( GTK_WIDGET(ans_4), GTK_STATE_NORMAL, &color );
            
        }
        
}
void hide_correct(){
    gtk_widget_modify_fg( GTK_WIDGET(ans_1), GTK_STATE_NORMAL, NULL );
    gtk_widget_modify_fg( GTK_WIDGET(ans_2), GTK_STATE_NORMAL, NULL );
    gtk_widget_modify_fg( GTK_WIDGET(ans_3), GTK_STATE_NORMAL, NULL );
    gtk_widget_modify_fg( GTK_WIDGET(ans_4), GTK_STATE_NORMAL, NULL );
}
void on_check_vie1_clicked(GtkButton *button)
{
    if (key_check == 1)
    {
        game_result.correct_num++;
    }
    else
        set_mean_textview_text(textview4, "Không chính xác");

    is_answed(TRUE);
}
void on_check_vie2_clicked(GtkButton *button)
{
    if (key_check == 2)
    {
        set_mean_textview_text(textview4, "Chính xác");
        game_result.correct_num++;
    }
    else
        set_mean_textview_text(textview4, "Không chính xác");

    is_answed(TRUE);
}
void on_check_vie3_clicked(GtkButton *button)
{
    if (key_check == 3)
    {
        set_mean_textview_text(textview4, "Chính xác");
        game_result.correct_num++;
    }
    else
        set_mean_textview_text(textview4, "Không chính xác");

    is_answed(TRUE);
}
void on_check_vie4_clicked(GtkButton *button)
{
    if (key_check == 4)
    {
        set_mean_textview_text(textview4, "Chính xác");
        game_result.correct_num++;
    }
    else
        set_mean_textview_text(textview4, "Không chính xác");

    is_answed(TRUE);
}
void show_game_his(GtkButton *button)
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file(ui_path);

    gtk_builder_connect_signals(builder, NULL);

    window_game_history = GTK_WIDGET(gtk_builder_get_object(builder, "window_game_history"));

    textview5 = GTK_WIDGET(gtk_builder_get_object(builder, "textview5"));

    char buffer[MAX];
    char line[MAX];
    if ((f = fopen("../db/game_history.txt", "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    while (fgets(line, MAX, f))
    {
        strcat(buffer, line);
    }
    fclose(f);

    set_mean_textview_text(textview5, buffer);
    g_object_unref(builder);
    gtk_widget_show(window_game_history);
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
    printf(his);
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
void delete_word_in_history(char *buf)
{
    // char line[MAX];
    // if ((f = fopen("../db/history.txt", "w")) == NULL)
    // {
    //     printf("Lỗi không thể mở file.\n");
    //     return -1;
    // }
    // while (fgets(line, MAX, f))
    // {
    //     printf("line\n");
    //     if(strcmp(buf, line)==0){
    //         fprintf(f, "%s", "");
    //     }
    // }
    // fclose(f);
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