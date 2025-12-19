
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DEF_ARRAY(_type, _name) \
typedef struct {                \
  int len;                      \
  _type* data;                  \
} _name;                        \

#define _DEF_ARRAY_NEW(_type, _name)     \
_name _name##_new(int len, _type* data)
#define _IMPL_ARRAY_NEW(_type, _name)         \
_DEF_ARRAY_NEW(_type, _name) {                \
  _name arr;                                  \
  arr.len = len;                              \
  arr.data = malloc(sizeof(_type) * arr.len); \
                                              \
  if(arr.data == NULL) {                      \
    arr.len = 0;                              \
    return arr;                               \
  }                                           \
                                              \
  memcpy(arr.data, data, sizeof(_type)*len);  \
  return arr;                                 \
}                                             \

#define _DEF_ARRAY_OF_SINGLE(_type, _name) \
_name _name##_of_single(_type num)
#define _IMPL_ARRAY_OF_SINGLE(_type, _name) \
_DEF_ARRAY_OF_SINGLE(_type, _name) {        \
  return _name##_new(1, &num);              \
}                                           \

#define _DEF_ARRAY_PUSH(_type, _name)       \
_name* _name##_push(_name *arr, _type elem)
#define _IMPL_ARRAY_PUSH(_type, _name)                      \
_DEF_ARRAY_PUSH(_type, _name) {                             \
  _type *new_data = malloc(sizeof(_type) * (arr->len + 1)); \
  memcpy(new_data, arr->data, arr->len * sizeof(_type));    \
  new_data[arr->len] = elem;                                \
  free(arr->data);                                          \
  arr->data = new_data;                                     \
  arr->len++;                                               \
  return arr;                                               \
}                                                           \


// MAYBE optimization (add max to struct and check if len is max on push to avoid realloc)
#define _DEF_ARRAY_POP(_type, _name)       \
_type* _name##_pop(_name *arr)
#define _IMPL_ARRAY_POP(_type, _name) \
_DEF_ARRAY_POP(_type, _name) {        \
  arr->len--;                         \
  return &arr->data[arr->len];        \
}                                     \

#define _DEF_ARRAY_CONCAT(_type, _name) \
_name _name##_concat(_name a, _name b)
#define _IMPL_ARRAY_CONCAT(_type, _name)                    \
_DEF_ARRAY_CONCAT(_type, _name) {                           \
  _name arr;                                                \
  arr = _name##_new(a.len + b.len, a.data);                 \
                                                            \
  int offset = a.len;                                       \
  if(arr.data == NULL) offset = 0;                          \
                                                            \
  memcpy(arr.data + offset, b.data, sizeof(_type) * b.len); \
                                                            \
  return arr;                                               \
}                                                           \

#define _DEF_ARRAY_FREE(_type, _name) \
void _name##_simple_free(_name *arr) 
#define _IMPL_ARRAY_FREE(_type, _name) \
_DEF_ARRAY_FREE(_type, _name) {        \
  if(arr->data == NULL) return;        \
  free(arr->data);                     \
  arr->data = NULL;                    \
  arr->len = 0;                        \
}                                      \

#define _DEF_ARRAY_PRINT(_type, _name)               \
void _name##_print(_name arr, void (*print)(_type))
#define _IMPL_ARRAY_PRINT(_type, _name)               \
_DEF_ARRAY_PRINT(_type, _name) {                      \
  printf(#_name" [ ");                                \
  for(int i = 0; i < arr.len; i++) {                  \
    print(arr.data[i]);                               \
    printf(",");                                      \
  }                                                   \
  printf(" ]");                                       \
}                                                     \

#define _DEF_ARRAY_EMPTY(_type, _name) \
_name _name##_empty()
#define _IMPL_ARRAY_EMPTY(_type, _name) \
_DEF_ARRAY_EMPTY(_type, _name){         \
  return _name##_new(0, NULL);          \
}                                       \

#define IMPL_ARRAY(_type, _name)    \
_IMPL_ARRAY_NEW(_type, _name)       \
_IMPL_ARRAY_OF_SINGLE(_type, _name) \
_IMPL_ARRAY_PRINT(_type, _name)     \
_IMPL_ARRAY_EMPTY(_type, _name)     \
_IMPL_ARRAY_CONCAT(_type, _name)    \
_IMPL_ARRAY_FREE(_type, _name)      \
_IMPL_ARRAY_PUSH(_type, _name)      \
_IMPL_ARRAY_POP(_type, _name)       \

#define DEF_ARRAY(_type, _name)     \
_DEF_ARRAY(_type, _name);           \
_DEF_ARRAY_NEW(_type, _name);       \
_DEF_ARRAY_OF_SINGLE(_type, _name); \
_DEF_ARRAY_PRINT(_type, _name);     \
_DEF_ARRAY_EMPTY(_type, _name);     \
_DEF_ARRAY_CONCAT(_type, _name);    \
_DEF_ARRAY_FREE(_type, _name);      \
_DEF_ARRAY_PUSH(_type, _name);      \
_DEF_ARRAY_POP(_type, _name);       \

