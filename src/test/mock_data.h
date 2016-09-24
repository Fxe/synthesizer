#ifndef SYNTHESIZER_TEST_MOCK_H_
#define SYNTHESIZER_TEST_MOCK_H_
#include "reaction.h"
#include "blas/matrix.h"
#include "model/stoichiometric_model.h"
#include "pgraph/pgraph.h"

namespace test
{
  map<string, double>* GetEcoliCoreFluxes()
  {
    const int ITEMS = 77;
    const string rxn[ITEMS] = {
      "R_EX_pi_e_", "R_EX_h2o_e_", "R_EX_glc_e_", "R_EX_co2_e_", "R_EX_for_e_", 
      "R_EX_succ_e_", "R_EX_ac_e_", "R_EX_fum_e_", "R_EX_lac_D_e_", "R_EX_akg_e_", 
      "R_EX_pyr_e_", "R_EX_h_e_", "R_EX_etoh_e_", "R_EX_o2_e_", "R_FUM", "R_PDH", 
      "R_ICDHyr", "R_PYRt2r", "R_FORt", "R_ACONT", "R_TPI", "R_PTAr", "R_PPCK", 
      "R_ATPM", "R_PGI", "R_PGK", "R_ADK1", "R_PGM", "R_PGL", "R_H2Ot", "R_NADH11", 
      "R_ATPS4r", "R_ETOHt2r", "R_SUCOAS", "R_FUMt2_2", "R_GAPD", "R_CO2t", 
      "R_G6PDH2r", "R_PIt", "R_ICL", "R_O2t", "R_FBA", "R_AKGDH", "R_PPC", 
      "R_NADTRHD", "R_PFL", "R_PFK", "R_ACt2r", "R_TKT1", "R_TKT2", "R_SUCCt2_2", 
      "R_GLCpts", "R_D_LACt2", "R_SUCD4", "R_GND", "R_ENO", "R_ME1", "R_ME2", 
      "R_ADHEr", "R_CS", "R_Biomass_Ecoli_core_N__w_GAM_", "R_LDH_D", "R_MDH", 
      "R_THD2", "R_SUCCt2b", "R_ACKr", "R_AKGt2r", "R_CYTBD", "R_FRD", "R_MALS", 
      "R_RPI", "R_SUCD1i", "R_PYK", "R_PPS", "R_TALA", "R_RPE", "R_FBP" };
    const double flux[ITEMS] = {
      -5.27E-17, 0.450957, -0.0475871, 0.469464, 0.861407, 0.861407, 0.861407, 
      0.861407, 0.861407, 0.861407, 0.861407, 3.33E-17, 0.861407, 0.240825, 
      0.775079, 0.782938, 6.16E-32, 0.861407, 0.861407, -1.23E-30, 0.704037, 
      0.861407, 0.861407, 0.905523, 0.845693, 1.40E-16, 0.861407, -6.33E-18, 
      0.852611, 0.450957, 0.240825, 0.404474, 0.861407, 0.840372, 0.861407, 
      -1.65E-19, 0.469464, 0.852611, -1.03E-16, 0.861407, 0.240825, 0.704037, 
      0.840372, 0.85713, 0.861407, 0.861407, 0.704037, 0.861407, 0.853465, 
      0.855194, 0.861407, 0, 0.861407, 0.775079, 0.852611, -5.89E-18, 0.861407, 
      0.861407, 0.861407, 4.24E-30, 0, 0.861407, 0.804496, 0.861407, 0.861407, 
      0.861407, 0.861407, 0.240825, 0.861407, 0.861407, -1.63E-27, 0.775079, 
      0.850835, 0.861407, 0.853465, 0.854328, 0.861407 };
    map<string, double> flux_map;
    for (int i = 0; i < ITEMS; i++)
    {
      flux_map[rxn[i]] = flux[i];
    }

    return &flux_map;
  }

  blas::Matrix* MakeTestMatrix1()
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
        { 1, 0,  0,  0, -1, -1, -1,  0,  0,  0 }, //A
        { 0, 1,  0,  0,  1,  0,  0, -1, -1,  0 }, //B
        { 0, 0,  0,  0,  0,  1,  0,  1,  0, -1 }, //C
        { 0, 0,  0,  0,  0,  0,  1,  0,  0, -1 }, //D
        { 0, 0,  0, -1,  0,  0,  0,  0,  0,  1 }, //E
        { 0, 0, -1,  0,  0,  0,  0,  0,  1,  1 }, //F
    };
    blas::Matrix* matrix = new blas::Matrix(6, 10);
    for (int i = 0; i < 6; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        matrix->set_value(i, j, M[i][j]);
      }
    }
    return matrix;
  }

  model::StoichiometricModel* MakeTestStoichiometricModel1()
  {
    blas::Matrix* mat = MakeTestMatrix1();
    vector<double> lb = { 0, -5, 0, 0, 0, 0, 0, -5, 0, 0 };
    vector<double> ub = { 5,  5, 5, 5, 5, 5, 5,  5, 5, 5 };
    vector<string> cpd_alias = { "A", "B", "C", "D", "E", "F" };
    vector<string> rxn_alias = { "R1", "R2", "R3", "R4", "R5", 
                                 "R6", "R7", "R8", "R9", "R10" };
    model::StoichiometricModel* model = new model::StoichiometricModel(mat, 
                                            &lb, &ub, &cpd_alias, &rxn_alias);
    return model;
  }

  pgraph::OperatingUnit** MakeTestOua1()
  {
    pgraph::OperatingUnit** oua = new pgraph::OperatingUnit*[7];

    int ou1_lhs[] = { 3 };
    int ou1_rhs[] = { 1, 6 };
    int ou2_lhs[] = { 4 };
    int ou2_rhs[] = { 1, 2 };
    int ou3_lhs[] = { 5, 6 };
    int ou3_rhs[] = { 3 };
    int ou4_lhs[] = { 6, 7 };
    int ou4_rhs[] = { 3, 4 };
    int ou5_lhs[] = { 7, 8 };
    int ou5_rhs[] = { 4 };
    int ou6_lhs[] = { 10 };
    int ou6_rhs[] = { 6 };
    int ou7_lhs[] = { 11, 12 };
    int ou7_rhs[] = { 8 };
    pgraph::OperatingUnit* o1 = new pgraph::OperatingUnit(1, ou1_lhs, 1, ou1_rhs, 2);
    pgraph::OperatingUnit* o2 = new pgraph::OperatingUnit(2, ou2_lhs, 1, ou2_rhs, 2);
    pgraph::OperatingUnit* o3 = new pgraph::OperatingUnit(3, ou3_lhs, 2, ou3_rhs, 1);
    pgraph::OperatingUnit* o4 = new pgraph::OperatingUnit(4, ou4_lhs, 2, ou4_rhs, 2);
    pgraph::OperatingUnit* o5 = new pgraph::OperatingUnit(5, ou5_lhs, 2, ou5_rhs, 1);
    pgraph::OperatingUnit* o6 = new pgraph::OperatingUnit(6, ou6_lhs, 1, ou6_rhs, 1);
    pgraph::OperatingUnit* o7 = new pgraph::OperatingUnit(7, ou7_lhs, 2, ou7_rhs, 1);
    oua[0] = o1;
    oua[1] = o2;
    oua[2] = o3;
    oua[3] = o4;
    oua[4] = o5;
    oua[5] = o6;
    oua[6] = o7;
    return oua;
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