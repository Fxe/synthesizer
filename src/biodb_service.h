#ifndef SYNTHESIZER_BIODB_BIODB_SERVICE_H_
#define SYNTHESIZER_BIODB_BIODB_SERVICE_H_

#include "reaction.h"
#include <json/json.h>
#include <string>

using namespace::std;

namespace biodb {

bio::Metabolite parseMetaboliteFromJson(Json::Value base);
bio::Metabolite parseMetaboliteFromJsonString(const string json);

bio::Reaction wut2(Json::Value base);
bio::Reaction wut3(const string json);
bio::Reaction wut(const string filename);

class BiodbService
{
public:
  bio::Reaction getReaction(long id);
  bio::Reaction getReaction(string db, string entry);

  bio::Metabolite getMetabolite(string db, long id);
  bio::Metabolite getMetabolite(string db, string entry);
};



};



#endif
