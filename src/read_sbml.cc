#include <iostream>
#include <cstdlib>
#include "sbml/SBMLTypes.h"

using std::cout;
using std::cerr;
using std::endl;

void foo()
{

}


int mains(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << endl << "Usage: cmd <sbml>" << endl;
    exit(0);
  }
  const char* path = argv[1];

  SBMLDocument* sbml;
  SBMLReader reader;

  sbml = reader.readSBML(path);

  sbml->printErrors(cerr);

  Model* model = sbml->getModel();

  for (unsigned int i = 0; i < model->getNumReactions(); i++)
  {
    Reaction* rxn = model->getReaction(i);
    cout << "[" << i << "] Reaction: " << rxn->getId() << endl;
    for (unsigned int j = 0; j < rxn->getNumProducts(); j++)
    {
      SpeciesReference* spi =  rxn->getProduct(j);
      cout << "\t" << spi->getId() << endl;
    }
    for (unsigned int j = 0; j < rxn->getNumReactants(); j++)
    {
    }
  }
  delete sbml;
  return 0;
}
