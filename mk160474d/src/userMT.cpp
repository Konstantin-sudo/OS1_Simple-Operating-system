/*
 * mainthread.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "userMT.h"

int userMain(int argc, char* argv[]);;

UserMainThread::UserMainThread(int argc, char* argv[]):Thread(),myArgc(argc), myArgv(argv), ret(0){}

void UserMainThread::run(){
	ret = userMain(myArgc,myArgv);
}

//geteri
int UserMainThread::getRet() const {
	return ret;
}
