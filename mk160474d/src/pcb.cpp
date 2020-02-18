/*
 * pcb.cpp
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "thread.h"
#include "DEF.h"
#include "idle.h"
#include "SCHEDULE.h"
#include <dos.h>
#include "listPcb.h"



volatile PCB* PCB::runningPCB = 0;
PCB* PCB::idlePCB = 0;
PCB* PCB::userMainPCB = 0;
PCB* PCB::mainPCB = 0;
ListPCB* PCB::allPCBs = new ListPCB();
volatile ID PCB::staticId = 0;


PCB::PCB(){
	id = -1;
	ss = sp = bp = 0;
	stack = 0;
	stackSize = 0;
	myThread = 0;
	timeSlice = timeLeft = 0;
	state = 0;
	waitList = 0;
	sleep_queue_time_expired = 0;
}

PCB::PCB (StackSize stackSize, Time timeSlice, Thread* thread ){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	id = PCB::staticId++;
	myThread = thread;
	this->timeSlice = timeSlice;
	timeLeft = timeSlice;
	sleep_queue_time_expired = 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
	waitList = new ListPCB();
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif

	if(id == 1){						// id od main niti a je 1, od idle niti je 0, a random br za ostale korisnicke niti (mora tako )
		PCB::userMainPCB = this;
	}
	this->stackSize = stackSize/sizeof(unsigned);
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
	stack = new unsigned[this->stackSize];
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
	//da li treba uslov if(stack) ??? u slucaju da ne uspe alokacija memorije, neka ga;
	if(stack){
		state = PCB::NEW;
		stack[this->stackSize-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
		stack[this->stackSize-2] = FP_SEG(PCB::wrapper);
		stack[this->stackSize-3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(stack + this->stackSize - 12);						//kako bre 12 ????????
		ss = FP_SEG(stack + this->stackSize - 12);
	#endif
		bp = sp;
		if(id == 0){
			PCB::idlePCB = this;
			state = PCB::READY; 	//da l ovo treba???svjdn, nigde se ne gleda, ali je READY najlogicnijji
		}
	}else state = PCB::FINISHED;

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}



void PCB::putInWaitList(){
	waitList->put((PCB*)PCB::runningPCB); //okruzen sa lock i unlock (iz thread::waitToComplete() f-je)
}


void PCB::wrapper(){
	/*PCB::runningPCB->state = PCB::RUNNINg; odradjeno unutar prekida timer()*/
	PCB::runningPCB->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif

	if(PCB::runningPCB->waitList) PCB::runningPCB->waitList->emptyList();
	PCB::allPCBs->deleteById(PCB::runningPCB->id);
	PCB::runningPCB->state = PCB::FINISHED;

#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
	dispatch();
}


PCB::~PCB(){
	if(stack) {
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif
		delete [] stack;
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif
	}
	if(waitList){
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif
		delete waitList;
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif
	}
	myThread = 0;
}

//geteri i seteri
PCB::State PCB::getState() const {
	return state;
}

void PCB::setState(State state) {
	this->state = state;
}

ID PCB::getId() const {
	return id;
}

Thread* PCB::getMyThread() {
	return myThread;
}

Time PCB::getTimeLeft() const {
	return timeLeft;
}

void PCB::setTimeLeft(Time timeLeft) {
	this->timeLeft = timeLeft;
}

Time PCB::getTimeSlice() const {
	return timeSlice;
}

void PCB::setTimeSlice(Time timeSlice) {
	this->timeSlice = timeSlice;
}

unsigned PCB::getBp() const {
	return bp;
}

void PCB::setBp(unsigned bp) {
	this->bp = bp;
}

unsigned PCB::getSp() const {
	return sp;
}

void PCB::setSp(unsigned sp) {
	this->sp = sp;
}

unsigned PCB::getSs() const {
	return ss;
}

void PCB::setSs(unsigned ss) {
	this->ss = ss;
}

int PCB::getSleepQueueTimeExpired() const {
	return sleep_queue_time_expired;
}

void PCB::setSleepQueueTimeExpired(int sleepQueueTimeExpired) {
	sleep_queue_time_expired = sleepQueueTimeExpired;
}
