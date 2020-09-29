Include_Dir = /home/dww01/ABECrypto/include/
Obj_Dir = /home/dww01/ABECrypto/obj/
Source_Dir = /home/dww01/ABECrypto/source/
Lib_Dir = /home/dww01/ABECrypto/lib/
Link = -L. -lgmp -lpbc

All: Waters11Scheme



Waters11Scheme: Waters11Scheme.o LSSSMatrix.o ElementInMPQMatrix.o ElementInMPQVector.o ElementInZrMatrix.o ElementInZrVector.o AttsMapToElement.o
	g++ $(Obj_Dir)Waters11Scheme.o $(Obj_Dir)LSSSMatrix.o $(Obj_Dir)ElementInMPQMatrix.o $(Obj_Dir)ElementInMPQVector.o $(Obj_Dir)ElementInZrMatrix.o $(Obj_Dir)ElementInZrVector.o $(Obj_Dir)AttsMapToElement.o  -o Waters11Scheme $(Link)


Waters11Scheme.o: $(Source_Dir)Waters11Scheme.cpp 
	g++ $(Source_Dir)Waters11Scheme.cpp -c -o $(Obj_Dir)Waters11Scheme.o -I$(Include_Dir)

LSSSMatrix.o: $(Source_Dir)LSSSMatrix.cpp
	g++ $(Source_Dir)LSSSMatrix.cpp -c -o $(Obj_Dir)LSSSMatrix.o -I$(Include_Dir)


ElementInMPQMatrix.o:$(Source_Dir)ElementInMPQMatrix.cpp
	g++ $(Source_Dir)ElementInMPQMatrix.cpp -c -o $(Obj_Dir)ElementInMPQMatrix.o -I$(Include_Dir) -L. -lgmp


ElementInMPQVector.o:$(Source_Dir)ElementInMPQVector.cpp
	g++ $(Source_Dir)ElementInMPQVector.cpp -c -o $(Obj_Dir)ElementInMPQVector.o -I$(Include_Dir) -L. -lgmp


ElementInZrMatrix.o:$(Source_Dir)ElementInZrMatrix.cpp
	g++ $(Source_Dir)ElementInZrMatrix.cpp -c -o $(Obj_Dir)ElementInZrMatrix.o -I$(Include_Dir) -L. -lgmp -lpbc

ElementInZrVector.o:$(Source_Dir)ElementInZrVector.cpp
	g++ $(Source_Dir)ElementInZrVector.cpp -c -o $(Obj_Dir)ElementInZrVector.o -I$(Include_Dir) -L. -lgmp -lpbc

AttsMapToElement.o:$(Source_Dir)AttsMapToElement.cpp
	g++ $(Source_Dir)AttsMapToElement.cpp -c -o $(Obj_Dir)AttsMapToElement.o -I$(Include_Dir) -L. -lgmp -lpbc

clear:
	rm -r Waters11Scheme
	rm -r $(Obj_Dir)*.o