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
    lv_oto = GTK_WIDGET(gtk_builder_get_object(builder, "lv_oto"));
    lv_detmay = GTK_WIDGET(gtk_builder_get_object(builder, "lv_detmay"));
    lv_thucpham = GTK_WIDGET(gtk_builder_get_object(builder, "lv_thucpham"));
    lv_moitruong = GTK_WIDGET(gtk_builder_get_object(builder, "lv_moitruong"));
    lv_vatly = GTK_WIDGET(gtk_builder_get_object(builder, "lv_vatly"));
    lv_gtvt = GTK_WIDGET(gtk_builder_get_object(builder, "lv_gtvt"));

    lt_danhtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_danhtu"));
    lt_dongtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_dongtu"));
    lt_tinhtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_tinhtu"));
    lt_trangtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_trangtu"));
    lt_daitu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_daitu"));
    lt_thantu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_thantu"));
    lt_gioitu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_gioitu"));
    lt_lientu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_lientu"));
    lt_noidongtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_noidongtu"));
    lt_ngoaidongtu = GTK_WIDGET(gtk_builder_get_object(builder, "lt_ngoaidongtu"));

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
    oto = gtk_toggle_button_get_active(lv_oto);
    detmay = gtk_toggle_button_get_active(lv_detmay);
    thucpham = gtk_toggle_button_get_active(lv_thucpham);
    moitruong = gtk_toggle_button_get_active(lv_moitruong);
    vatly = gtk_toggle_button_get_active(lv_vatly);
    gtvt = gtk_toggle_button_get_active(lv_gtvt);

    danhtu = gtk_toggle_button_get_active(lt_danhtu);
    dongtu = gtk_toggle_button_get_active(lt_dongtu);
    tinhtu = gtk_toggle_button_get_active(lt_tinhtu);
    trangtu = gtk_toggle_button_get_active(lt_trangtu);
    daitu = gtk_toggle_button_get_active(lt_daitu);
    thantu = gtk_toggle_button_get_active(lt_thantu);
    gioitu = gtk_toggle_button_get_active(lt_gioitu);
    lientu = gtk_toggle_button_get_active(lt_lientu);
    noidongtu = gtk_toggle_button_get_active(lt_noidongtu);
    ngoaidongtu = gtk_toggle_button_get_active(lt_ngoaidongtu);

    kinh_te = gtk_toggle_button_get_active(cn_kinh_te);
    ky_thuat = gtk_toggle_button_get_active(cn_ky_thuat);
    filter_ghi_chu = gtk_toggle_button_get_active(tu_ghi_chu);
}

void insert_game_tree(BTA *source, char filter[][50])
{
    long i = 0;
    char *start;
    char *op;
    char *p, *ptr;
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    char *mean = (char *)malloc(sizeof(char) * MAX);

    int rsize, check_lv = 0;
    btpos(source, ZSTART);
    while (!btseln(source, eng, vie, MAX, &rsize))
    {
        strcpy(mean, "");
        check_lv = 0;
        for (int j = 0; j < 26; j++)
        {
            if (strlen(filter[j]) > 0)
            {
                char *str = strdup(vie);
                if (start = strstr(str, filter[j]))
                {

                    strsep(&start, "\n");
                    if (strchr(start, '*') != NULL)
                    {
                        ptr = strsep(&start, "*");
                        if (strlen(ptr) > 0)
                        {
                            strcat(mean, filter[j]);
                            strcat(mean, ptr);
                            strcat(mean, "\n");
                            check_lv++;
                        }
                    }
                    else if (strchr(start, '@') != NULL)
                    {
                        ptr = strsep(&start, "@");
                        if (strlen(ptr) > 0)
                        {
                            strcat(mean, filter[j]);
                            strcat(mean, ptr);
                            strcat(mean, "\n");
                            check_lv++;
                        }
                    }
                    else
                    {
                        ptr = strsep(&start, "\0");
                        if (strlen(ptr) > 0)
                        {
                            strcat(mean, filter[j]);
                            strcat(mean, ptr);
                            strcat(mean, "\n");
                            check_lv++;
                        }
                    }
                }
            }
        }
        if ((strlen(mean) > 0 && check_lv == lv_num) || (filter_ghi_chu && !strlen(filter[0])))
        {
            i++;
            if (filter_ghi_chu && !strlen(filter[0]))
            {
                if (p = strstr(vie, eng))
                {
                    strsep(&p, "\n");
                    strcat(eng, "^");
                    strcat(eng, p);
                }
                else
                {
                    strcat(eng, "^");
                    strcat(eng, vie);
                }
            }
            else
            {
                strcat(eng, "^");
                strcat(eng, mean);
            }
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

    char filter[26][50];
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
    if (oto)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: ô tô\n");
        lv_num++;
    }
    if (detmay)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: dệt may\n");
        lv_num++;
    }
    if (thucpham)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: thực phẩm\n");
        lv_num++;
    }
    if (vatly)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: vật lý\n");
        lv_num++;
    }
    if (moitruong)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: môi trường\n");
        lv_num++;
    }
    if (gtvt)
    {
        strcpy(filter[index_filter++], "@Lĩnh vực: giao thông & vận tải\n");
        lv_num++;
    }
    if (danhtu)
    {
        strcpy(filter[index_filter++], "danh từ\n");
        lv_num++;
    }
    if (dongtu)
    {
        strcpy(filter[index_filter++], "động từ\n");
        lv_num++;
    }
    if (tinhtu)
    {
        strcpy(filter[index_filter++], "tính từ\n");
        lv_num++;
    }
    if (trangtu)
    {
        strcpy(filter[index_filter++], "trạng từ\n");
        lv_num++;
    }
    if (gioitu)
    {
        strcpy(filter[index_filter++], "giới từ\n");
        lv_num++;
    }
    if (daitu)
    {
        strcpy(filter[index_filter++], "đại từ\n");
        lv_num++;
    }
    if (lientu)
    {
        strcpy(filter[index_filter++], "liên từ\n");
        lv_num++;
    }
    if (thantu)
    {
        strcpy(filter[index_filter++], "thán từ\n");
        lv_num++;
    }
    if (noidongtu)
    {
        strcpy(filter[index_filter++], "nội động từ\n");
        lv_num++;
    }
    if (ngoaidongtu)
    {
        strcpy(filter[index_filter++], "ngoại động từ\n");
        lv_num++;
    }
    if (ky_thuat)
    {
        strcpy(filter[index_filter++], "@Chuyên ngành kỹ thuật\n");
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
    gtk_text_buffer_create_tag(buffer, "red_fg", "foreground", "#ff5c5c", NULL);
    gtk_text_buffer_create_tag(buffer, "yellow_fg", "foreground", "#f8bb32", NULL);
    gtk_text_buffer_create_tag(buffer, "green_fg", "foreground", "#40ad87", NULL);
    gtk_text_buffer_create_tag(buffer, "greeen_fg", "foreground", "blue", NULL);
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
        get_end_time = strtok(&buf, "-");
        get_correct_num = strtok(NULL, "-");
        get_game_tree_size = strtok(NULL, "-");
        sprintf(line, "%-30s%-10s%-5s\n", get_end_time, get_correct_num, get_game_tree_size);
        float lv = (atof(get_correct_num) / atof(get_game_tree_size)) * 10;
        if (lv >= 0 && lv < 5)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "red_fg", NULL);
        else if (lv >= 5 && lv < 8)
            gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, line, -1, "yellow_fg", NULL);
        else
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