#include "extend_controller.h"

void extend()
{

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