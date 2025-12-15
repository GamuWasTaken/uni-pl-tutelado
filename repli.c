#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO make a template for generics, so we can reuse this :)
typedef struct {
	int len;
	int* data;
} Array;

Array array_new(int len, int* data) {
	Array arr;
	arr.len = len;
	arr.data = malloc(sizeof(int) * arr.len);

	if(arr.data == NULL) {
		arr.len = 0;
		return arr;
	}

	memcpy(arr.data, data, sizeof(int)*len);
	return arr;
}

Array array_of_int(int num) {
	return array_new(1, &num);
}

Array array_concat(Array a, Array b) {
	Array arr;
	arr = array_new(a.len + b.len, a.data);

	int offset = a.len;
	if(arr.data == NULL) offset = 0;
	
	memcpy(arr.data + offset, b.data, sizeof(int) * b.len);
	
	return arr;
}

void array_free(Array arr) {
	if(arr.data == NULL) return;
	free(arr.data);
}

int main(void) {
	int from[3] = {1,2,3};
	int empty[0] = {};
	Array arr1 = array_new(3, from);
	Array arr2 = array_new(0, empty);

	Array arr3 = array_of_int(atoi("8"));

	Array arr = array_concat(arr3, arr3);

	for (int i = 0; i < arr.len; i++) {
		printf("%d, %d\n", i, arr.data[i]);
	}

	array_free(arr1);
	array_free(arr2);
	array_free(arr);

	return 0;
}
