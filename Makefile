all:
	gcc OTRACE.c -o bin/OTRACE -lpthread
	gcc OAMH.c -o bin/OAMH -lpthread
	gcc L3_gNB.c -o bin/L3_gNB -lpthread
	gcc UE.c -o bin/UE -lpthread

clean:
	rm bin/OTRACE
	rm bin/OAMH
	rm bin/L3_gNB
	rm bin/UE
