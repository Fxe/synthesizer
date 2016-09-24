#ifndef SYNTHESIZER_BIODB_BIODB_SERVICE_H_
#define SYNTHESIZER_BIODB_BIODB_SERVICE_H_

#include "reaction.h"
#include <json\json.h>
#include <string>

using namespace::std;

namespace biodb {

bio::Reaction wut2(Json::Value base);
bio::Reaction wut3(const string json);
bio::Reaction wut(const string filename);

class BiodbService
{
public:
  void getReaction(long id);
  void getReaction(string db, string entry);
};



};



#endif