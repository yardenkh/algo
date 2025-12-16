#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * Types
 * ============================================================ */

typedef enum { DA_ERROR = 0, DA_OK = 1 } DA_STATE;

typedef struct {
  void *data;
  size_t size;
  size_t capacity;
  size_t element_size;
} DA_array;

/* ============================================================
 * Core implementation (header-only)
 * ============================================================ */

static inline DA_STATE DA_init(DA_array *arr, size_t element_size) {
  if (!arr || element_size == 0)
    return DA_ERROR;

  arr->size = 0;
  arr->capacity = 2;
  arr->element_size = element_size;
  arr->data = malloc(arr->capacity * arr->element_size);

  if (!arr->data)
    return DA_ERROR;

  return DA_OK;
}

static inline DA_STATE DA_push(DA_array *arr, const void *element) {
  if (!arr || !element)
    return DA_ERROR;

  if (arr->size == arr->capacity) {
    size_t new_capacity = arr->capacity * 2;
    void *tmp = realloc(arr->data, new_capacity * arr->element_size);
    if (!tmp)
      return DA_ERROR;

    arr->data = tmp;
    arr->capacity = new_capacity;
  }

  memcpy((char *)arr->data + arr->size * arr->element_size, element,
         arr->element_size);

  arr->size++;
  return DA_OK;
}

static inline DA_STATE DA_pop(DA_array *arr, void *out_element) {
  if (!arr || arr->size == 0)
    return DA_ERROR;

  arr->size--;

  if (out_element) {
    memcpy(out_element, (char *)arr->data + arr->size * arr->element_size,
           arr->element_size);
  }

  return DA_OK;
}

static inline DA_STATE DA_peek(const DA_array *arr, void *out_element) {
  if (!arr || arr->size == 0 || !out_element)
    return DA_ERROR;

  memcpy(out_element, (char *)arr->data + (arr->size - 1) * arr->element_size,
         arr->element_size);

  return DA_OK;
}

static inline DA_STATE DA_get(const DA_array *arr, size_t index,
                              void *out_element) {
  if (!arr || !out_element || index >= arr->size)
    return DA_ERROR;

  memcpy(out_element, (char *)arr->data + index * arr->element_size,
         arr->element_size);

  return DA_OK;
}

static inline size_t DA_size(const DA_array *arr) {
  return arr ? arr->size : 0;
}

static inline int DA_is_empty(const DA_array *arr) {
  return !arr || arr->size == 0;
}

static inline void DA_clear(DA_array *arr) {
  if (arr)
    arr->size = 0;
}

static inline void DA_free(DA_array *arr) {
  if (!arr)
    return;

  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
  arr->element_size = 0;
}

/* ============================================================
 * Convenience macros (honest, simple)
 * ============================================================ */

/* Declare + init */
#define DA_INIT(arr, type)                                                     \
  DA_array arr;                                                                \
  DA_init(&(arr), sizeof(type))

/* Typed helpers (explicit & by design) */
#define DA_PUSH_T(arr, type, value)                                            \
  do {                                                                         \
    type _tmp = (value);                                                       \
    DA_push(&(arr), &_tmp);                                                    \
  } while (0)

#define DA_POP_T(arr, type, out_ptr) DA_pop(&(arr), (out_ptr))

#define DA_PEEK_T(arr, type, out_ptr) DA_peek(&(arr), (out_ptr))

#define DA_GET_T(arr, type, idx, out_ptr) DA_get(&(arr), (idx), (out_ptr))

#define DA_FREE(arr) DA_free(&(arr))

#endif /* DYNAMIC_ARRAY_H */
