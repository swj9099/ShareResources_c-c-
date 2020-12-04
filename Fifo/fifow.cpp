#include "fifo.h"

int main(int argc, char *argv[]){

	string path("/tmp/my");
	Fifo fifow(path,0777);
	fifow.SetSigpipe(sigfuc1);
	if(!fifow.CheckFile())
		fifow.Mkfifo();
	else
		cout<< "fifo file is exist" << endl;
	//O_NONBLOCK | O_WRONLY
	//if(!fifow.OpenFifo(O_WRONLY)){
	if(!fifow.OpenFifo(O_WRONLY | O_NONBLOCK)){
		cout << "OpenFifo failed exit" <<endl;
		perror("OpenFifo failed error is :");
		exit(-1);
	}
	int count = 0;
	string str("write.....");
	while(count < 15){
		cout<< "count " << count << endl;
		fifow.SetData(str);
		fifow.WriteFifo();
		count++;
		sleep(2);
	}
	fifow.CloseFifo();
	return 0;
}
