/*
 * DEF.h
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#ifndef DEF_H_
#define DEF_H_

extern volatile int lockFlag;
extern volatile int explicit_context_change_requested;

#define lock asm { pushf; cli; }
#define unlock asm { popf; }

#define lockF lockFlag = 0;
#define unlockF lockFlag = 1;\
if(explicit_context_change_requested == 1){\
	dispatch();\
}\


#endif /* DEF_H_ */
