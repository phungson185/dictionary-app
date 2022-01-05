#ifndef _UTILS_H_
#define _UTILS_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

void set_mean_textview_text(GtkWidget *textview, char *text);
void show_message(GtkWidget *parent, GtkMessageType type, char *mms, char *content);
char *make_long_to_string(long l);
void randomize(long arr[], long n);
GtkTextBuffer* clear_textview(GtkWidget *textview);
#endif