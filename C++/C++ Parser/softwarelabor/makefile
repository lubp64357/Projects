CC=g++ --std=c++11
# testParser : testParser.o parser.o tokenizer.o ast.o
# 	$(CC) testParser.o parser.o tokenizer.o ast.o -o testParser
# testParser.o: testParser.cpp
# 	$(CC) -c testParser.cpp



# Das Programm starten mit "make" 
# ./testVM.exe 


testVM : testVM.o vm.o parser.o tokenizer.o ast.o
	$(CC) testVM.o vm.o parser.o tokenizer.o ast.o -o testVM

testVm.o: testVm.cpp
	$(CC) -c testVm.o 



ast.o: ast.cpp ast.h vm.h
	$(CC) -c ast.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CC) -c tokenizer.cpp

parser.o: parser.cpp parser.h ast.h tokenizer.h utility.h
	$(CC) -c parser.cpp 

vm.o : vm.cpp vm.h
	$(CC) -c vm.cpp