cc = g++ 
prom = main
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 
lib = -lpthread

$(prom): $(obj)
	$(cc)  -g $(lib) -o $(prom) $(obj)

%.o: %.cpp $(deps)
	$(cc) -c -g $< -o $@
clean:
	rm -rf $(obj) $(prom)
