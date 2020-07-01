#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

/*
 * a: vector
 * b: backing store
 * x: item
 * i: index
 */

// Allocate a vector of the given capacity
static internal_vector_t* vector_alloc(int cap) {
	size_t sz = sizeof(internal_vector_t) + sizeof(void*) * cap;
	internal_vector_t *b = (internal_vector_t*) malloc(sz);
	memset(b, 0, sz);
	b->cap = cap;
	return b;
}

// Create a new vector
vector_t vector_new() {
	internal_vector_t *b = vector_alloc(16);
	// Use a second layer of indirection to let us swap out the backing store
	vector_t a = (vector_t) malloc(sizeof(internal_vector_t*));
	*a = b;
	return a;
}

// Deletes a vector (NOTE: does not attempt to free members)
void vector_delete(vector_t a) {
	internal_vector_t *b = *a;
	free(a);
	free(b);
}

unsigned int vector_get_len(vector_t a) {
	return (*a)->len;
}

// We don't let the user read past len, even if it's within cap
void* vector_get(vector_t a, unsigned int i) {
	internal_vector_t *b = *a;
	assert(i < b->len);
	return b->buf[i];
}

void* vector_get_last(vector_t a) {
	internal_vector_t *b = *a;
	assert(b->len >= 1);
	return b->buf[b->len-1];
}

void vector_set(vector_t a, unsigned int i, void* x) {
	internal_vector_t *b = *a;
	assert(i < b->len);
	b->buf[i] = x;
}

static internal_vector_t* vector_resize(internal_vector_t *b, unsigned int cap) {
	// Clone the old one into the new one
	internal_vector_t* b2 = vector_alloc(cap);
	b2->len = b->len;
	memcpy(b2->buf, b->buf, sizeof(void*) * b->len);
	
	// Clean the old one
	free(b);
	return b2;
}

// Add an item to the vector, reallocating if neccessary
void vector_add(vector_t a, void* x) {
	internal_vector_t *b = *a;
	
	// If the vector doesn't have room, we must resize it
	if (b->len >= b->cap)
		*a = b = vector_resize(b, b->cap * 2);
	
	b->buf[b->len] = x;
	b->len++;
}

// Remove an item from the vector
void vector_remove(vector_t a, unsigned int i) {
	internal_vector_t *b = *a;
	assert(i < b->len);
	
	memmove(&b->buf[i], &b->buf[i+1], sizeof(void*) * (b->len - i - 1));
	b->buf[b->len-1] = 0;
	b->len--;
	
	// If the vector is too big, we resize it here - must save memory
	if (b->len <= (b->cap / 3))
		*a = vector_resize(b, b->cap / 2);
}