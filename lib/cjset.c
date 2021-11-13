#include <jrb.h>
#include <stdio.h>
#include <cjset.h>
int size = 0;
int jset_insert_str(JRB tree, char *key, Jval val){
    char* p =(char*)malloc(strlen(key)+1);
    strcpy(p,key);
  if(jrb_find_str(tree, p)==NULL)
  {
      jrb_insert_str(tree,p,val);
      size++;
      return 1;
  }
  return 0;
}
int jset_update_str(JRB tree, char *key, Jval val){
    JRB p = jrb_find_str(tree,key);
    if(p!=NULL)
        {
            p->val = val;
            return 1;
        }
    return 0;
}
int jset_size(JRB tree){
    return size;
}
