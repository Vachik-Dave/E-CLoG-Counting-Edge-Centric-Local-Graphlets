/*
	----------- time counter class -----------
*/

#ifndef RUNTIMECOUNTER_H_
#define RUNTIMECOUNTER_H_

#include <sys/time.h>

/*struct timevalue{
long    tv_sec;         // seconds
long    tv_usec;        // microseconds
};
*/

class Runtimecounter
{
public:
	timeval t1;
	timeval t2;
	Runtimecounter();
	void start();
	void stop();
	float GetRuntime();

};

Runtimecounter::Runtimecounter(){}

void Runtimecounter::start()
{
	gettimeofday(&this->t1, NULL);
}

void Runtimecounter::stop()
{
	gettimeofday(&this->t2, NULL);
}

float Runtimecounter::GetRuntime()
{
	float t=(float)(t2.tv_sec-t1.tv_sec)*1.0+(float)(t2.tv_usec-t1.tv_usec)/1000000.0;
//	t = t* 1000; 		//milisec
	return t;
}

#endif
