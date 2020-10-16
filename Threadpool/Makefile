cc = g++ 
prom = main
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 
lib = -lpthread

$(prom): $(obj)
	$(cc) $(lib) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@
clean:
	rm -rf $(obj) $(prom)
