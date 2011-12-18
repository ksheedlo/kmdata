/*Ken Sheedlo
 * Error handling implementation */

#include "error_handling.h"

void CriticalError(char *str){
    fprintf(stderr, "%s\n", str);
}

