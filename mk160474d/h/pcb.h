/*
 * pcb.h
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#ifndef OS1_PROJEKAT_PCB_H_
#define OS1_PROJEKAT_PCB_H_
#include "listPcb.h"
#include "thread.h"



class PCB{
public:
	enum State {READY,NEW,FINISHED,BLOCKED,RUNNING};

	volatile static PCB* runningPCB;
	static PCB* idlePCB;
	static PCB* userMainPCB;
	static PCB* mainPCB;
	volatile static ID staticId;
	static ListPCB* allPCBs;

	PCB (StackSize stackSize, Time timeSlice, Thread* thread );
	PCB();
	~PCB();

	static void wrapper();
	void putInWaitList();

	//geteri i seteri
	State getState() const;
	void setState(State state);
	ID getId() const;
	Thread* getMyThread();
	Time getTimeLeft() const;
	void setTimeLeft(Time timeLeft);
	Time getTimeSlice() const;
	void setTimeSlice(Time timeSlice);
	unsigned getBp() const;
	void setBp(unsigned bp);
	unsigned getSp() const;
	void setSp(unsigned sp);
	unsigned getSs() const;
	void setSs(unsigned ss);
	int getSleepQueueTimeExpired() const;
	void setSleepQueueTimeExpired(int sleepQueueTimeExpired);

private:
	ID id;
	Thread* myThread;
	unsigned ss,sp,bp;
	unsigned *stack;
	Time timeSlice;
	Time timeLeft;
	StackSize stackSize; 			//Velicina steka thisPCBa izrazena je u 2B = word = sizeof(unsigned));
	State state;
	ListPCB* waitList;
	int sleep_queue_time_expired; 	// 0 ako ga metoda signal izbaci iz spavanja a 1 ako mu istekne dodeljeno vreme spavanja

};


#endif /* OS1_PROJEKAT_PCB_H_ */
