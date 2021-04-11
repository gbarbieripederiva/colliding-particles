CPP_SOURCES = $(shell find simulation -name *.cpp)
HEADER_SOURCES = $(shell find simulation -name *.h)
OUT_FOLDER=out


compile:$(CPP_SOURCES)
	mkdir -p out
	g++ -std=c++17 $(CPP_SOURCES) $(HEADER_SOURCES) -o $(OUT_FOLDER)/simulation.out

compile_debug:$(CPP_SOURCES)
	mkdir -p out
	g++ -std=c++17 -g $(CPP_SOURCES) $(HEADER_SOURCES) -o $(OUT_FOLDER)/simulation.out

run:compile
	@echo "...running:"
	@./$(OUT_FOLDER)/simulation.out

valgrind_test:compile_debug
	@echo ""
	@valgrind --leak-check=full -s ./$(OUT_FOLDER)/simulation.out

clean_data:
	rm -r data

.PHONY clean:
	rm -r out