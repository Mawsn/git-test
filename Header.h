#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include "Classes.h"
Issue* createNationalIssue(std::string);
void checkArgumentsExist(int);
void checkInputConstraints(int,int);
std::vector<int> createPartyRanges(std::string);
Leader* createLeader(std::string,std::vector<Issue*> issues);
Electoral_Candidate* createCandidate(std::string,std::vector<Issue*> issues);
Electorate_Manager* createElectorateManager(std::string);
Campaign_Manager* createCampaignManager(std::string);
Finnancial_Manager* createFinnancialManager(std::string);
std::string outputPartyBreakdown(std::vector<Political_Party*>&,std::vector<Electorate*>&,std::vector<Issue*>&);
void dayBeforeElectionReport(std::vector<Electorate*> &electorates,std::vector<Issue*> &issues,std::vector<Political_Party*> &parties);
void electionResultsReport(std::vector<Electorate*> &electorates,std::vector<Political_Party*>&);
void checkIfFilesOpened(std::ifstream&,std::ifstream&,std::ifstream&,std::ifstream&,std::ifstream&,std::ifstream&,std::ifstream&,std::ifstream&);
void clearMemory(std::vector<Electorate*>&,std::vector<Political_Party*>&,std::vector<Leader*>&,std::vector<Electoral_Candidate*>&,std::vector<Issue*>&,std::vector<Electorate_Manager*>&,std::vector<Campaign_Manager*>&,std::vector<Finnancial_Manager*>&);