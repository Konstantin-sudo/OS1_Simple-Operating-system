/*
 * listKSem.cpp
 *
 *  Created on: Jun 5, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "DEF.h"
#include "kernelS.h"
#include "listKSem.h"
#include "sleepQ.h"

ListKSem::ListKSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	tail = head =  0;
	cnt = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

ListKSem::~ListKSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	while(head){
		ksemElem* old = head;
		head = head->next;
		delete old; // ne mora ni ovde da se okruzuje sa lock/unlock
		cnt--;
	}
	head = tail = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void ListKSem::put(KernelSem* k){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(k){
		if(head){
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF
#endif
			tail->next = new ksemElem(k);
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
			tail = tail->next;
		}else{
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF
#endif
			head = tail = new ksemElem(k);
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF
#endif
		}
		cnt++;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void ListKSem::remove(KernelSem* k){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	ksemElem* curr = head;
	ksemElem* prev = 0;
	while(curr && curr->data != k){
		prev = curr;
		curr=curr->next;
	}
	if(curr){
		if(prev){
			prev->next = curr->next;
			if(curr == tail) tail = prev;
			curr->next = 0;
		}else{
			head = head->next;
			curr->next = 0;
		}
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
		delete curr;
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void ListKSem::decrementTime(){
	ksemElem* curr = head;
	while(curr){
		curr->data->getSleepQueue()->decrementTime();
		curr=curr->next;
	}
}






