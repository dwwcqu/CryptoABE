#
#		尚未完成
#		Auther: Dengweiwei
#		2020/10/19 21:22
#
root_dic = /home/dww01/ABECrypto

inc_schemes = $(root_dic)/schemes
inc_policy = $(root_dic)/accesspolicy/
inc_cryptoparam = $(root_dic)/cryptoparam/
inc_matrix = $(root_dic)/lsssmatrix/
inc_param = $(root_dic)/param/
inc_waters = $(inc_schemes)/waters11/

obj = $(root_dic)/obj
obj_schemes = $(obj)/schemes
obj_policy = $(obj)/accesspolicy/
obj_matrix = $(obj)/lsssmatrix/
obj_param = $(obj)/param/
obj_waters = $(obj_schemes)waters

link = -L. -lgmp -lpbc

waters11: waters11.o accesspolicy.o attsmaptoelement.o elementmatrix.o elementvector.o lsssmatrix.o abeparam.o curve_param.o
	g++ $(obj_waters)waters11.o $(obj_policy)accesspolicy.o $(obj_matrix)attsmaptoelement.o \
	$(obj_matrix)elementmatrix.o $(obj_matrix)elementvector.o $(obj_matrix)lsssmatrix.o     \
	$(obj_param)abeparam.o $(obj_param)curve_param.o -o $(obj_schemes)/waters11 $(link)

waters11.o:waters11.cpp
	g++ $(inc_waters)waters11.cpp -c -o $(obj_waters)waters11.o -I$(inc_policy) -I$(inc_param) -I$(inc_cryptoparam) -I$(inc_matrix) -I$(inc_waters)


accesspolicy.o:accesspolicy.cpp
	g++ $(inc_policy)accesspolicy.cpp -c -o $(obj_policy)accesspolicy.o -I$(inc_policy)

attsmaptoelement.o:attsmaptoelement.cpp
	g++ $(inc_matrix)attsmaptoelement.cpp -c -o $(obj_matrix)attsmaptoelement.o -I$(inc_matrix)

elementmatrix.o:elementmatrix.cpp
	g++ $(inc_matrix)elementmatrix.cpp -c -o $(obj_matrix)elementmatrix.o -I$(inc_policy) -I$(inc_matrix)

elementvector.o:elementvector.cpp
	g++ $(inc_matrix)elementvector.cpp -c -o $(obj_matrix)elementvector.o -I$(inc_matrix)

lsssmatrix.o:lsssmatrix.cpp
	g++ $(inc_matrix)lsssmatrix -c -o $(obj_matrix)lsssmatrix.o -I$(inc_matrix)

abeparam.o:abeparam.cpp
	g++ $(inc_param)abeparam.cpp -c -o $(obj_param)abeparam.o -I$(inc_param)

curve_param.o:curve_param.cpp
	g++ $(inc_param)curve_param -c -o $(obj_param)curve_param.o -I$(inc_param)