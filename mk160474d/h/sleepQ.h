/*
 * Queue.h
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */

#ifndef SLEEPQ_H_
#define SLEEPQ_H_

#include "semaphor.h"

class PCB;
class KernelSem;

class SleepQueue{
public:
	class qElem{
	public:
		PCB* data;
		qElem* next;
		int infinity;
		Time timeToWait;

		qElem(PCB* p, Time t, int i = 0, qElem* n = 0){
			data = p;
			timeToWait = t;
			infinity = i;
			next = n;
		}
		~qElem(){
			data = 0;
			next = 0;
			infinity = 0;
			timeToWait = 0;
		}
	};

	SleepQueue(KernelSem* k);
	~SleepQueue();

	void put(PCB* p,Time timeToWait);
	int remove(int n);						//remove vraca broj odblokiranih niti
	void decrementTime();

	//geteri
	int getCnt() const;

private:
	qElem* head;
	qElem* tail;
	int cnt;
	KernelSem* myKsem;

};



#endif /* SLEEPQ_H_ */
