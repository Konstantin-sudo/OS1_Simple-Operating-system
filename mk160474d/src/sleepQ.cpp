/*
 * queue.cpp
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */
#include <IOSTREAM.H>
#include "sleepQ.h"
#include "pcb.h"
#include "DEF.h"
#include "kernelS.h"
#include "SCHEDULE.H"

SleepQueue::SleepQueue(KernelSem* k){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	myKsem = k;
	head = 0;
	tail = 0;
	cnt = 0;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

SleepQueue::~SleepQueue(){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	while(head){   					// ili samo pozove this->remove(cnt); ali isti djavo
		head->data->setState(PCB::READY);
		head->data->setSleepQueueTimeExpired(0);
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
		Scheduler::put(head->data);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
		qElem* old = head;
		head=head->next;
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
		delete old;
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
		cnt--;
	}
	myKsem = 0;
	head = tail = 0;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

void SleepQueue::put(PCB* p, Time timeToWait){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	if(p){
		if(timeToWait <= 0){		//=========================================================================================================
			if(head){				//=========================================================================================================
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
				tail->next  = new qElem(p,timeToWait,1);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
				tail = tail->next;
			}else {									//ako u sleepQueue ubacujemo nit koja treba neograniceno dugo da ostane u stanju spavanja
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
				tail = head = new qElem(p,timeToWait,1);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;								//=========================================================================================================
			}						//=========================================================================================================
			cnt++;
		}else{
			//kada je timeToWait > 0:
			if(head){
				int num = timeToWait;
				qElem* curr = head;
				qElem* prev = 0;
				while(curr && num >= curr->timeToWait){
					num -=curr->timeToWait;
					prev = curr;
					curr = curr->next;
				}//treba i smanjiti timetowait od curr za num
				if(curr){
					if(prev) {
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
						prev->next = new qElem(p,num,0,curr);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
						prev = prev->next;
					}
					else {
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
						head = new qElem(p,num,0,curr);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
						prev = head;
					}
					if(!curr->infinity) curr->timeToWait -= num;
					while(curr){							//----------------------------------------------------------------
						if(curr->infinity && !prev->infinity){
							PCB* t = curr->data;
							curr->data = prev->data;
							prev->data = t;
							curr->infinity = 0;				//ovaj deo koda sluzi da red bude "vise fer" realizovan
							prev->infinity = 1;
							curr->timeToWait = prev->timeToWait;
							prev->timeToWait = 0;

						}
						prev = curr;
						curr=curr->next;
					}										//--------------------------------------------------------------------
				}else{
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
					prev->next = new qElem(p,num);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
					tail = prev->next;
				}
			}else{
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
				tail = head = new qElem(p,timeToWait);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
			}
			cnt++;
		}
	}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

int SleepQueue::remove(int n){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	int c = 0;
	if(n > 0){
		while(head && n){
			head->data->setState(PCB::READY);
			head->data->setSleepQueueTimeExpired(0);
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
			Scheduler::put(head->data);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;
			qElem* old = head;
			head = head->next;
#ifndef BCC_BLOCK_IGNORE
			lock;
			lockF;
#endif
			delete old;
#ifndef BCC_BLOCK_IGNORE
			unlock;
			unlockF;
#endif
			n--;
			c++;
			cnt--;
		}
	}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
	return c;
}

void SleepQueue::decrementTime(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif;
	if(head){
		qElem* curr = head;
		qElem* prev = 0;
		while(curr->infinity){
			prev = curr;
			curr = curr->next;
		}
		if(curr){
			curr->timeToWait--;
			while(curr->timeToWait == 0){
				if(!curr->infinity){
					curr->data->setState(PCB::READY);
					curr->data->setSleepQueueTimeExpired(1);
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif;
					Scheduler::put(curr->data);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif;

					myKsem->setValue(myKsem->getValue() + 1);
					qElem* old = curr;
					if(prev){
						prev->next = curr->next;
						curr = curr->next;
					}else{
						curr = curr->next;
						head->next = 0;
						head = curr;
					}
#ifndef BCC_BLOCK_IGNORE
			lock;
			lockF;
#endif
					delete old;
#ifndef BCC_BLOCK_IGNORE
			unlock;
			unlockF;
#endif
					cnt--;
				}else{
					prev = curr;
					curr = curr->next;
				}
			}
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif;
}

//geteri
int SleepQueue::getCnt() const {
	return cnt;
}
