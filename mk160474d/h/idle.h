/*
 * idle.h
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#ifndef OS1_PROJEKAT_IDLE_H_
#define OS1_PROJEKAT_IDLE_H_

#include "Thread.h"

class Idle: public Thread{
public:

	Idle();
	virtual void run();

};


#endif /* OS1_PROJEKAT_IDLE_H_ */
