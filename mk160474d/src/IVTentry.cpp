/*
 * IVTentry.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#include "IVTentry.h"
#include <DOS.H>
#include "DEF.h"

IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(unsigned num, InterruptRoutine newRoutine){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(num >= 0 && num <= 255){
		myNewInterruptRoutine = newRoutine;
		myKernelEvent = 0;
		myOldInterruptRoutine = 0;
		myIVTnumber = num;
#ifndef BCC_BLOCK_IGNORE
		myOldInterruptRoutine = getvect(myIVTnumber);
		setvect(myIVTnumber,myNewInterruptRoutine);
#endif
		IVTEntry::IVTable[myIVTnumber] = this;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
#ifndef BCC_BLOCK_IGNORE
		setvect(myIVTnumber,myOldInterruptRoutine); //prebaci u restore!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
		myKernelEvent = 0;
		myOldInterruptRoutine = 0;
		myNewInterruptRoutine = 0;
		IVTEntry::IVTable[myIVTnumber] = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

//geteri i seteri
InterruptRoutine IVTEntry::getMyOldInterruptRoutine() const {
	return myOldInterruptRoutine;
}

KernelEv* IVTEntry::getMyKernelEvent() {
	return myKernelEvent;
}

void IVTEntry::setMyKernelEvent(KernelEv* myKernelEvent) {
	this->myKernelEvent = myKernelEvent;
}
