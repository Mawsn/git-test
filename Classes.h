#include <iostream>
#include <vector>
#include <random>
class Political_Party;
class Electorate;
class party_range{
private:
	const int significance_lowest;
	const int significance_highest;
	const int approach_lowest;
	const int approach_highest;
public:
	party_range(int significance_lowest,int significance_highest,int approach_lowest, int approach_highest): significance_lowest(significance_lowest), significance_highest(significance_highest), approach_lowest(approach_lowest),approach_highest(approach_highest){};
	int getSignificanceLowest(){return significance_lowest;}
	int getSignificanceHighest(){return significance_highest;}
	int getApproachLowest(){return approach_lowest;}
	int getApproachHighest(){return approach_highest;}
};

class Votes{
private:
	int labour_votes = 0;
	int greens_votes = 0;
	int liberal_votes = 0;
public:
	void voteLabour(int);
	void voteGreens(int);
	void voteLiberal(int);

	int getLabourVotes();
	int getGreensVotes();
	int getLiberalVotes();
	std::string determineWinner();
};
class Issue{
private:
	std::string description;
public:
	Issue(std::string description): description(description){};
	std::string getDescription();
};
class Stance{
private:
	int significance;
	int approach;
	Issue *issue;
	int held_by = 1;
public:
	Stance(int,int,Issue*);
	Stance(int,int,Issue*,int);
	int getSignificance();
	int getApproach();
	Issue getIssue();
	int getAmountHoldingStance();
	void setSignificance(int);
	void setApproach(int);
};

class Party_Members{
protected:
	std::string name;
	Political_Party *party;
protected:
	Party_Members(std::string);
public:
	std::string getName();
	void setParty(Political_Party*);
	Political_Party* getParty();
};
class Finnancial_Manager: public Party_Members{
private:
	int effectiveness;
public:
	Finnancial_Manager(std::string name,int effectiveness): effectiveness(effectiveness), Party_Members(name){};
	void earnMoney(Votes);
};
class Campaign_Manager: public Party_Members{
private:
	int reliability;
public:
	Campaign_Manager(std::string);
	int getReliability();
};

class Electorate_Manager: public Party_Members{
private:
	Electorate *assigned_electorate;
	double current_funding_available;
public:
	Electorate_Manager(std::string name);
	void setElectorate(Electorate*);
	Electorate* getElectorate();
	void setFunding(double);
	double getAvailableFunds();
	void spendFunding();
	void assessImpactofElectorateInvestment(double);
};

class Public_Politician: public Party_Members{
protected:
	int popularity;
	int controversy_level;
	std::vector<Stance> stances_on_issues;
public:
	Public_Politician(std::string name,std::vector<Issue*> issues, int popularity,int controversy_level);
	void setStances(std::vector<party_range>,std::vector<Issue*>);
	int getPopularity();
	void setPopularity(int);
	int getControversyLevel();
	void setControversyLevel(int);
	std::vector<Stance>& getStances();
};
class Electoral_Candidate: public Public_Politician{
private:
	Electorate *assigned_electorate;
public:
	Electoral_Candidate(std::string name,std::vector<Issue*> issues,int popularity,int controversy_level): Public_Politician(name,issues,popularity,controversy_level){};
	void setElectorate(Electorate*);
	Electorate* getElectorate();
};
class Leader: public Public_Politician{
public:
	Leader(std::string name,std::vector<Issue*> issues,int popularity,int controversy_level): Public_Politician(name,issues,popularity,controversy_level){};

};
class Political_Party{
private:
	std::vector<party_range> party_ranges;
	std::string name;

	Leader* leader;
	std::vector<Stance> party_stances;
	std::vector<Electoral_Candidate*> candidates;
	std::vector<Electorate_Manager*> electorate_managers;
	Campaign_Manager* campaign_manager;
	Finnancial_Manager* finnancial_manager;

	double funds;
public:
	Political_Party(std::string,Leader*,std::vector<Electoral_Candidate*>,std::vector<Electorate_Manager*>,Campaign_Manager*,Finnancial_Manager*,std::vector<Issue*>,std::vector<int>&);
	std::string getName();
	std::vector<Stance>& getStances();
	Leader* getLeader();
	std::vector<Electoral_Candidate*> getCandidates();
	std::vector<Electorate_Manager*> getElectorateManagers();
	Campaign_Manager* getCampaignManager();
	Finnancial_Manager* getFinnancialManager();
	std::vector<party_range> getRanges();
	void distributeFunding();
	void addFundsToParty(double);
	double getAvailableFunds();

};

class Electorate{
private:
	std::string name;
	int population;
	std::vector<Stance> stance_distribution;
	std::vector<Electoral_Candidate*> local_candidates;
	std::vector<Electorate_Manager*> local_managers;


	double labour_investment = 0;
	double greens_investment = 0;
	double liberal_investment = 0;

	Votes party_votes;
	Votes candidate_votes;

public:
	Electorate(std::string name,std::vector<Electoral_Candidate*> candidates,std::vector<Electorate_Manager*> managers,std::vector<Issue*>);
	std::string getName();
	int getPopulation();
	std::vector<Stance>& getStances();
	std::vector<Electoral_Candidate*> getLocalCandidates();
	void setLabourInvestment(double funds);
	void setGreensInvestment(double funds);
	void setLiberalInvestment(double funds);
	double getLabourInvestment();
	double getGreensInvestment();
	double getLiberalInvestment();
	void getPerspectivesOnParties(std::vector<Political_Party*>);
	void getPerspectivesOnCandidates();
	Votes getPartyVotes();
	Votes getCandidateVotes();
	std::string determinePartyWithMostInvestment();
};

class Local_Event{
private:
	Electorate *electorate;
	void chooseEventToBegin();
	void localDebate();
	void candidateRally();
	void candidateCommunityOutReach();
	void crimeRateIncrease();
	void regularTrafficJams();
public:
	Local_Event(Electorate*);
};
class National_Event{
private:
	std::vector<Electorate*> electorates;
	std::vector<Political_Party*> parties;

	void chooseEventToBegin();
	void leadersDebate();
	void leaderRally();
	void leaderSmearCampaign();
	void weekOfDeadSlowInternet();
	void birdApocalypse();
public:
	National_Event(std::vector<Electorate*>,std::vector<Political_Party*>);
};