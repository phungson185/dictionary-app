#include "question_controller.h"

void start()
{
    num_of_ques = atoi(gtk_entry_get_text(GTK_ENTRY(question_num)));

    if (num_of_ques > game_tree_size || !num_of_ques || num_of_ques <= 0)
    {
        show_message(window_question_filter, GTK_MESSAGE_ERROR, "ERROR", "Số lượng câu hỏi không hợp lệ");
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
    gtk_widget_destroy(window_question_filter);
}


void set_answers(char *vie1, char *vie2, char *vie3, char *vie4)
{
    set_mean_textview_text(ans_1, vie1);
    set_mean_textview_text(ans_2, vie2);
    set_mean_textview_text(ans_3, vie3);
    set_mean_textview_text(ans_4, vie4);
}

void new_question()
{
    checked = 0;
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

    is_answed(FALSE);

    if (game_result.total + 1 >= num_of_ques)
        gtk_widget_set_sensitive(btn_next, FALSE);

    game_result.total++;

    key_word_correct = ques_arr[game_result.total - 1];

    int key_word1 = key_word2 = key_word3 = key_word_correct;
    key_check = rand() % 4 + 1;
    while (key_word1 == key_word_correct)
        key_word1 = 1 + rand() % game_tree_size;
    while (key_word2 == key_word_correct || key_word2 == key_word1)
        key_word2 = 1 + rand() % game_tree_size;
    while (key_word3 == key_word_correct || key_word3 == key_word1 || key_word3 == key_word2)
        key_word3 = 1 + rand() % game_tree_size;

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

    switch (key_check)
    {
    case 1:
        set_answers(buffer_correct, buffer1, buffer2, buffer3);
        break;
    case 2:
        set_answers(buffer1, buffer_correct, buffer2, buffer3);
        break;
    case 3:
        set_answers(buffer1, buffer2, buffer_correct, buffer3);
        break;
    case 4:
        set_answers(buffer1, buffer2, buffer3, buffer_correct);
        break;

    default:
        break;
    }
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
    strcat(end_info, make_long_to_string(game_result.correct_num));
    strcat(end_info, "/");
    strcat(end_info, make_long_to_string(num_of_ques));
    show_message(window_main, GTK_MESSAGE_INFO, "KẾT THÚC", end_info);
    free(end_info);
    save_record_result_of_game();
}

void save_record_result_of_game()
{
    FILE *f;
    char end_time[30];
    char *buf = (char *)malloc(sizeof(char) * MAX);

    if ((f = fopen(game_history_path, "a")) == NULL)
    {
        printf("Lỗi không thể mở file với game_history_path.\n");
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
    if (!bool)
        return hide_correct();

    switch (checked)
    {
    case 1:
        gtk_widget_modify_fg(GTK_WIDGET(ans_1), GTK_STATE_NORMAL, &red);
        break;
    case 2:
        gtk_widget_modify_fg(GTK_WIDGET(ans_2), GTK_STATE_NORMAL, &red);
        break;
    case 3:
        gtk_widget_modify_fg(GTK_WIDGET(ans_3), GTK_STATE_NORMAL, &red);
        break;
    case 4:
        gtk_widget_modify_fg(GTK_WIDGET(ans_4), GTK_STATE_NORMAL, &red);
        break;

    default:
        break;
    }

    switch (key_check)
    {
    case 1:
        gtk_widget_modify_fg(GTK_WIDGET(ans_1), GTK_STATE_NORMAL, &green);
        break;
    case 2:
        gtk_widget_modify_fg(GTK_WIDGET(ans_2), GTK_STATE_NORMAL, &green);
        break;
    case 3:
        gtk_widget_modify_fg(GTK_WIDGET(ans_3), GTK_STATE_NORMAL, &green);
        break;
    case 4:
        gtk_widget_modify_fg(GTK_WIDGET(ans_4), GTK_STATE_NORMAL, &green);
        break;

    default:
        break;
    }
}

void hide_correct()
{
    gtk_widget_modify_fg(GTK_WIDGET(ans_1), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_2), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_3), GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_fg(GTK_WIDGET(ans_4), GTK_STATE_NORMAL, NULL);
}

void set_correct_num()
{
    char *str = (char *)malloc(sizeof(char) * 100);
    game_result.correct_num++;
    sprintf(str, "%d", game_result.correct_num);
    gtk_label_set_text(correct_num, str);
    free(str);
}

void on_check_vie1_clicked()
{
    if (key_check == 1)
        set_correct_num();
    checked = 1;
    is_answed(TRUE);
}

void on_check_vie2_clicked()
{
    if (key_check == 2)
        set_correct_num();
    checked = 2;
    is_answed(TRUE);
}

void on_check_vie3_clicked()
{
    if (key_check == 3)
        set_correct_num();
    checked = 3;
    is_answed(TRUE);
}

void on_check_vie4_clicked()
{
    if (key_check == 4)
        set_correct_num();
    checked = 4;
    is_answed(TRUE);
}