
#include "types.h"

Ints dispatchBuiltin(char* name, Ints data);

Ints __forward();
Ints __turnRight();
Ints __turnLeft();
Ints __up();
Ints __down();
Ints __setBlock(Ints);
// Ints __readBlock();

void __parse(char*);
