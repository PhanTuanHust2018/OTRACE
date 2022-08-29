all:
	gcc OTRACE.c -o bin/OTRACE -lpthread
	gcc L3_RRC.c -o bin/L3_RRC -lpthread
	gcc UE.c -o bin/UE -lpthread

clean:
	rm bin/OTRACE
	rm bin/L3_RRC
	rm bin/UE
