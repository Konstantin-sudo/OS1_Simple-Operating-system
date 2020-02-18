/*
 * thread.h
 *
 *  Created on: May 20, 2019
 *      Author: OS1
 */

#ifndef OS1_PROJEKAT_THREAD_H_
#define OS1_PROJEKAT_THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default 2*55ms
typedef int ID;

class PCB;

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getID();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize,Time timeSlice = defaultTimeSlice);

	virtual void run();

private:
	PCB* myPCB;
};

void dispatch();

#endif /* OS1_PROJEKAT_THREAD_H_ */


