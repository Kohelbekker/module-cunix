#include "../include/hash.h"
#include "../include/linked_list.h"
#include <stdlib.h>

node_t      *list_create(void *data)
{
    node_t *list;

    list = (node_t *)malloc(sizeof(node_t));
    if (data != NULL)
        list->data = data;
    else
        list->data = NULL;
    list->next = NULL;
    return (list);
}

void    list_push(node_t *head, void *data)
{
    node_t *tmp;

    if (head == NULL)
        return ;
    tmp = head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = (node_t *)malloc(sizeof(node_t));
    tmp = tmp->next;
    tmp->data = malloc(sizeof(data));
    tmp->data = data;
    tmp->next = NULL;
}

hashtable_t     *hash_create(unsigned int size)
{
    hashtable_t *hash;
    
    if (size < 0)
        return (NULL);
    hash = (hashtable_t *)malloc(sizeof(hashtable_t));
    hash->table = (void **)malloc(size * sizeof(hash->table));
    hash->size = size;
    return (hash);
}

void            hash_destroy(hashtable_t *ht, void (*fp)(void *data))
{
    int         i;

    i = 0;
    while (i != ht->size)
    {
        fp(ht->table[i]);
        i++;
    }
    free(ht->table);
    free(ht);
}

unsigned int    hash_func(char *key)
{
    int         i;
    int         hash;

    i = 0;
    hash = 0;
    if (key == NULL)
        return (0);
    while (key[i])
    {
        hash += (key[i] % 10 + 35) - i;
        i++;
    }
    return (hash);
}

void            hash_set(hashtable_t *ht, char *key, void *ptr, void (*fp)(void *data))
{
    int hash;

    hash = hash_func(key);
    if (ht->table[hash])
        list_push(ht->table[hash], ptr);
    else
        ht->table[hash] = list_create(ptr);    
    fp((void *)key);

    
}

void            *hash_get(hashtable_t *ht, char *key)
{
    int     hash;
    node_t  *list;

    hash = hash_func(key) % ht->size; // mb remove %
    if (ht->table[hash] == NULL)
        return (NULL);
    list = ht->table[hash];
    return (list);
}

