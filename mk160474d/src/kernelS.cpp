/*
 * kernelsem.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#include "kernelS.h"
#include "pcb.h"
#include "sleepQ.h"
#include "DEF.h" //zbog lock i unlock

ListKSem* KernelSem::allSem = new ListKSem();

KernelSem::KernelSem(int init, Semaphore* sem){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	value = init;
	mySemaphore = sem;
	sleepQueue = new SleepQueue(this);
	KernelSem::allSem->put(this);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}


KernelSem::~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	value = 0;
	sleepQueue->remove(sleepQueue->getCnt());
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
	delete sleepQueue;
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
	allSem->remove(this);
	mySemaphore = 0; // ne mora delete da se okruzuje sa lock/unlock jer se ovo poziva na kraju main a uglavnom;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}


int KernelSem::wait(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif;
	--value;
	if(value < 0)	{
		((PCB*)PCB::runningPCB)->setState(PCB::BLOCKED);
		sleepQueue->put((PCB*)PCB::runningPCB, maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
		dispatch();
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	}else{
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
		return 1;
	}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
	return !((PCB*)PCB::runningPCB)->getSleepQueueTimeExpired();
}


int KernelSem::signal(int n){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif		// ne mora, moze samo unutar empty metode - valjda ?
	int c = 0;
	if(n < 0){
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
		return n;
	}
	else{
		if(n == 0){
			++value;
			if(value <= 0) c = sleepQueue->remove(1);
		}else{
			value += n;
			c = sleepQueue->remove(n);
		}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
		return c;
	}
}

//geteri i seteri
SleepQueue* KernelSem::getSleepQueue() {
	return sleepQueue;
}

int KernelSem::getValue() const {
	return value;
}

void KernelSem::setValue(int value) {
	this->value = value;
}

