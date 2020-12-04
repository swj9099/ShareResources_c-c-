#include "fifo.h"

void sigfuc1(int){
	cout << "get SIGPIPE" <<endl;
}
bool Fifo::CheckFile(){
	return access(m_fifopath.c_str(), F_OK) ? false : true;
}


int Fifo::Mkfifo(){
	int ret = 0;
	ret = mkfifo(m_fifopath.c_str(),m_mode);

	if(0 == ret)
		printf("mkfifo success!!\n");
	else
		perror("mkfifo fialed :");
	return ret;

}

bool Fifo::OpenFifo(int mode){
	bool ret = true;
	fd = open(m_fifopath.c_str(),mode);
	if(-1 == fd){
		perror("OpenFifo is failed :");
		ret = false;
	}

	return ret;
}

int Fifo::SetData(string str){
	bzero(Buff,BUFFSIZE);
	strncpy(Buff, str.c_str(), BUFFSIZE);
}


int Fifo::ReadFifo(){
	ssize_t byte = 0;
	byte = read(fd, Buff, sizeof(Buff));
	printf("read str is %s \n",Buff);
	printf("read byte is %d \n",byte);
}

int Fifo::WriteFifo(){
	ssize_t byte = 0;
	byte = write(fd, Buff, sizeof(Buff));	
	printf("wead byte is %d \n",byte);
}

bool Fifo::CloseFifo(){
	int ret = 0;
	ret = close(fd);
	if(-1 == ret)
		perror("close fialed...");
}

void Fifo::SetSigpipe(void(* func)(int)){
	signal(SIGPIPE, func);
}



