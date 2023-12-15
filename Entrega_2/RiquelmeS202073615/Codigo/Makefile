GCC = g++ -std=c++11
FOLDER_INSTANCIAS ?= Instancias-RCVRP
SET ?= SET_V/RISK_LEVEL
FILE_TEST ?= 10.txt
FOLDER = src

compile:
	$(GCC) $(FOLDER)/rcvrp.cpp -o out

one_test:
	./out $(FOLDER_INSTANCIAS) $(SET) $(FILE_TEST)
