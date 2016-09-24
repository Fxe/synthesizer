CXX=g++
MPICXX=mpic++
CPPFLAGS=-std=c++11 -O3
INCLUDE=-I$(GLPK_INCLUDE) -I$(JSONCPP_INCLUDE) -I./src
LIBRARY_PATH=-L$(GLPK_LIBRARY) -L$(JSONCPP_LIBRARY)
LIBS=-lglpk -lcurl -ljsoncpp

all: glpk glpk_mpi

glpk: glpk.cc.o reaction.cc.o model_glpk.cc.o glpk_lp.cc.o blas_utilities.cc.o blas_matrix.cc.o fba_adapter.cc.o stoichiometric_model.cc.o biodb_service.cc.o io.cc.o
	$(CXX) $(LIBRARY_PATH) $(LIBS) ./obj/glpk.cc.o ./obj/glpk_lp.cc.o ./obj/model_glpk.cc.o ./obj/reaction.cc.o ./obj/blas_utilities.cc.o ./obj/matrix.cc.o ./obj/fba_adapter.cc.o ./obj/stoichiometric_model.cc.o ./obj/biodb_service.cc.o ./obj/io.cc.o -o ./bin/glpk
glpk_mpi: glpk_mpi.cc.o reaction.cc.o model_glpk.cc.o glpk_lp.cc.o blas_utilities.cc.o
	$(MPICXX) $(LIBRARY_PATH) $(LIBS) ./obj/glpk_mpi.cc.o ./obj/glpk_lp.cc.o ./obj/model_glpk.cc.o ./obj/reaction.cc.o ./obj/blas_utilities.cc.o -o ./bin/glpk_mpi
glpk_mpi.cc.o:
	$(MPICXX) -c $(CPPFLAGS) $(INCLUDE) ./src/glpk_mpi.cc -o ./obj/glpk_mpi.cc.o
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
blas_matrix.cc.o:
	$(CXX) -c -DUNIX $(CPPFLAGS) $(INCLUDE) ./src/blas/matrix.cc -o ./obj/matrix.cc.o
fba_adapter.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/model/fba_adapter.cc -o ./obj/fba_adapter.cc.o
stoichiometric_model.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/model/stoichiometric_model.cc -o ./obj/stoichiometric_model.cc.o
biodb_service.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/biodb_service.cc -o ./obj/biodb_service.cc.o
io.cc.o:
	$(CXX) -c $(CPPFLAGS) $(INCLUDE) ./src/io/io.cc -o ./obj/io.cc.o
