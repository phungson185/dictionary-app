#include "utils.h"
#include "main_controller.h"
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

char *make_long_to_string(long l)
{
    char *s = (char *)malloc(sizeof(char) * 10);
    sprintf(s, "%ld", l);
    return s;
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

GtkTextBuffer* clear_textview(GtkWidget *textview)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
    return buffer;
}