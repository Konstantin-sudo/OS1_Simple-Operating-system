/*
 * kernelsem.h
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

#include "semaphor.h"
#include "listKSem.h"

class SleepQueue;

class KernelSem{
public:
	static ListKSem *allSem;

	KernelSem(int init, Semaphore* sem);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	//geteri i seteri
	SleepQueue* getSleepQueue();
	int getValue() const;
	void setValue(int value);

private:
	int value; 							//ako je value < 0 onda je abs(value) broj blokiranih niti na ovom semaforu
	Semaphore *mySemaphore;
	SleepQueue *sleepQueue; 			//red svih blokiranih niti na ovom semaforu

};


#endif /* KERNELS_H_ */
