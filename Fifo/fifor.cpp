#include "fifo.h"

int main(int argc, char *argv[]){
	string path("/tmp/my");
	Fifo fifow(path,0777);
	if(!fifow.CheckFile())
		fifow.Mkfifo();
	else
		cout<< "fifo file is exist" << endl;
	//O_RDONLY | O_NONBLOCK
	if(!fifow.OpenFifo(O_RDONLY)){
		cout << "OpenFifo failed exit" <<endl;
		exit(-1);
	}
	fifow.ReadFifo();
	
	int count = 0;
	while(1){
		cout<< "count " << count << endl;
		fifow.ReadFifo();
		count++;
		sleep(4);
	}
	
	return 0;
}

