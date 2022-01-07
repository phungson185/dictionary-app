#include "filter_controller.h"

void on_btn_game_clicked()
{

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
    query_result = GTK_WIDGET(gtk_builder_get_object(builder, "query_result"));
    btn_start = GTK_WIDGET(gtk_builder_get_object(builder, "btn_start"));

    gtk_widget_set_sensitive(btn_start, FALSE);
    gtk_widget_set_sensitive(question_num, FALSE);

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
}

void insert_game_tree(BTA *source, char filter[][50])
{
    long i = 0;
    char *start;
    char *op;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    char *mean = (char *)malloc(sizeof(char) * MAX);

    int rsize, check_lv = 0;

    btpos(source, ZSTART);
    while (!btseln(source, eng, vie, MAX, &rsize))
    {
        strcpy(mean, "");
        check_lv = 0;

        for (int j = 0; j < 10; j++)
        {
            if (strlen(filter[j]) > 0)
            {
                if (start = strstr(vie, filter[j]))
                {
                    check_lv++;

                    strsep(&start, "\n");
                    if (strchr(start, '*') != NULL)
                    {
                        strcat(mean, filter[j]);
                        strcat(mean, strsep(&start, "*"));
                        strcat(mean, "\n");
                    }
                    else if (strchr(start, '@') != NULL)
                    {
                        strcat(mean, filter[j]);
                        strcat(mean, strsep(&start, "@"));
                        strcat(mean, "\n");
                    }
                    else
                    {
                        strcat(mean, filter[j]);
                        strcat(mean, strsep(&start, "\0"));
                        strcat(mean, "\n");
                    }

                    // if (start = strtok(start, "@"))
                    // {
                    //     strcat(mean, "@");
                    //     strcat(mean, start);
                    //     strcat(mean, "\n");
                    // }
                    // else if (start = strtok(0, "\n"))
                    // {
                    //     strcat(mean, "@");
                    //     strcat(mean, start);
                    //     strcat(mean, "\n");
                    // }
                }
            }
        }
        if (strlen(mean) > 0 && check_lv == lv_num)
        {
            i++;
            strcat(eng, "^");
            strcat(eng, mean);
            btins(question_tree, make_long_to_string(i), eng, strlen(eng) + 1);
        }
    }
    game_tree_size = i;
    printf("game size: %ld\n", game_tree_size);
    btcls(note);
    btcls(question_tree);
}

void query()
{
    get_filter();

    question_tree = btcrt(question_path, 0, 0);
    note = btopn(note_path, 0, 0);

    char filter[10][50];
    lv_num = 0;
    memset(filter, '\0', sizeof(filter));

    int index_filter = 0;
    if (y_hoc)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: y học\n");
        lv_num++;
    }
    if (toan_tin)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: toán & tin\n");
        lv_num++;
    }
    if (dtvt)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: điện tử & viễn thông\n");
        lv_num++;
    }
    if (xay_dung)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: xây dựng\n");
        lv_num++;
    }
    if (dien_lanh)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: điện lạnh\n");
        lv_num++;
    }
    if (hh_vat_lieu)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: hóa học & vật liệu\n");
        lv_num++;
    }
    if (dien)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: điện\n");
        lv_num++;
    }
    if (ckct)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: cơ khí & công trình\n");
        lv_num++;
    }
    if (ky_thuat)
    {
        strcpy(filter[8], "@Chuyên ngành kỹ thuật\n");
        lv_num++;
    }
    if (kinh_te)
    {
        strcpy(filter[index_filter++], "@Chuyên ngành kinh tế\n");
        lv_num++;
    }
    if (filter_ghi_chu)
        insert_game_tree(note, filter);
    else
        insert_game_tree(dict, filter);

    if (game_tree_size == 0)
    {
        show_message(window_question_filter, GTK_MESSAGE_ERROR, "ERROR", "Không có câu hỏi nào được tìm thấy");
        gtk_label_set_text(query_result, "");
        return;
    }
    else if (game_tree_size > 0 && game_tree_size < 4)
    {
        show_message(window_question_filter, GTK_MESSAGE_ERROR, "ERROR", "Số lượng câu hỏi không đủ để bắt đầu (cần có nhiều hơn 3 câu hỏi)");
        gtk_label_set_text(query_result, "");
        return;
    }

    char *query_result_notif = (char *)malloc(sizeof(char) * 100);
    sprintf(query_result_notif, "Có %ld câu hỏi được tìm thấy", game_tree_size);
    gtk_label_set_text(query_result, query_result_notif);
    free(query_result_notif);

    gtk_widget_set_sensitive(btn_start, TRUE);
    gtk_widget_set_sensitive(question_num, TRUE);
}

void show_game_his()
{

    builder = gtk_builder_new_from_file(ui_path);

    gtk_builder_connect_signals(builder, NULL);

    window_game_history = GTK_WIDGET(gtk_builder_get_object(builder, "window_game_history"));

    textview5 = GTK_WIDGET(gtk_builder_get_object(builder, "textview5"));

    GtkTextBuffer *buffer;
    GtkTextIter iter;

    buffer = clear_textview(textview5);
    gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "red", NULL);
    gtk_text_buffer_create_tag(buffer, "yellow_fg", "foreground", "yellow", NULL);
    gtk_text_buffer_create_tag(buffer, "green_fg", "foreground", "green", NULL);
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

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

    if ((fd = fopen(game_history_path, "r")) == NULL)
    {
        printf("Lỗi không thể mở file với game_history_path.\n");
        return;
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
        for (i = count - 1; i >= 0 && count > 0; i--)
            buf[index++] = len[i];
        buf[index] = '\0';
        puts(buf);
        get_end_time = strtok(buf, "-");
        get_correct_num = strtok(NULL, "-");
        get_game_tree_size = strtok(NULL, "-");
        sprintf(line, "%-30s%-10s%-5s\n", get_end_time, get_correct_num, get_game_tree_size);
        float lv = (atof(get_correct_num) / atof(get_game_tree_size)) * 10;
        if (lv >= 0 && lv <= 3)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "red_fg", NULL);
        else if (lv >= 4 && lv <= 6)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "yellow_fg", NULL);
        else if (lv >= 7 && lv <= 10)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "green_fg", NULL);

        index = 0;
        memset(buf, '\0', 100);
    }

    fclose(fd);
    g_object_unref(builder);
    gtk_widget_show(window_game_history);
}

void delelte_all_game_history()
{
    FILE *fd;
    if ((fd = fopen(game_history_path, "w")) == NULL)
    {
        printf("Lỗi không thể mở file với game_history_path.\n");
        return -1;
    }
    fclose(fd);
    set_mean_textview_text(textview5, "Danh sách trống !");
}