#ifndef _FILTERCONTROLLER_H_
#define _FILTERCONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <btree.h>
#include <stdlib.h>

#include "main_controller.h"
#include "note_controller.h"
#include "utils.h"

BTA *question_tree;

GtkWidget *window_question_filter, *window_game_history, *textview5, *question_num, *query_result, *btn_start;
GtkWidget *lv_y_hoc, *lv_toan_tin, *lv_dtvt, *lv_xay_dung, *lv_dien_lanh, *lv_hh_vat_lieu, *lv_dien, *lv_ckct, *cn_kinh_te, *cn_ky_thuat, *tu_ghi_chu;

int y_hoc, toan_tin, dtvt, xay_dung, dien_lanh, hh_vat_lieu, dien, ckct;
int kinh_te, ky_thuat, filter_ghi_chu;

long game_tree_size, lv_num;

void on_btn_game_clicked();
void get_filter();
void insert_game_tree(BTA *source, char filter[][50]);
void query();
void show_game_his();
void delelte_all_game_history();
void get_history();
void add_to_history(char *buf);
void rewrite_history(char *his);
void search_history_handler(char *gettext);
int strremove(char *str, char *sub);

#endif
