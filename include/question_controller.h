#ifndef _QUESTIONCONTROLLER_H_
#define _QUESTIONCONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <btree.h>
#include <stdlib.h>

#include "filter_controller.h"
#include "utils.h"

typedef struct game_result
{
    long total;
    int correct_num;
};
struct game_result game_result;

GtkWidget *window_game;
GtkWidget *ans_1, *ans_2, *ans_3, *ans_4;
GtkWidget *question, *total_num, *correct_num, *btn_next;

long num_of_ques;
long *ques_arr;

int key_check;
int checked;

void start();
void set_answers(char *vie1, char *vie2, char *vie3, char *vie4);
void new_question();
void new_record_result_of_game();
void out_game();
void save_record_result_of_game();
void is_answed(int bool);
void show_correct(int bool);
void hide_correct();
void set_correct_num();
void on_check_vie1_clicked();
void on_check_vie2_clicked();
void on_check_vie3_clicked();
void on_check_vie4_clicked();


#endif