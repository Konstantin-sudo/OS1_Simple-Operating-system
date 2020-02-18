/*
 * main.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */
#include <IOSTREAM.H>
#include "thread.h"
#include "pcb.h"
#include "timer.h"
#include "idle.h"
#include "sleepQ.h"
#include "userMT.h"



int main(int argc, char* argv[]) {
	Idle* IdleThread = new Idle();
	UserMainThread *userMainThread = new UserMainThread(argc,argv);
	userMainThread->start();
	PCB::mainPCB = new PCB();
	PCB::runningPCB = PCB::mainPCB;

	inic();

	dispatch();
	int ret = userMainThread->getRet();
	delete IdleThread;
	delete userMainThread;

	restore();

	cout<<"\nMain finished ";
	int x;
	cin>>x;

	return ret;
}
