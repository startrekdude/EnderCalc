#ifndef _INCLUDED_VECTOR_H
#define _INCLUDED_VECTOR_H

typedef struct {
	unsigned int cap; // capacity of buffer
	unsigned int len; // number of elements actually in list
	void* buf[]; // the backing store
} internal_vector_t;

typedef internal_vector_t **vector_t; // use ** so we can resize it w/o caller intervention

vector_t vector_new();
void vector_delete(vector_t a);

unsigned int vector_get_len(vector_t a);

void* vector_get(vector_t a, unsigned int i);
void* vector_get_last(vector_t a);
void vector_set(vector_t a, unsigned int i, void* x);

void vector_add(vector_t a, void* x);
void vector_remove(vector_t a, unsigned int x);

#endif