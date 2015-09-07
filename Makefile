CXX=g++
CPPFLAGS=-std=c++11 -O3
INCLUDE=-I$(GLPK_INCLUDE) -I./src
LIBRARY_PATH=-L$(GLPK_LIBRARY)
LIBS=-lglpk

all: glpk

glpk: glpk.cc.o reaction.cc.o model_glpk.cc.o glpk_lp.cc.o blas_utilities.cc.o
	$(CXX) $(LIBRARY_PATH) $(LIBS) ./obj/glpk.cc.o ./obj/glpk_lp.cc.o ./obj/model_glpk.cc.o ./obj/reaction.cc.o ./obj/blas_utilities.cc.o -o ./bin/glpk
glpk.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/glpk.cc -o ./obj/glpk.cc.o
glpk_lp.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/optmization/glpk_lp.cc -o ./obj/glpk_lp.cc.o
model_glpk.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/optmization/model_glpk.cc -o ./obj/model_glpk.cc.o
reaction.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/reaction.cc -o ./obj/reaction.cc.o
blas_utilities.cc.o:
	$(CXX) -c -DUNIX $(CPPFLAGS) $(INCLUDE) ./src/blas/blas_utilities.cc -o ./obj/blas_utilities.cc.o
