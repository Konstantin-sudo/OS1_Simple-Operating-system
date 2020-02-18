/*
 * event.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: OS1
 */
#include "event.h"
#include "kernelev.h"

Event::Event(IVTNo ivtNo){
	if((unsigned)ivtNo >= 0 && (unsigned)ivtNo <256) myImpl = new KernelEv((unsigned)ivtNo,this);
	else myImpl = 0;
}

Event::~Event(){
	if(myImpl) delete myImpl;
}

void Event::wait(){
	if(myImpl) myImpl->wait();
}

void Event::signal(){
	if(myImpl) myImpl->signal();
}
