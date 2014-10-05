#ifndef HE_INTEGRATOR_H
#define HE_INTEGRATOR_H
#include "HE_Task.h"

void HE_Integrator_Implicit(HE_Task* task, HES_Status* stat);
void HE_Integrator_Explicit(HE_Task* task, HES_Status* stat);

#endif
