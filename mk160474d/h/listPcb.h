/*
 * listofpcbs.h
 *
 *  Created on: May 22, 2019
 *      Author: OS1
 */

#ifndef OS1_PROJEKAT_LIST_H_
#define OS1_PROJEKAT_LIST_H_

#include "thread.h"

class PCB;

class ListPCB{
public:
	class pcbElem{
	public:
		PCB* data;
		pcbElem* next;
		pcbElem* prev;
		pcbElem(PCB* d, pcbElem* n = 0, pcbElem *p = 0){
			data = d;
			next = n;
			prev = p;
		}
		~pcbElem(){
			data = 0;
			next = prev = 0;
		}

	};

	ListPCB();
	~ListPCB();

	void put(PCB* p);
	void deleteById(ID id);
	void emptyList();

	//geteri i seteri
	volatile int getCnt() const;
	pcbElem* getHead() const;

private:
	pcbElem* head;
	pcbElem* tail;
	volatile int cnt;

};



#endif /* OS1_PROJEKAT_LIST_H_ */
