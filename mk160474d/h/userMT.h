/*
 * mainthread.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef OS1_PROJEKAT_MAINT_H_
#define OS1_PROJEKAT_MAINT_H_
#include"thread.h"

class UserMainThread: public Thread {
public:
	UserMainThread(int argc, char* argv[]);

	virtual void run();

	//geteri
	int getRet() const;

private:
	int ret;
	int myArgc;
	char **myArgv;
};


#endif /* OS1_PROJEKAT_MAINT_H_ */
