all: clean compile link 
link:
	g++ -o bin/warehouse bin/main.o bin/Order.o bin/Volunteer.o bin/Customer.o bin/Action.o bin/WareHouse.o
compile:
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/main.o src/main.cpp
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/Order.o src/Order.cpp
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/Customer.o src/Customer.cpp
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/Action.o src/Action.cpp
	g++ -std=c++11 -Iinclude -g -Wall -Weffc++ -c -o bin/WareHouse.o src/WareHouse.cpp
 	
clean:
	rm -f ./bin/*.o
	rm -f ./bin/warehouse

cleanObjAndRun:
	rm -f *.o
	clear
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse ./bin/rest/input_file.txt

