all: fifow fifor

fifow: fifow.cpp fifo.cpp
	g++ -o fifow fifow.cpp fifo.cpp

fifor: fifor.cpp fifo.cpp
	g++ -o fifor fifor.cpp fifo.cpp

clean:
	rm *.o fifow fifor -rf
