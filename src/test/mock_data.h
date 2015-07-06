#ifndef SYNTHESIZER_TEST_MOCK_H_
#define SYNTHESIZER_TEST_MOCK_H_
#include "reaction.h"
#include "model/stoichiometric_model.h"
#include "pgraph/pgraph.h"

namespace test
{
  model::StoichiometricModel* MakeTestStoichiometricModel1()
  {
    /*   r1 r2 r3 r4 r5 r6 r7 r8 r9 r10
     * A  1          -1 -1 -1
     * B     1        1       -1 -1
     * C                 1     1    -1
     * D                    1       -1
     * E
     * P
     * -
     * R 0  1   0  0  0  0  0  1  0  0
     */
    double const M[6][10] = {
        { 1, 0, 0, 0,-1,-1,-1, 0, 0, 0 }, //A
        { 0, 1, 0, 0, 1, 0, 0,-1,-1, 0 }, //B
        { 0, 0, 0, 0, 0, 1, 0, 1, 0,-1 }, //C
        { 0, 0, 0, 0, 0, 0, 1, 0, 0,-1 }, //D
        { 0, 0, 0,-1, 0, 0, 0, 0, 0, 1 }, //E
        { 0, 0,-1, 0, 0, 0, 0, 0, 1, 1 }, //F
    };
    int const R[10] =
        { 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 };
    double **a = 0;
    model::StoichiometricModel* model = new model::StoichiometricModel(6, 10, a);
    return model;
  }

  pgraph::ProcessGraph* MakeTestPgraph1()
  {
    /* A  B  C  D  E  F  G  H  I  J  K  L
     * 1  2  3  4  5  6  7  8  9  10 11 12
     */
    set<int> o1_l = { 3 }; set<int> o1_r = { 1, 6 };
    set<int> o2_l = { 4 }; set<int> o2_r = { 1, 2 };
    set<int> o3_l = { 5, 6 }; set<int> o3_r = { 3 };
    set<int> o4_l = { 6, 7 }; set<int> o4_r = { 3, 4 };
    set<int> o5_l = { 7, 8 }; set<int> o5_r = { 4 };
    set<int> o6_l = { 10 }; set<int> o6_r = { 6 };
    set<int> o7_l = { 11, 12 }; set<int> o7_r = { 8 };

    pgraph::OperatingUnit o1(1, &o1_l, &o1_r);
    pgraph::OperatingUnit o2(2, &o2_l, &o2_r);
    pgraph::OperatingUnit o3(3, &o3_l, &o3_r);
    pgraph::OperatingUnit o4(4, &o4_l, &o4_r);
    pgraph::OperatingUnit o5(5, &o5_l, &o5_r);
    pgraph::OperatingUnit o6(6, &o6_l, &o6_r);
    pgraph::OperatingUnit o7(7, &o7_l, &o7_r);

    vector<pgraph::OperatingUnit> O = { o1, o2, o3, o4, o5, o6, o7 };

    pgraph::ProcessGraph* pgraph = new pgraph::ProcessGraph(&O);
    pgraph->setMaterialAlias(1, "A");
    pgraph->setMaterialAlias(2, "B");
    pgraph->setMaterialAlias(3, "C");
    pgraph->setMaterialAlias(4, "D");
    pgraph->setMaterialAlias(5, "E");
    pgraph->setMaterialAlias(6, "F");
    pgraph->setMaterialAlias(7, "G");
    pgraph->setMaterialAlias(8, "H");
    pgraph->setMaterialAlias(9, "I");
    pgraph->setMaterialAlias(10, "J");
    pgraph->setMaterialAlias(11, "K");
    pgraph->setMaterialAlias(12, "L");

    return pgraph;
  }
} //namespace bio

#endif // SYNTHESIZER_TEST_MOCK