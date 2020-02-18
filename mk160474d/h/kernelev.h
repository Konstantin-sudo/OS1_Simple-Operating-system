/*
 * kernelev.h
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

class SleepQueue;
class PCB;
class Event;

class KernelEv{
public:
	KernelEv(unsigned ivtNo,Event* ev);
	~KernelEv();

	void wait();
	void signal();

private:
	int value;
	unsigned myIVTnum;
	PCB* myPCB;
	Event* myEvent;
};



#endif /* KERNELEV_H_ */
