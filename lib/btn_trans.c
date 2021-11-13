#include "header.h"
void on_btn_suggest1_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 1
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest1);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 1;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest2_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 2
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest2);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 2;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest3_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 3
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest3);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 3;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest4_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 4
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest4);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 4;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest5_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 5
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest5);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 5;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest6_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 6
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest6);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 6;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest7_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 7
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest7);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 7;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest8_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 8
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest8);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 8;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest9_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 9
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest9);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 9;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}
void on_btn_suggest10_clicked(GtkButton *button,app_widgets* app)// Nút dịch của từ gợi ý 10
{
    tudien = btopn("tudien.bt",0,0);
    char* gettext = gtk_label_get_text(app->lbl_suggest10);
    char buffer[MAX];
    char value[MAX];
    int rsize;
    check_his = 1;
    check_trans = 10;
    if(strcmp(buftrans,gettext)!=0)
        {
            strcpy(buftrans,gettext);
            strcpy(buffer,gettext);
            strcat(buffer,"\n");
            strcat(buffer,htr);
            strcpy(htr,buffer);
        }
    if(!btsel(tudien,gettext,value,MAX,&rsize)) 
        {
            gtk_label_set_text(GTK_LABEL(app->textview1), value);
        }
    btcls(tudien);
}