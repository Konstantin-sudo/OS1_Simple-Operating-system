/*
 * timer.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "timer.h"
#include "DEF.h"
#include "kernelS.h"
#include "sleepQ.h"
#include "SCHEDULE.H"
#include "IVTentry.h"

volatile unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina
volatile unsigned tsp,tss,tbp;
volatile int explicit_context_change_requested = 0;
volatile int lockFlag = 1;

void tick();
// postavlja novu prekidnu rutinu
void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

// vraca staru prekidnu rutinu
void restore(){
	for(int i = 0; i < 256; i++){
		if(IVTEntry::IVTable[i]){
			delete IVTEntry::IVTable[i];
		}
	}
	delete [] IVTEntry::IVTable;
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif

}

void interrupt timer(){
	Time timeSlice = ((PCB*)PCB::runningPCB)->getTimeSlice();
	Time timeLeft = ((PCB*)PCB::runningPCB)->getTimeLeft();
	if (!explicit_context_change_requested) {
		if(timeSlice != 0) ((PCB*)PCB::runningPCB)->setTimeLeft(--timeLeft);
		KernelSem::allSem->decrementTime();
		tick();
	}
	if ((timeLeft == 0 && timeSlice != 0) || explicit_context_change_requested) {
		if (lockFlag){
			if(timeLeft == 0 && timeSlice != 0) ((PCB*)PCB::runningPCB)->setTimeLeft(timeSlice);
			explicit_context_change_requested = 0;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			((PCB*)PCB::runningPCB)->setSp(tsp);
			((PCB*)PCB::runningPCB)->setSs(tss);
			((PCB*)PCB::runningPCB)->setBp(tbp);

			if(((PCB*)PCB::runningPCB)->getState() == PCB::RUNNING && PCB::runningPCB != PCB::idlePCB && PCB::runningPCB && PCB::runningPCB!=PCB::mainPCB) {
				((PCB*)PCB::runningPCB)->setState(PCB::READY);
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
				Scheduler::put((PCB*)PCB::runningPCB);				//da li treba okrziti funkcije iz scheduler a sa lock i nlock direktivama????
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
			}
#ifndef BCC_BLOCK_IGNORE
	lock;
	lockF;
#endif
			PCB::runningPCB = Scheduler::get();
#ifndef BCC_BLOCK_IGNORE
	unlock;
	unlockF;
#endif
			if(PCB::runningPCB == 0){
				PCB::runningPCB = PCB::idlePCB;
			}
			if(PCB::allPCBs->getCnt() == 1 && PCB::userMainPCB->getState() != PCB::BLOCKED){ //uslov sa block se ispituje zbog slucaja kada bi userMain bio blokiran na nekom semaforu a ostale niti bile zavrsene
				PCB::runningPCB = PCB::userMainPCB;
				if(PCB::userMainPCB->getState() == PCB::FINISHED) PCB::runningPCB = PCB::mainPCB;
			}
			if(PCB::allPCBs->getCnt() == 0) PCB::runningPCB = PCB::mainPCB;

			((PCB*)PCB::runningPCB)->setState(PCB::RUNNING);

			tsp = ((PCB*)PCB::runningPCB)->getSp();
			tss = ((PCB*)PCB::runningPCB)->getSs();
			tbp = ((PCB*)PCB::runningPCB)->getBp();

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif

		}
		else explicit_context_change_requested = 1;
	}
	if (!explicit_context_change_requested) {
	#ifndef BCC_BLOCK_IGNORE
			asm int 60h;
	#endif
	}
}




