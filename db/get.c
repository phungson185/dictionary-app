#include <stdio.h>
#include <string.h>

int main (){
    FILE *fp;
    char str[10000];
    char test[10000] = "@Lĩnh vực: y học\n";
    fp=fopen("AnhViet.txt","r");

    while(fgets(str,10000,fp)!=NULL){
        char *result = strstr(str, "@Lĩnh vực");
        if(result){
            char *token = strtok(result, "\\");
            if (token) {
                if(strstr(test, token) == NULL){
                    strcat(test, token);
                    strcat(test, "\n");
                }
            }
        }
        
    }
    printf("%s", test);
}