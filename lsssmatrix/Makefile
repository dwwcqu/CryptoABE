Obj_dir = /home/dww01/ABECrypto/obj/lsssmatrix/
Obj_acc_dir = /home/dww01/ABECrypto/obj/accesspolicy/
Obj_param_dir = /home/dww01/ABECrypto/obj/param/
Inc_dir01 = /home/dww01/ABECrypto/accesspolicy/
Inc_dir02 = /home/dww01/ABECrypto/param/
link = -L. -lgmp -lpbc

#main:elementmatrix.o
	#g++ $(Obj_dir)elementmatrix.o $(Obj_dir)lsssmatrix.o $(Obj_dir)elementvector.o $(Obj_acc_dir)accesspolicy.o $(Obj_param_dir)curve_param.o -o main $(link)

elementmatrix.o:lsssmatrix.o elementvector.o
	g++ elementmatrix.cpp -c -o $(Obj_dir)elementmatrix.o -I$(Inc_dir01) -I$(Inc_dir02) $(link)

lsssmatrix.o:lsssmatrix.cpp
	g++ lsssmatrix.cpp -c -o $(Obj_dir)lsssmatrix.o -I$(Inc_dir01)

elementvector.o:elementvector.cpp
	g++ elementvector.cpp -c -o $(Obj_dir)elementvector.o $(link)