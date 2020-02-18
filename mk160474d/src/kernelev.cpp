/*
 * kernelev.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#include "kernelev.h"
#include "IVTentry.h"
#include "pcb.h"
#include "DEF.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(unsigned ivtNo,Event* ev){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	myIVTnum = ivtNo;
	if(IVTEntry::IVTable[myIVTnum]) IVTEntry::IVTable[myIVTnum]->setMyKernelEvent(this);
	myPCB = (PCB*)PCB::runningPCB;
	value = 1;
	myEvent = ev;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

KernelEv::~KernelEv(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	myPCB = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void KernelEv::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(value == 1 && myPCB == PCB::runningPCB){
		value = 0;
		((PCB*)PCB::runningPCB)->setState(PCB::BLOCKED);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
		dispatch();
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void KernelEv::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(value == 0){
		value = 1;
		myPCB->setState(PCB::READY);
		Scheduler::put(myPCB);
/*#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
		dispatch(); 			//da li ovo sme ( unutar prekidne rutine(prekidna rutina eventa koja poziva signal) se poziva prekidna rutina(timer)) ?????
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif*/
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}
