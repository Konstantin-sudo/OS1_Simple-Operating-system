/*
 * listofpcbs.cpp
 *
 *  Created on: May 22, 2019
 *      Author: OS1
 */

#include "listPcb.h"
#include "pcb.h"
#include "DEF.h"
#include "SCHEDULE.H"


ListPCB::ListPCB(){
	head = 0;
	tail = 0;
	cnt = 0;
}

ListPCB::~ListPCB(){
	emptyList();
}

void ListPCB::put(PCB* p){  				//implementirano kao stek
	if(p){
		if(head){
			pcbElem *pom = head;
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif;
			head = new pcbElem(p,pom); 	//vec je okruzen sa lockf spolja
#ifndef BCC_BLOCK_IGNORE
	unlock;								//ako se desi da se unutar new operatora setuje fleg za prekide na 1
	unlockF;
#endif;
			pom->prev = head;
		}
		else {
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF
#endif;
			head = new pcbElem(p);
			tail = head;
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF
#endif;
		}
		cnt++;
	}
}

void ListPCB::deleteById(ID id){			//izbacuje pcb sa zadatim id-om iz liste svih pcb ova,ova metoda se poziva na kraju zivota niti sa tim id-om(na kraju metode wrapper)
	pcbElem* prev = 0, *curr;
	curr = head;
	while(curr && curr->data->getId() != id){
		prev = curr;
		curr = curr->next;
	}
	if(curr){
		pcbElem* old = curr;
		if(prev) prev->next = curr->next;
		else head = head->next;
		curr->next = 0;
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
	}
}

void ListPCB::emptyList(){			//implementirano kao red (prazni listu otpozadi)
	while(tail) {
		tail->data->setState(PCB::READY);
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockF;
#endif
		Scheduler::put(tail->data);
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockF;
#endif
		pcbElem* old = tail;
		tail = tail->prev;
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
	}
	head = tail = 0;

}

//geteri i seteri
volatile int ListPCB::getCnt() const {
	return cnt;
}

ListPCB::pcbElem* ListPCB::getHead() const {
	return head;
}
