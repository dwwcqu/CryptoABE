inc_mmh = /home/dww01/ABECrypto/murmurhash/

obj = /home/dww01/ABECrypto/obj/rigbf/
obj_mmh = /home/dww01/ABECrypto/obj/murmurhash/

#link1 = -L. -lopencv_core -lopencv_dnn -lopencv_imgcodecs -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_gapi -lopencv_highgui
#link2 = -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio
# $(link1) $(link2)

#rigbf:rigbf.o
	#g++ $(obj)rigbf.o $(obj_mmh)MurmurHash3.o $(obj_mmh)hasher.o -o rigbf
	
rigbf.o:rigbf.cpp
	g++ rigbf.cpp -c -o $(obj)rigbf.o -I$(inc_mmh)