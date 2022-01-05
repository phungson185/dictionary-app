#ifndef _EXTENDCONTROLLER_H_
#define _EXTENDCONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <btree.h>
#include <stdlib.h>

#include "utils.h"
#include "main_controller.h"

GtkWidget *window_advanced, *entry_newword, *entry_meanword, *entry_del, *textview2;

void extend();
void add_to_dict();
void repair_word();
void delete_from_dict();

#endif