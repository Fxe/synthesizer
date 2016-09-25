#include "biodb_service.h"
#include "reaction.h"
#include <sstream>
#include <fstream>
#include <json/json.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

using namespace biodb;
using namespace std;

std::string dataa;


bio::Reaction biodb::wut2(Json::Value base) {
  int id = base.get("id", -1).asInt();
  string entry = base.get("entry", "").asString();
  bio::Reaction rxn(id, entry);
  Json::Value lhs = base["leftStoichiometry"];
  Json::Value rhs = base["rightStoichiometry"];

  for (auto cpdId : lhs.getMemberNames())
  {
    double value = lhs.get(cpdId, 0.0).asDouble();
    rxn.add_lhs_stoichiometry(atol(cpdId.c_str()), value);
  }

  for (auto cpdId : rhs.getMemberNames())
  {
    double value = rhs.get(cpdId, 0.0).asDouble();
    rxn.add_rhs_stoichiometry(atol(cpdId.c_str()), value);
  }

  return rxn;
}

bio::Reaction biodb::wut(const string filename) {
  std::ifstream config_doc(filename, std::ifstream::binary);
  Json::Value base;
  config_doc >> base;

  bio::Reaction rxn = wut2(base);
  config_doc.close();

  return rxn;
}

bio::Reaction biodb::wut3(const string json) {
  Json::Reader reader;
  Json::Value base;
  bool parsingSuccessful = reader.parse(json.c_str(), base);
  if (!parsingSuccessful)
  {
    std::cout << "Failed to parse"
      << reader.getFormattedErrorMessages();
    return bio::Reaction();
  }

  bio::Reaction rxn = wut2(base);
  return rxn;
}

bio::Metabolite biodb::parseMetaboliteFromJson(Json::Value base) {
  int id = base.get("id", -1).asInt();
  string entry = base.get("entry", "").asString();
  bio::Metabolite cpd(id, entry);

  return cpd;
}

bio::Metabolite biodb::parseMetaboliteFromJsonString(const string json) {
  Json::Reader reader;
  Json::Value base;
  bool parsingSuccessful = reader.parse(json.c_str(), base);
  if (!parsingSuccessful)
  {
    std::cout << "Failed to parse"
      << reader.getFormattedErrorMessages();
    return bio::Metabolite();
  }

  bio::Metabolite cpd = parseMetaboliteFromJson(base);
  return cpd;
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
  //buf is a pointer to the data that curl has for us
  //size*nmemb is the size of the buffer
  for (int c = 0; c<size*nmemb; c++)
  {
    dataa.push_back(buf[c]);
  }
  return size*nmemb; //tell curl how many bytes we handled
}

bio::Reaction BiodbService::getReaction(long id) {
  dataa = "";
  std::cout << "BiodbService::getReaction " << id << std::endl;
  CURL* curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, "http://darwin.di.uminho.pt:8080/biosynth-web-biobase/api/dsa/BiGG/rxn/PYK");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_perform(curl);
  //std::cout << dataa << std::endl;
  bio::Reaction rxn = wut3(dataa);
  //std::cout << rxn << std::endl;

  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return rxn;
}

bio::Reaction BiodbService::getReaction(string db, string entry) {
  dataa = "";
  //std::cout << "BiodbService::getReaction " << db << "@" << entry << std::endl;
  CURL* curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  string query = "http://darwin.di.uminho.pt:8080/biosynth-web-biobase/api/dsa/" + db + "/rxn/" + entry;
  curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_perform(curl);
  //std::cout << "json " << dataa.size() << std::endl;
  if (dataa.size() == 0) {
    return bio::Reaction();
  }
  bio::Reaction rxn = wut3(dataa);
  //std::cout << rxn << std::endl;

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return rxn;
}

bio::Metabolite BiodbService::getMetabolite(string db, long id) {
  dataa = "";
  //std::cout << "BiodbService::getMetabolite " << db << "@" << id << std::endl;
  CURL* curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  ostringstream ss;
  ss << "http://darwin.di.uminho.pt:8080/biosynth-web-biobase/api/dsa/" << db << "/cpd/" << id;
  string query = ss.str();
  curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_perform(curl);
  //std::cout << dataa << std::endl;
  bio::Metabolite cpd = parseMetaboliteFromJsonString(dataa);
  //std::cout << rxn << std::endl;

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return cpd;
}

bio::Metabolite getMetabolite(string db, string entry) {
  dataa = "";
  std::cout << "BiodbService::getMetabolite " << db << "@" << entry << std::endl;
  CURL* curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  string query = "http://darwin.di.uminho.pt:8080/biosynth-web-biobase/api/dsa/" + db + "/cpd/" + entry;
  curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_perform(curl);
  //std::cout << dataa << std::endl;
  bio::Metabolite cpd = parseMetaboliteFromJsonString(dataa);
  //std::cout << rxn << std::endl;

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return cpd;
}