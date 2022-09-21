
#include "utils.h"

int util_sys_inb (int port, uint8_t* byte){
    uint32_t aux;

    if(sys_inb(port, &aux)){
        return 1;
    }

    *byte = (uint8_t)aux;

    return 0;
}
