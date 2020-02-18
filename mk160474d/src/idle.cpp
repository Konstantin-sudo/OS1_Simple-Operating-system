/*
 * idle.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include "idle.h"

//#include "DEF.h"
//#include <iostream.h>

void Idle::run(){
	while(1){

		for(int ii = 0; ii < 10; ii++){
			for(int i = 0; i < 10000; i++)
				for(int j = 0; j < 10000; j++);
		}
		//lockF
		//cout<<"IDLE ";
		//unlockF
	}
}

Idle::Idle():Thread(defaultStackSize,defaultTimeSlice){}

