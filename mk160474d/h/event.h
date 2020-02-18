/*
 * event.h
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "IVTentry.h"
#include "kernelev.h"

typedef unsigned char IVTNo;
class KernelEv;

#define PREPAREENTRY(number, callOldRoutine)\
	void interrupt newRoutine##number(...);\
	IVTEntry ivtEntry##number(number,&newRoutine##number);\
	void interrupt newRoutine##number(...){\
		if(IVTEntry::IVTable[number]->getMyKernelEvent()) IVTEntry::IVTable[number]->getMyKernelEvent()->signal();\
		if (callOldRoutine == 1) IVTEntry::IVTable[number]->getMyOldInterruptRoutine()();\
	}\

class Event {
public:

	Event (IVTNo ivtNo);
	~Event ();

	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */
