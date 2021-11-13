#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<order.h>

Bytes *serialize(Order *o)
{
	Bytes *b;
	char buffer[100];
	char s[50];
	char s1[10];
 
	sprintf(s,"%d",o->total);
    
	buffer[0]=strlen(o->name);
	strcat(buffer+1,o->name);
	sprintf(s1,"%d",strlen(s));
	strcat(buffer+buffer[0]+1,s1);
	strcat(buffer+buffer[0]+1+1,s);
    
	b->len=strlen(buffer);
	b->dat=(char*)malloc(sizeof(char)*100);
	strcpy(b->dat,buffer);
	return b;
}
Order *deserialize(Bytes *b)
{
	Order *o;
	int i,j,k;
	char buffer[100];
	i=atoi(b->dat[0]);
	for(k=1;k<i+1;k++)
	{
		strcat(o->name,b->dat[k]);
	}
	j=atoi(b->dat[i+1]);
	for(k=i+1;k<j+1+i+1;k++)
	{
		strcat(buffer,b->dat[k]);
	}
	o->total=atoi(buffer);
	return o;
}