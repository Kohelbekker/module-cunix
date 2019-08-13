	#include "hash.h"
	#include "linked_list.c"
	#include "linked_list.h"
	#include <stdlib.h>
	#include <stdio.h>
	
	unsigned int hash_func(char *key)
		{
	    int hash=0;
	    int i=0;
		  if (key == NULL)
		    return 0;
		  while (key[i] != '\0')
		  {
		    hash += key[i];
		    i++;
		  }
		  return hash;
		}

hashtable_t *hash_create(unsigned int size)
	{
	  hashtable_t   *node_n;
	  if(size > 0)
	  {
	    node_n = (hashtable_t *) malloc(sizeof(hashtable_t));
	    node_n->size = size;
	    node_n->table = (void **) malloc(size * sizeof(void *));
	    for(unsigned int i = 0; i < size; i++)
	      node_n->table[i] = NULL;
	    return node_n;
	  }
	  return NULL;
	}	
	
	void            hash_destroy(hashtable_t *ht)
	{
	  node_t *node;
	  unsigned int i=0;
	  while(i<ht->size)
	  {
	    if (ht->table[i]!=NULL)
	        {
	          node=ht->table[i];
	          list_destroy(&node);
	        }
	    i++;
	  }
	  free(ht->table);
	  ht->table=NULL;
	  free(ht);
	}
	
	void            hash_set(hashtable_t *ht, char *key, char *value)
	{
	  int index;

	  index=hash_func(key)%ht->size;
	  
	  printf("set index = %d\n", index); ///
      if (ht->table[index] != NULL)
	  {
        list_print(ht->table[index]);  
	    list_push(ht->table[index],key,value);
	  }
	  else
	  {
	    ht->table[index]=list_create(key, value);
      }

      /////////// DELETE! //////
      node_t *node;

      node = ht->table[index];
    hash_get(ht, node->key);
    ////////////////
	}

	void            *hash_get(hashtable_t *ht, char *key)
	{
	  int index;
	  node_t *node; 

      printf("START GET: key %s \n", key);
      index = hash_func(key) %  ht->size;
      printf("index = %d | hash_function = %d | hash_size = %d\n", index, hash_func(key), ht->size);
      if(ht->table[index]==NULL )
	  {
	    return NULL;
	  }
	  else
	  {
	node=ht->table[index];
	printf("HASH GET: key %s | value %s\n", node->key, node->value);
    //list_print_pair(node,key);
	//printf("Index: %d\n", index);
	return (node->value);
	  }
	}

