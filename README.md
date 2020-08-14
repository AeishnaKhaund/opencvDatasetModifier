# opencvDatasetModifier
Creating large datasets from a small pool of images


Build Instructions:
   -Any C++ compiler that supports c++17
   -Opencv4.X
   -ninja/make 
   -mkdir build && cd build
   -cmake -G"Ninja" ..

To Do:
1)Bring linux support -Done(credits @Mehul Gupta(sombr3ro)
2)Remove black patches when rotating image
3)add more filters in addition to contrast and brightness
4) csv loader becomes slower for larger datasets,implementing a faster csv reader
5)Only supports full hd images ,support for higher resolution images coming up soon!!
