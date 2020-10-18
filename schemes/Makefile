Obj_param = /home/dww01/ABECrypto/obj/param/
Obj_waters = /home/dww01/ABECrypto/obj/schemes/waters/


Sour_dir = /home/dww01/ABECrypto/schemes/waters11/

Inc_param = /home/dww01/ABECrypto/param/
Inc_cryptoparam = /home/dww01/ABECrypto/cryptoparam/
Inc_matrix = /home/dww01/ABECrypto/lsssmatrix/
Inc_acc = /home/dww01/ABECrypto/accesspolicy/



Link = -L. -lgmp -lpbc

waters11: waters11.o
	g++ $(Obj_waters)waters11.o $(Obj_param)curve_param.o $(Obj_param)abeparam.o -o $(Sour_dir)waters11 $(Link)

waters11.o: $(Sour_dir)waters11.cpp
	g++ $(Sour_dir)waters11.cpp -c -o $(Obj_waters)waters11.o -I$(Inc_param) -I$(Inc_cryptoparam) -I$(Inc_matrix) -I$(Inc_acc)