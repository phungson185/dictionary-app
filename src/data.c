#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<btree.h>
#define MAX 1000
char key[MAX];
char eng[MAX];
char vie[MAX];
void separate(char str[])
{
	strcpy(eng,strtok(str,"_"));
	strcpy(vie,strtok(0,"\n"));
}
int main()
{
	btinit();
	BTA* tudien;
	BTA* diff;
	FILE *p;
	diff = btcrt("difficult_word.bt",0,0);
	tudien = btcrt("tudien.bt",0,0);
	if((p=fopen("../src/tudien.txt","r"))==NULL)
		{
			printf("Loi khong the mo file.\n");
			return -1;
		}
	while(fgets(key,MAX,p))
	{
		for(int i =0;i<strlen(key);i++)
			{
				if(key[i] =='_')
				{
					separate(key);
					btins(tudien,eng,vie,MAX);
				}
			}
	}
	fclose(p);
	btcls(diff);
	btcls(tudien);
}