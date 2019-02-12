#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Definitions */

#define PowerActivated 1
#define PowerDeactivated 2

#define Testing 1
#define Implementation 2

/* Queue Handles*/

/* Variables all handeled as private */

/* Functions */


void Controller_Initialize(void);
void Controller_Task(void* params);

static void trace(char * string);	// Maybe move to new file, to keep everything organised.


#endif /* CONTROLLER_H */
