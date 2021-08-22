/*
 * CLI.h
 *
 * Author: Liron Weizman 206505588
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI
{
	DefaultIO *dio;
	//Info* info;
	// you can add data members
public:
	CLI(DefaultIO *dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
