/*
 * thread.cpp
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "timer.h"
#include "DEF.h"
#include "idle.h"
#include "listPcb.h"
#include "SCHEDULE.H"


ID Thread::getID(){
	return myPCB->getId();
}

ID Thread::getRunningId(){
	return ((PCB*)PCB::runningPCB)->getId();
}

Thread * Thread::getThreadById(ID id){
	ListPCB::pcbElem* curr = PCB::allPCBs->getHead();
	while(curr && (curr->data->getId() != id)) curr = curr->next;//prodje kroz listu ili niz svih pcb-ova
	if (curr) return curr->data->getMyThread();						// i vrati odgovarajuci
	else return 0;
}


void Thread::start(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(PCB::idlePCB != myPCB && myPCB->getState() == PCB::NEW ){		//ako se ne startuje idle nit, stavi nit u scheduler
		myPCB->setState(PCB::READY);
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
		Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
		PCB::allPCBs->put(this->myPCB); //ovo ide ovde a ne u pcb konstruktoru, jer kad bi se napravila nit koja se nikad ne startuje main bi je cekao da se zavrsi(program se nikada ne bi zavrsio - zbog cnt)
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif
	if(myPCB && myPCB != PCB::idlePCB && PCB::runningPCB != PCB::idlePCB
			&& myPCB != PCB::runningPCB && myPCB->getState() != PCB::FINISHED
			&& myPCB->getState() != PCB::NEW && myPCB != PCB::userMainPCB && myPCB != PCB::mainPCB){  	//previse uslova al neka ih

		this->myPCB->putInWaitList();
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


Thread::~Thread(){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif
	if(myPCB->getState() != PCB::FINISHED) this->waitToComplete();
	myPCB->~PCB();
	myPCB = 0;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
}

Thread::Thread(StackSize stackSize,Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(stackSize > 65536 || stackSize < 0) stackSize = 65536;
	lockF;
	myPCB = new PCB(stackSize,timeSlice,this); //stek mora biti do 64KB
	unlockF;
	#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
	}


void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	explicit_context_change_requested = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

}

void Thread::run(){}

