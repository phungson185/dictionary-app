#include "main_controller.h"

int main(int argc, char *argv[])
{
    get_history();
    gdk_color_parse("#f6465d", &red);
    gdk_color_parse("#0ecb81", &green);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file(ui_path);
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

    searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));

    textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    textview_his = GTK_WIDGET(gtk_builder_get_object(builder, "textview_his"));
    combo = GTK_WIDGET(gtk_builder_get_object(builder, "combo"));

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    if (!gtk_css_provider_load_from_file(provider, g_file_new_for_path("../ui/styles.css"), &error))
    {
        return -1;
    }

    set_mean_textview_text(textview_his, his);

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    list = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(list));
    gtk_entry_set_completion(GTK_ENTRY(searchentry), comple);

    store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);

    gtk_widget_set_visible(combo, FALSE);

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
        set_mean_textview_text(textview1, "");
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

void translate()
{
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
        if (btsel(dict, gettext, translate_value, MAX, &rsize))
            show_message(window_main, GTK_MESSAGE_ERROR, "ERROR!", "Không tìm thấy từ bạn cần tìm");
        else
        {
            set_mean_textview_text(textview1, translate_value);
            search_history_handler(gettext);
        }
    }
    set_mean_textview_text(textview_his, his);

    search_result = NULL;
    gtk_widget_set_visible(combo, TRUE);

    split_result(strdup(translate_value));
}

Option *make_option(char *eng, char *vie)
{
    Option *option = (Option *)malloc(sizeof(Option));
    option->option = strdup(eng);
    option->mean = strdup(vie);
    return option;
}

void split_result(char *s)
{
    search_result = make_jrb();
    gtk_list_store_clear(store);
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, "Tất cả", -1);

    char *ptr, *p;
    char *op;
    int count = 1;
    for (int i = 0; i < 26; i++)
    {
        char *str = strdup(s);
        if ((ptr = strstr(str, word_type[i])) != NULL)
        {
            op = strsep(&ptr, "\n");

            if (strchr(ptr, '*') != NULL)
            {
                p = strsep(&ptr, "*");
                if (strlen(p) > 0)
                {
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter, 0, op, -1);
                    jrb_insert_int(search_result, count++, (Jval){.v = make_option(op, p)});
                }
            }
            else if (strchr(ptr, '@') != NULL)
            {
                p = strsep(&ptr, "@");
                if (strlen(p) > 0)
                {
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter, 0, op, -1);
                    jrb_insert_int(search_result, count++, (Jval){.v = make_option(op, p)});
                }
            }
            else
            {
                p = strsep(&ptr, "\0");
                if (strlen(p) > 0)
                {
                    gtk_list_store_append(store, &iter);
                    gtk_list_store_set(store, &iter, 0, op, -1);
                    jrb_insert_int(search_result, count++, (Jval){.v = make_option(op, p)});
                }
            }
        }
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}

void on_combo_changed()
{

    int id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));

    if (!id)
        set_mean_textview_text(textview1, translate_value);
    else if (id > 0)
    {
        Option *o = (Option *)(jrb_find_int(search_result, id)->val.v);
        set_mean_textview_text(textview1, o->mean);
    }
}

void clear_history()
{
    strcpy(his, "");
    set_mean_textview_text(textview_his, his);
    fclose(fopen(history_path, "w"));
    show_message(window_main, GTK_MESSAGE_INFO, "SUCCESS!", "Xóa lịch sử tra từ thành công.");
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

void get_history()
{
    char buffer[MAX];
    char line[MAX];
    FILE *f;
    if ((f = fopen("../db/history.txt", "r")) == NULL)
    {
        printf("Lỗi không thể mở file với history_path.\n");
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
    fclose(f);
}
void add_to_history(char *buf)
{
    FILE *f;
    if ((f = fopen(history_path, "a")) == NULL)
    {
        printf("Lỗi không thể mở file với history_path.\n");
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