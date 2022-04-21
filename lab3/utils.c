
#include "utils.h"

int util_sys_inb (int port, uint8_t* value){
    uint32_t aux;

    if(sys_inb(port, &aux)){
        return 1;
    }

    *value = (uint8_t)aux;

    return 0;
}
