obj = /home/dww01/ABECrypto/obj/murmurhash/


mmh: MurmurHash3.o hash.o
	g++ $(obj)MurmurHash3.o $(obj)hash.o -o mmh

MurmurHash3.o:MurmurHash3.cpp
	g++ MurmurHash3.cpp -c -o $(obj)MurmurHash3.o

hash.o:hash.cpp
	g++ hash.cpp -c -o $(obj)hash.o