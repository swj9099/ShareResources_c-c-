#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

using namespace std;
#define MYFIFOPATH "/tmp/my_fifo"
#define BUFFSIZE 125

void sigfuc1(int);

class Fifo{

public:
	Fifo(){}
	
	Fifo(string path,mode_t mode)
		:m_fifopath(path), m_mode(mode){}
		
	~Fifo(){}
	bool CheckFile();
	
	int Mkfifo();
	bool OpenFifo(int mode);
	int ReadFifo();
	int WriteFifo();
	int SetData(string str);
	bool CloseFifo();
	void SetSigpipe(void(* func)(int));

private:
	mode_t m_mode;
	string m_fifopath;
	int fd;
	char Buff[BUFFSIZE];

};


