/*
 * ivtentry.h
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef void interrupt (*InterruptRoutine)(...);
class KernelEv;

class IVTEntry{
public:
	static IVTEntry* IVTable[256];

	IVTEntry(unsigned num, InterruptRoutine newRoutine);
	~IVTEntry();

	//geteri i seteri
	InterruptRoutine getMyOldInterruptRoutine() const;
	KernelEv* getMyKernelEvent();
	void setMyKernelEvent(KernelEv* myKernelEvent);

private:
	InterruptRoutine myOldInterruptRoutine, myNewInterruptRoutine;
	unsigned myIVTnumber;
	KernelEv* myKernelEvent;

};



#endif /* IVTENTRY_H_ */
