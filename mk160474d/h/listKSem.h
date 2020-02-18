/*
 * listSem.h
 *
 *  Created on: Jun 5, 2019
 *      Author: OS1
 */

#ifndef LISTKSEM_H_
#define LISTKSEM_H_

class KernelSem;

class ListKSem{
public:
	class ksemElem{
	public:
		KernelSem* data;
		ksemElem* next;
		ksemElem(KernelSem* k, ksemElem* n = 0){
			data = k;
			next = n;
		}
		~ksemElem(){
			data = 0;
			next = 0;
		}
	};

	ListKSem();
	~ListKSem();

	void put(KernelSem* k);
	void remove(KernelSem* k);
	void decrementTime();

private:
	ksemElem* head;
	ksemElem* tail;
	int cnt;
};

#endif /* LISTKSEM_H_ */
