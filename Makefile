VacRouter.out : VaccineRouter.o VaccineRunner.o  
	g++ -ansi -Wall -g -o VacRouter.out VaccineRouter.o VaccineRunner.o  

VaccineRouter.o : VaccineRouter.cpp VaccineRouter.h 
	g++ -ansi -Wall -g -c VaccineRouter.cpp

VaccineRunner.o : VaccineRunner.cpp VaccineRunner.h VaccineRouter.h CPUTimer.h 
	g++ -ansi -Wall -g -c VaccineRunner.cpp

clean : 
	rm -f VacRouter.out VaccineRouter.o  VaccineRunner.o   
