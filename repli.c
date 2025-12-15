#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _IMPL_ARRAY(_type, _name) \
typedef struct {                  \
	int len;                        \
	_type* data;                    \
} _name;                          \
\
_name _name##_new(int len, _type* data) {     \
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
\
_name _name##_of_single(_type num) { \
	return _name##_new(1, &num);       \
}                                    \
\
_name _name##_concat(_name a, _name b) {                    \
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
                                                            \
void _name##_free(_name arr) {                              \
	if(arr.data == NULL) return;                              \
	free(arr.data);                                           \
}                                                           \

_IMPL_ARRAY(int, Ints)

int main(void) {
	int from[3] = {1,2,3};
	int empty[0] = {};
	Ints arr1 = Ints_new(3, from);
	Ints arr2 = Ints_new(0, empty);

	Ints arr3 = Ints_of_single(atoi("8"));

	Ints arr = Ints_concat(arr3, arr3);

	for (int i = 0; i < arr.len; i++) {
		printf("%d, %d\n", i, arr.data[i]);
	}

	Ints_free(arr1);
	Ints_free(arr2);
	Ints_free(arr);

	return 0;
}
