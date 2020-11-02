inc_cparam = /home/dww01/ABECrypto/cryptoparam/
inc_matrix = /home/dww01/ABECrypto/lsssmatrix/
inc_accp = /home/dww01/ABECrypto/accesspolicy/
inc_murmur = /home/dww01/ABECrypto/murmurhash/
inc_param = /home/dww01/ABECrypto/param/
inc_rigbf = /home/dww01/ABECrypto/rigbf/


obj_matrix = /home/dww01/ABECrypto/obj/lsssmatrix/
obj_accp = /home/dww01/ABECrypto/obj/accesspolicy/
obj_murmur = /home/dww01/ABECrypto/obj/murmurhash/
obj_param = /home/dww01/ABECrypto/obj/param/
obj_rigbf = /home/dww01/ABECrypto/obj/rigbf/

obj_myscheme = /home/dww01/ABECrypto/obj/schemes/myscheme/
link = -L. -lgmp -lpbc

myscheme:myscheme.o
	g++ $(obj_myscheme)myscheme.o $(obj_accp)accesspolicy.o $(obj_matrix)attsmaptoelement.o $(obj_matrix)elementmatrix.o \
	$(obj_matrix)elementvector.o $(obj_matrix)lsssmatrix.o $(obj_murmur)hasher.o $(obj_murmur)MurmurHash3.o \
	$(obj_param)abeparam.o $(obj_param)curve_param.o $(obj_rigbf)rigbf.o -o myscheme $(link)

myscheme.o:myscheme.cpp
	g++ myscheme.cpp -c -o $(obj_myscheme)myscheme.o -I$(inc_cparam) -I$(inc_matrix) -I$(inc_accp) -I$(inc_murmur) \
	-I$(inc_param) -I$(inc_rigbf)