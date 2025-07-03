#ifndef MAX86141_SPO2_H
#define MAX86141_SPO2_H

#include "MAX86141.h"

#include<Math.h>

class MAX86141_SpO2{
    public:
        MAX86141_SpO2();

        void initialize();
        void configure();
        float getSpO2();

}

#endif

