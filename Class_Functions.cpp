#include "Header.h"
using std::ifstream;
using std::stoi;
using std::invalid_argument;
using std::cerr;
using std::endl;
using std::cout;
using std::string;
using std::vector;
using std::uniform_int_distribution;
//Create random engine and distributions for different functions. These are set here so that they can be used wherever needed
std::random_device seed;
std::default_random_engine randEng(seed());
uniform_int_distribution<int> funds_rand(1000, 10000);
uniform_int_distribution<int> uniform(0, 10);
uniform_int_distribution<int> events(0,4);
uniform_int_distribution<int> population_rand(10000, 500000);
uniform_int_distribution<int> candidateRange(0,2);
uniform_int_distribution<int> leaderRange(0,2);

//Votes class functions
void Votes::voteLabour(int votes){this->labour_votes += votes;}
void Votes::voteGreens(int votes){this->greens_votes += votes;}
void Votes::voteLiberal(int votes){this->liberal_votes += votes;}
int Votes::getLabourVotes(){return labour_votes;}
int Votes::getGreensVotes(){return greens_votes;}
int Votes::getLiberalVotes(){return liberal_votes;}
string Votes::determineWinner(){
	//Based on the votes that have been input into the class, determine the winner. If there is a draw, declare there to be no winner!
	if(labour_votes > greens_votes && labour_votes > liberal_votes)
		return "Labour";
	if(greens_votes > labour_votes && greens_votes > liberal_votes)
		return "Greens";
	if(liberal_votes > labour_votes && liberal_votes > greens_votes)
		return "Liberal";
	else{
		return "None";
	}
}
//Issue class functions
string Issue::getDescription(){return description;}
//Stance class functions
Stance::Stance(int significance,int approach,Issue *issue){
	this->significance = significance;
	this->approach = approach;
	this->issue = issue;
	//If either significance or approach is less than 0, set it to 0, if it is greater than 10 set it to 10. This is done as all values in the program for stances center around the range 0 - 10.
	if(this->significance < 0)
		this->significance = 0;
	if(this->significance > 10)
		this->significance = 10;

	if(this->approach < 0)
		this->approach = 0;
	if(this->approach > 10)
		this->approach = 10;
}
Stance::Stance(int significance,int approach,Issue *issue,int held_by){
	//This constructor allows for information on how many people hold a stance. This is used within the electorate stance clustering system.
	this->significance = significance;
	this->held_by = held_by;
	this->approach = approach;
	this->issue = issue;
	if(this->significance < 0)
		this->significance = 0;
	if(this->significance > 10)
		this->significance = 10;

	if(this->approach < 0)
		this->approach = 0;
	if(this->approach > 10)
		this->approach = 10;
}
int Stance::getSignificance(){return significance;}
int Stance::getApproach(){return approach;}
Issue Stance::getIssue(){return *issue;}
int Stance::getAmountHoldingStance(){return held_by;}
void Stance::setSignificance(int num){
	this->significance += num;
	if(this->significance > 10)
		this->significance = 10;
	if(this->significance < 0)
		this->significance = 0;
}
void Stance::setApproach(int num){
	this->approach += num;
	if(this->approach > 10)
		this->approach = 10;
	if(this->approach < 0)
		this->approach = 0;
}
//Party Members class functions
Party_Members::Party_Members(string name): party(NULL){this->name = name;}
void Party_Members::setParty(Political_Party* party){this->party = party;}
Political_Party* Party_Members::getParty(){return party;}
string Party_Members::getName(){return name;}
//Public Politician class functions
Public_Politician::Public_Politician(string name,vector<Issue*> issues,int popularity,int controversy_level): Party_Members(name){
	this->name = name;
	this->popularity = popularity;
	this->controversy_level = controversy_level;
	//Set party to NULL as it will be initialised with a pointer when a politician is assigned to an electorate
	this->party = NULL;
}
void Public_Politician::setStances(vector<party_range> party_ranges,vector<Issue*> issues){
	for(int x = 0;x < issues.size();x++){
		uniform_int_distribution<int> significance_generator(party_ranges[x].getSignificanceLowest(),party_ranges[x].getSignificanceHighest());
		uniform_int_distribution<int> approach_generator(party_ranges[x].getApproachLowest(),party_ranges[x].getApproachHighest());

		int significance = significance_generator(randEng);
		int approach = approach_generator(randEng);
		stances_on_issues.push_back(Stance(significance,approach,issues[x]));
	}
}
vector<Stance>& Public_Politician::getStances(){return stances_on_issues;}
int Public_Politician::getControversyLevel(){return controversy_level;}
int Public_Politician::getPopularity(){return popularity;}
void Public_Politician::setControversyLevel(int num){
	this->controversy_level = this->controversy_level + num;
	
	if(this->controversy_level > 10)
		this->controversy_level = 10;
	if(this->controversy_level < 0)
		this->controversy_level = 0;
}
void Public_Politician::setPopularity(int num){
	this->popularity += num;
	if(this->popularity > 10)
		this->popularity = 10;
	if(this->popularity < 0)
		this->popularity = 0;
}
//Electoral_Candidate class functions 
void Electoral_Candidate::setElectorate(Electorate* e){this->assigned_electorate = e;}
Electorate* Electoral_Candidate::getElectorate(){return assigned_electorate;}
//Electorate Manager Class functions
Electorate_Manager::Electorate_Manager(string name): Party_Members(name){}
void Electorate_Manager::setElectorate(Electorate* e){this->assigned_electorate = e;}
Electorate* Electorate_Manager::getElectorate(){return assigned_electorate;}
void Electorate_Manager::setFunding(double funds){this->current_funding_available = funds;}
double Electorate_Manager::getAvailableFunds(){return current_funding_available;}
void Electorate_Manager::spendFunding(){
	//Spend funding on the electorate assigned to the manager. Based on their party, the setPartyInvestment for their electorate will be called and the amount they have invested will be logged by the electorate
	if(party->getName() == "Labour")
		assigned_electorate->setLabourInvestment(current_funding_available);
	if(party->getName() == "Greens")
		assigned_electorate->setGreensInvestment(current_funding_available);
	if(party->getName() == "Liberal")
		assigned_electorate->setLiberalInvestment(current_funding_available);
	//Based on the funding a manager has given to their electorate, call the assess impact function
	assessImpactofElectorateInvestment(current_funding_available);
}
void Electorate_Manager::assessImpactofElectorateInvestment(double funding_provided){
	//This function is used to give the party extra votes based on how much they have invested into the electorate
	const double rate = 2;
	int bonus_votes_from_funding = 0;
	//Based on their party, call the getParty votes function of the electorate is used to add additional votes/gain support for the party
	if(party->getName() == "Labour"){
		bonus_votes_from_funding = lround(funding_provided/assigned_electorate->getPopulation() * rate);
		assigned_electorate->getPartyVotes().voteLabour(bonus_votes_from_funding);
	}
	if(party->getName() == "Greens"){
		bonus_votes_from_funding = lround(funding_provided/assigned_electorate->getPopulation() * rate);
		assigned_electorate->getPartyVotes().voteGreens(bonus_votes_from_funding);
	}
	if(party->getName() == "Liberal"){
		bonus_votes_from_funding = lround(funding_provided/assigned_electorate->getPopulation() * rate);
		assigned_electorate->getPartyVotes().voteLiberal(bonus_votes_from_funding);
	}
}
//Campaign Manager class functions
Campaign_Manager::Campaign_Manager(string name): Party_Members(name){this->reliability = uniform(randEng);}
int Campaign_Manager::getReliability(){return reliability;}
//Finanncial Manager class functions
void Finnancial_Manager::earnMoney(Votes party_votes){
	//Money earned calculation will be based on the votes/support a party has earnt thus so far and the effectiveness of the finanncial manager
	double rate = 0.0025;

	if(party->getName() == "Labour")
		party->addFundsToParty(((effectiveness * party_votes.getLabourVotes()) * rate));
	if(party->getName() == "Greens")
		party->addFundsToParty((effectiveness * party_votes.getGreensVotes()) * rate);
	if(party->getName() == "Liberal")
		party->addFundsToParty((effectiveness * party_votes.getLiberalVotes()) * rate);
}
//Political Party class functions
Political_Party::Political_Party(string name,Leader* leader,vector<Electoral_Candidate*> candidates,vector<Electorate_Manager*> electorate_managers,Campaign_Manager* campaign_manager,Finnancial_Manager* finnancial_manager,vector<Issue*> issues,vector<int> &range){
	this->name = name;
	this->leader = leader;
	this->candidates = candidates;
	this-> electorate_managers = electorate_managers;
	this->campaign_manager = campaign_manager;
	this->finnancial_manager = finnancial_manager;
	this->funds = funds_rand(randEng);
	//For each member in the party, use their setParty() function to associate the party with each member.
	leader->setParty(this);
	campaign_manager->setParty(this);
	finnancial_manager->setParty(this);
	for(int x = 0;x < candidates.size();x++)
		candidates[x]->setParty(this);
	for(int x = 0;x < electorate_managers.size();x++)
		electorate_managers[x]->setParty(this);

	//Create party_range classes based on the party ranges read from the file
	int count = 0;
	for(int x = 0;x < issues.size();x++){
		int sig_lower = range[count + 0];
		int sig_upper = range[count + 1];
		int app_lower = range[count + 2];
		int app_upper = range[count + 3];
		count += 4;
		party_ranges.push_back(party_range(sig_lower,sig_upper,app_lower,app_upper));
	}
	//Based on the range, generate initial stance values!
	for(int x = 0;x < issues.size();x++){
		uniform_int_distribution<int> significance_generator(party_ranges[x].getSignificanceLowest(),party_ranges[x].getSignificanceHighest());
		uniform_int_distribution<int> approach_generator(party_ranges[x].getApproachLowest(),party_ranges[x].getApproachHighest());

		int significance = significance_generator(randEng);
		int approach = approach_generator(randEng);
		party_stances.push_back(Stance(significance,approach,issues[x]));
	}

	leader->setStances(party_ranges,issues);
	for(int x = 0;x < candidates.size();x++)
		candidates[x]->setStances(party_ranges,issues);
}
string Political_Party::getName(){return name;}
Leader* Political_Party::getLeader(){return leader;}
vector<Electoral_Candidate*> Political_Party::getCandidates(){return candidates;}
Campaign_Manager* Political_Party::getCampaignManager(){return campaign_manager;}
Finnancial_Manager* Political_Party::getFinnancialManager(){return finnancial_manager;}
vector<Electorate_Manager*> Political_Party::getElectorateManagers(){return electorate_managers;}
vector<Stance>& Political_Party::getStances(){return party_stances;}
void Political_Party::distributeFunding(){
	//Evenly distribute available funding amongst each electorate manager in the party.
	double divided_funds = funds / electorate_managers.size();
	for(int x = 0;x < electorate_managers.size();x++)
		electorate_managers[x]->setFunding(divided_funds);
	funds = 0;
}
void Political_Party::addFundsToParty(double new_funds){this->funds += new_funds;}
double Political_Party::getAvailableFunds(){return funds;}
vector<party_range> Political_Party::getRanges(){return party_ranges;}
//Electorate class functions
Electorate::Electorate(string name,vector<Electoral_Candidate*> candidates,vector<Electorate_Manager*> managers,vector<Issue*> issues){
	this->name = name;
	this->population = 2 * population_rand(randEng);
	this->local_candidates = candidates;
	this->local_managers = managers;
	//For each manager and candidate assigned to the electorate, associate them with their assigned electorate.
	for(int x = 0;x < candidates.size();x++)
		candidates[x]->setElectorate(this);
	for(int x = 0;x < managers.size();x++)
		managers[x]->setElectorate(this);
	//Create 6 six clusters, these will be used to represent an electorates stance on each issue.
	const int number_of_clusters = 6;
	int people_per_cluster = population/number_of_clusters;
	//For each issue, and for each cluster, generate a random approach and significance value. 
	for(int x = 0;x < issues.size();x++){
		for(int y = 0;y < number_of_clusters;y++){
			int significance = uniform(randEng);
			int approach = uniform(randEng);
			//Add this cluster stance into the stance_distribution vector. Assign a number of people to the stance 
			stance_distribution.push_back(Stance(significance,approach,issues[x],people_per_cluster));
		}
	}	
}
string  Electorate::getName(){return name;}
int Electorate::getPopulation(){return population;}
vector<Stance>& Electorate::getStances(){return stance_distribution;}
vector<Electoral_Candidate*> Electorate::getLocalCandidates(){return local_candidates;}
void Electorate::setLabourInvestment(double f){this->labour_investment += f;}
void Electorate::setGreensInvestment(double f){this->greens_investment += f;}
void Electorate::setLiberalInvestment(double f){this->liberal_investment += f;}
double Electorate::getLabourInvestment(){return labour_investment;}
double Electorate::getGreensInvestment(){return greens_investment;}
double Electorate::getLiberalInvestment(){return liberal_investment;}
void Electorate::getPerspectivesOnParties(vector<Political_Party*> parties){
	int votes = 0;
	int count = 0;
	//For each of the 30 stances...
	for(int x = 0;x < stance_distribution.size();x++){
		//Check how significant a stance is to the cluster and it's approach
		int electorate_significance =  stance_distribution[x].getSignificance();
		int electorate_approach = stance_distribution[x].getApproach();

		int party_significance = 0;
		int party_approach = 0;
		//For each party...
		for(int y = 0;y < parties.size();y++){
			//Check how significant the party thinks the issue is
			party_significance = parties[y]->getStances()[count].getSignificance();
			party_approach = parties[y]->getStances()[count].getApproach();
			int temp_votes = 0;
			//Check whether the leader of each party is popular or controversial, if they are popular and not too controvesial, it will convince more people to vote for their party. If not they will obtain less votes as a consequence!
			//Next, check how closely each cluster aligns with the party's stance on each national issue. If they have the same view, the party will earn some votes/support based on the amount of people represented within the stance cluster
			if(parties[y]->getLeader()->getPopularity() > 5 && parties[y]->getLeader()->getControversyLevel() < 6){
				//If the party's significance is the same as the electorate clusters significance value, assign some votes
				if(party_significance == electorate_significance)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.005;
				//If the party's approach is the same as the electorate clusters approach value, assign some more votes
				if(party_approach == electorate_approach)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.0005;
			}
			else{
				if(party_significance == electorate_significance)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.00025;
				if(party_approach == electorate_approach)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.00025;
			}
			//Based on the votes accumulated above, assign the votes with the party_votes variable within the electorate
			if(parties[y]->getName() == "Labour")
				party_votes.voteLabour(temp_votes);
			if(parties[y]->getName() == "Greens")
				party_votes.voteGreens(temp_votes);
			if(parties[y]->getName() == "Liberal")
				party_votes.voteLiberal(temp_votes);
		}
		count++;
		if(count == 5)
			count = 0;
	}
}
void Electorate::getPerspectivesOnCandidates(){
	int count = 0;

	for(int x = 0;x < stance_distribution.size();x++){
		int electorate_significance =  stance_distribution[x].getSignificance();
		int electorate_approach = stance_distribution[x].getApproach();

		int candidate_significance = 0;
		int candidate_approach = 0;

		for(int y = 0;y < local_candidates.size();y++){
			candidate_significance = local_candidates[y]->getStances()[count].getSignificance();
			candidate_approach = local_candidates[y]->getStances()[count].getApproach();
			int temp_votes = 0;

			if(local_candidates[y]->getPopularity() > 5 && local_candidates[y]->getControversyLevel() < 6){
				
				if(candidate_significance == electorate_significance)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.005;
				if(candidate_approach == electorate_approach)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.005;
			}
			else{
				if(candidate_significance == electorate_significance)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.00025;
				if(candidate_approach == electorate_approach)
					temp_votes += stance_distribution[x].getAmountHoldingStance() * 0.00025;
			}
			
			if(local_candidates[y]->getParty()->getName() == "Labour")
				candidate_votes.voteLabour(temp_votes);
			if(local_candidates[y]->getParty()->getName() == "Greens")
				candidate_votes.voteGreens(temp_votes);
			if(local_candidates[y]->getParty()->getName() == "Liberal")
				candidate_votes.voteLiberal(temp_votes);
		}
		count++;
		if(count == 5)
			count = 0;
	}
}
Votes Electorate::getPartyVotes(){return party_votes;}
Votes Electorate::getCandidateVotes(){return candidate_votes;}
string Electorate::determinePartyWithMostInvestment(){
	if(getLabourInvestment() > getGreensInvestment() && getLabourInvestment() > getLiberalInvestment()){
		return "Labour";
	}
	if(getGreensInvestment() > getLabourInvestment() && getGreensInvestment() > getLiberalInvestment()){
		return "Greens";
	}
	if(getLiberalInvestment() > getLabourInvestment() && getLiberalInvestment() > getGreensInvestment()){
		return "Liberal";
	}
	else{
		return "None";
	}

}

//*************************************EVENTS***********************************************//


Local_Event::Local_Event(Electorate* electorate){
	//Associate the event with the electorate
	this->electorate = electorate;
	//If the random number chosen is 9 or 10 (20% chance), call the choose event to begin function
	int number = uniform(randEng);
		if(number > 8){
		chooseEventToBegin();
	}
}
void Local_Event::chooseEventToBegin(){
	int event_selector = uniform(randEng);
	//Based on a random number start a local event.
	if(event_selector >= 0 && event_selector <= 2){
		localDebate();
	}
	if(event_selector >= 3 && event_selector <= 5){
		candidateRally();
	}
	if(event_selector == 10){
		candidateCommunityOutReach();
	}
	if(event_selector >= 6 && event_selector <= 8){
		crimeRateIncrease();
	}
	if(event_selector == 9){
		regularTrafficJams();
	}
}
void Local_Event::localDebate(){
	int winner = candidateRange(randEng);
		cout << "A local debate took place in " << electorate->getName() << ", the agreed winner of this debate was " << electorate->getLocalCandidates()[winner]->getName() << " (" << electorate->getLocalCandidates()[winner]->getParty()->getName() << ") ";
	if(electorate->getLocalCandidates()[winner]->getControversyLevel() < 5){
		if(electorate->getLocalCandidates()[winner]->getParty()->getName() == electorate->determinePartyWithMostInvestment()){
			cout << " and due to thier party having invested the most money of all into the electorate and not being overtly controversial, their popularity increased by 20% and they became 10% less controversial!" << endl;
			electorate->getLocalCandidates()[winner]->setPopularity(2);
			electorate->getLocalCandidates()[winner]->setControversyLevel(-1);
		}
		else{
			electorate->getLocalCandidates()[winner]->setPopularity(1);
			cout << " and due to thier party having invested the most money of all into the electorate and not being overtly controversial, their popularity increased by 10%!" << endl;
		}
	}
	else{
		electorate->getLocalCandidates()[winner]->setPopularity(-1);
		electorate->getLocalCandidates()[winner]->setControversyLevel(1);
		cout << "and due to them being quite controversial, they became less popular and more controversial!" << endl;
	}
}
void Local_Event::candidateRally(){
	int host = candidateRange(randEng);
	int chance = uniform(randEng);
	cout << "The candidate " << electorate->getLocalCandidates()[host]->getName() << " (" << electorate->getLocalCandidates()[host]->getParty()->getName() << ") held a rally in " << electorate->getName();
	if(chance >= 5){
		if(electorate->getLocalCandidates()[host]->getParty()->getName() == electorate->determinePartyWithMostInvestment()){
			cout << ". Due to their party having invested the most money, they became 20% more popular and 20% less controversial!" << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(-2);
			electorate->getLocalCandidates()[host]->setPopularity(2);
		}
		else{
			cout << ". They became 10% more popular and 10% less controversial!" << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(-1);
			electorate->getLocalCandidates()[host]->setPopularity(1);	
		}
	}
	else{
		if(electorate->getLocalCandidates()[host]->getParty()->getName() == electorate->determinePartyWithMostInvestment()){
			cout << ". Due to their party having invested the most money, they became 0% more popular and 0% less controversial!" << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(0);
			electorate->getLocalCandidates()[host]->setPopularity(0);
		}
		else{
			cout << ". The rally went poorly and they became 10% less popular and 10% more controversial." << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(1);
			electorate->getLocalCandidates()[host]->setPopularity(-1);
		}
	}
}
void Local_Event::candidateCommunityOutReach(){
	int host = candidateRange(randEng);
	cout << electorate->getLocalCandidates()[host]->getName() << " (" <<  electorate->getLocalCandidates()[host]->getParty()->getName() << ") " << " held a community outreach and met with many locals! ";
	
	if(electorate->getLocalCandidates()[host]->getControversyLevel() >= 5){
		if(electorate->getLocalCandidates()[host]->getParty()->getName() == electorate->determinePartyWithMostInvestment()){
			cout << "Prior to this outreach, this candidate was quite controversial. But due to their party investing the most in the electorate and the outreach, they became 30% less controversial and 30% more popular!" << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(-3);
			electorate->getLocalCandidates()[host]->setPopularity(3);
		}
		else{
			cout << "Prior to this outreach, this candidate was quite controversial. But due to the outreach, they became 20% less controversial and 20% more popular!" << endl;
			electorate->getLocalCandidates()[host]->setControversyLevel(-2);
			electorate->getLocalCandidates()[host]->setPopularity(2);
		}
	}
	else{
		cout << "Since this candidate was not very controversial, the outreach boosted their popularity by only 10%!" << endl;
		electorate->getLocalCandidates()[host]->setPopularity(1);
	}
}
void Local_Event::crimeRateIncrease(){
	cout << "Crime rates have increased in " << electorate->getName() << endl;
	int candidate_significance_approach_increment = 0;
	int electorate_significance_approach_increment = 0;
	vector<int> increments;

	int labour_count = 0;
	int greens_count = 0;
	int liberal_count = 0;
	//For each of the local candidates representing the electorate
	for(int x = 0;x < electorate->getLocalCandidates().size();x++){
		//Generate a random significance and approach increase for each candidate. So, if a candidate previosuly thought an issue was 3/10 important (30%), then the event(randEng) maybe produce a 3 and thus increase their significance on the issue to 6/10
		candidate_significance_approach_increment = events(randEng);
		//Log the increment made so that later it can be compared to the each of the clusters deemed significance of crime rates.
		increments.push_back(candidate_significance_approach_increment);
		electorate->getLocalCandidates()[x]->getStances()[0].setSignificance(candidate_significance_approach_increment);
		electorate->getLocalCandidates()[x]->getStances()[0].setApproach(candidate_significance_approach_increment);
	}
	//For each stance held by the electorate, total is 30 as (5 issues * 6 clusters = 30)
	for(int y = 0;y < electorate->getStances().size();y++){
		//If the stance is on Crime Rates... do the following. This is repeated 6 times based on the cluster system
		if(electorate->getStances()[y].getIssue().getDescription() == "Crime Rates"){
			//Generate a significance and approach increment for the stance cluster
			electorate_significance_approach_increment = events(randEng);
			electorate->getStances()[y].setSignificance(electorate_significance_approach_increment);
			electorate->getStances()[y].setApproach(electorate_significance_approach_increment);
			//For each of the candidates (their incremented significance/approach value)
			for(int z = 0;z < increments.size();z++){
				//If the electorate has the same increase in significance and approach, increment party_count by 1. 
				if(electorate_significance_approach_increment == increments[z]){
					if(z == 0)
						labour_count++;
					if(z == 1)
						greens_count++;
					if(z == 2)
						liberal_count++;
				}
				//If half of all clusters within an electorate agree, increment the popularity of the candidate. Set the party_count to zero to restart the process for the next run of the for loop
				if(labour_count >= 3){
					electorate->getLocalCandidates()[0]->setPopularity(2);
					cout << "Due to " << electorate->getLocalCandidates()[0]->getName() << " (" << electorate->getLocalCandidates()[0]->getParty()->getName() << ") agreeing on what should be done to address rising crime rates with many people in the electorate " << electorate->getName() << " they became 20% more popular!" << endl;
					labour_count = 0;
				}
				if(greens_count >= 3){
					electorate->getLocalCandidates()[1]->setPopularity(2);
					cout << "Due to " << electorate->getLocalCandidates()[1]->getName() << " (" << electorate->getLocalCandidates()[1]->getParty()->getName() << ") agreeing on what should be done to address rising crime rates with many people in the electorate " << electorate->getName() << " they became 20% more popular!" << endl;
					greens_count = 0;
				}			
				if(liberal_count >= 3){
					electorate->getLocalCandidates()[2]->setPopularity(2);
					cout << "Due to " << electorate->getLocalCandidates()[2]->getName() << " (" << electorate->getLocalCandidates()[2]->getParty()->getName() << ") agreeing on what should be done to address rising crime rates with many people in the electorate " << electorate->getName() << " they became 20% more popular!" << endl;
					liberal_count = 0;
				}
			}

		}
	}
}
//Comments made for crimeRateIncrease apply the same way for regularTrafficJams function
void Local_Event::regularTrafficJams(){
	cout << "Regular Traffic Jams have been occuring in " << electorate->getName() << endl;
	int candidate_significance_approach_increment = 0;
	int electorate_significance_approach_increment = 0;
	vector<int> increments;

	int labour_count = 0;
	int greens_count = 0;
	int liberal_count = 0;

	for(int x = 0;x < electorate->getLocalCandidates().size();x++){
		candidate_significance_approach_increment = events(randEng);
		increments.push_back(candidate_significance_approach_increment);

		electorate->getLocalCandidates()[x]->getStances()[0].setSignificance(candidate_significance_approach_increment);
		electorate->getLocalCandidates()[x]->getStances()[0].setApproach(candidate_significance_approach_increment);
	}
	for(int y = 0;y < electorate->getStances().size();y++){
		
		if(electorate->getStances()[y].getIssue().getDescription() == "Road Infastructure"){
			electorate_significance_approach_increment = events(randEng);
			electorate->getStances()[y].setSignificance(electorate_significance_approach_increment);
			electorate->getStances()[y].setApproach(electorate_significance_approach_increment);
			for(int z = 0;z < increments.size();z++){
				if(electorate_significance_approach_increment == increments[z]){
					if(z == 0)
						labour_count++;
					if(z == 1)
						greens_count++;
					if(z == 2)
						liberal_count++;
				}
				if(labour_count >= 3){
					electorate->getLocalCandidates()[0]->setPopularity(1);
					cout << "Due to " << electorate->getLocalCandidates()[0]->getName() << " (" << electorate->getLocalCandidates()[0]->getParty()->getName() << ") agreeing on what should be done to address frequent traffic jams with many people in the electorate " << electorate->getName() << " they became 10% more popular!" << endl;
					labour_count = 0;
				}
				if(greens_count >= 3){
					electorate->getLocalCandidates()[1]->setPopularity(1);
					cout << "Due to " << electorate->getLocalCandidates()[0]->getName() << " (" << electorate->getLocalCandidates()[0]->getParty()->getName() << ") agreeing on what should be done to address rising crime rates with many people in the electorate " << electorate->getName() << " they became 10% more popular!" << endl;
					greens_count = 0;
				}			
				if(liberal_count >= 3){
					electorate->getLocalCandidates()[2]->setPopularity(1);
					cout << "Due to " << electorate->getLocalCandidates()[0]->getName() << " (" << electorate->getLocalCandidates()[0]->getParty()->getName() << ") agreeing on what should be done to address rising crime rates with many people in the electorate " << electorate->getName() << " they became 10% more popular!" << endl;
					liberal_count = 0;
				}
			}
		}
	}
}
National_Event::National_Event(vector<Electorate*> electorates,vector<Political_Party*> parties){
	this->electorates = electorates;
	this->parties = parties;
	//Capture the vector of parties and electorates for association with the national_event (parties and electorates will be affected by these events)
	int number = uniform(randEng);
	//30% chance of a national event occuring as, if number is either 5,6 or 7 then this if block will be reached
	if(number < 8 && number >= 5){
		chooseEventToBegin();
	}
}
void National_Event::chooseEventToBegin(){
	int event_selector = uniform(randEng);
	if(event_selector >= 0 && event_selector <= 2){
		leadersDebate();
	}
	if(event_selector >= 3 && event_selector <= 6){
		leaderRally();
	}
	if(event_selector == 10){
		leaderSmearCampaign();
	}
	if(event_selector >= 7 && event_selector <= 8){
		weekOfDeadSlowInternet();
	}
	if(event_selector == 9){
		birdApocalypse();
	}
}
void National_Event::leadersDebate(){
	int debate_winner = leaderRange(randEng);
	int chance = uniform(randEng);
	//Choose a random debate "winner". Create a random chance number.
	cout << parties[debate_winner]->getLeader()->getName() << " (" << parties[debate_winner]->getLeader()->getParty()->getName() << ") has won a debate against the leaders of all other parties!";
	//Based on the chance value, a leaders popularity may increase and their controversy level may decrease. There is a chance nothing will happen if the leader is unlucky!
	if(chance >= 8){
		cout << " This has resulted in " << parties[debate_winner]->getLeader()->getName() << " becoming 30% more popular and 30% less controversial!" << endl;
		parties[debate_winner]->getLeader()->setPopularity(3);
		parties[debate_winner]->getLeader()->setControversyLevel(-3);
	}
	if(chance >= 5 && chance < 8){
		cout << " This has resulted in " << parties[debate_winner]->getLeader()->getName() << " becoming 20% more popular and 10% less controversial!" << endl;
		parties[debate_winner]->getLeader()->setPopularity(2);
		parties[debate_winner]->getLeader()->setControversyLevel(-1);
	}
	if(chance < 5 && chance > 0){
		cout << " This has resulted in " << parties[debate_winner]->getLeader()->getName() << " becoming 10% more popular and 10% less controversial!" << endl;
		parties[debate_winner]->getLeader()->setPopularity(1);
		parties[debate_winner]->getLeader()->setControversyLevel(-1);
	}
	if(chance == 0){
		cout << " This has resulted in " << parties[debate_winner]->getLeader()->getName() << " becoming 0% more popular and 0% less controversial!" << endl;
		parties[debate_winner]->getLeader()->setPopularity(0);
		parties[debate_winner]->getLeader()->setControversyLevel(0);
	}
}
void National_Event::leaderRally(){
	int host = leaderRange(randEng);
	int chance = uniform(randEng);
	//Choose a random leader who will be hosting the rally
	cout << parties[host]->getLeader()->getName() << " (" << parties[host]->getLeader()->getParty()->getName() << ") held a national rally! ";
	//If the party's campaign/national manager is good at their job, they will achieve better results due to the rally
	//This function is also based on chance but overall, if the party has a good campaign manager, then they will do well. If they do not then they could actually become less popular!
	if(parties[host]->getCampaignManager()->getReliability() >= 5){
		if(chance >= 9){
			cout << "Due to the party's national manager being exceptionally reliable, " << parties[host]->getLeader()->getName() << " has become 30% more popular and 30% less controversial" << endl;
			parties[host]->getLeader()->setPopularity(3);
			parties[host]->getLeader()->setControversyLevel(-3);
		}
		if(chance < 9 && chance >= 4){
			cout << "Due to the party's national manager being exceptionally reliable, " << parties[host]->getLeader()->getName() << " has become 10% more popular and 10% less controversial" << endl;
			parties[host]->getLeader()->setPopularity(1);
			parties[host]->getLeader()->setControversyLevel(-1);
		}
		if(chance < 4){
			cout << "Due to the party's national manager being exceptionally reliable, " << parties[host]->getLeader()->getName() << " has become 0% more popular and 0% less controversial" << endl;
			parties[host]->getLeader()->setPopularity(0);
			parties[host]->getLeader()->setControversyLevel(0);
		}
	}
	if(parties[host]->getCampaignManager()->getReliability() < 5){
		if(chance >= 9){
			cout << "" << parties[host]->getLeader()->getName() << " has become 10% more popular and 10% less controversial as a result!" << endl;
			parties[host]->getLeader()->setPopularity(1);
			parties[host]->getLeader()->setControversyLevel(-1);
		}
		if(chance < 9 && chance >= 4){
			cout << "" << parties[host]->getLeader()->getName() << " has become 0% more popular and 0% less controversial as a result!" << endl;
			parties[host]->getLeader()->setPopularity(0);
			parties[host]->getLeader()->setControversyLevel(0);
		}
		if(chance < 4){
			cout << "" << parties[host]->getLeader()->getName() << " has become 10% less popular and 10% more controversial as a result!" << endl;
			parties[host]->getLeader()->setPopularity(-1);
			parties[host]->getLeader()->setControversyLevel(1);
		}
	}
}
void National_Event::leaderSmearCampaign(){
	int unlucky_party = leaderRange(randEng);
	cout << parties[unlucky_party]->getLeader()->getName() << " has been exposed in a smear campaign!" << endl;
	if(parties[unlucky_party]->getCampaignManager()->getReliability() >= 5){
		parties[unlucky_party]->getLeader()->setPopularity(-3);
		parties[unlucky_party]->getLeader()->setControversyLevel(3);
	}
	else{
		parties[unlucky_party]->getLeader()->setPopularity(-5);
		parties[unlucky_party]->getLeader()->setControversyLevel(5);
	}
	
}
void National_Event::weekOfDeadSlowInternet(){
	int party_significance_and_approach_increment = 0;
	int electorate_significance_and_approach_increment = 0;
	vector<int> increments;
	cout << "The internet has been absolutely shocking this week across the ENTIRE nation! " << endl;
	int labour_count = 0;
	int greens_count = 0;
	int liberal_count = 0;
	//Generate a random increment number
	//For each party, add the increment to their approach and their significance value
	//For each electorate, add an increment to their significance and approach on high speed internet
	for(int x = 0;x < parties.size();x++){
		party_significance_and_approach_increment = events(randEng);
		
		increments.push_back(party_significance_and_approach_increment);
		
		parties[x]->getStances()[1].setSignificance(party_significance_and_approach_increment);
		parties[x]->getStances()[1].setApproach(party_significance_and_approach_increment);
	//FOR EACH ELECTORATE
	for(int y = 0;y < electorates.size();y++){
		//FOR EACH OF THE SiX CLUSTERS
		for(int z = 0;z < electorates[y]->getStances().size();z++){
			//FOR THE CLUSTERS ON INTERNET ISSUE
			if(electorates[y]->getStances()[z].getIssue().getDescription() == "High Speed Internet Access"){
				electorate_significance_and_approach_increment = events(randEng);
				electorates[y]->getStances()[z].setSignificance(electorate_significance_and_approach_increment);
				electorates[y]->getStances()[z].setApproach(electorate_significance_and_approach_increment);
				//CHECK EACH OF THE PARTIES CHANGE IN VIEWS, IF THE MOST CLUSTERS IN THE ELECTORATE SHARE THE SAME VIEW, THEN THE LOCAL MEMBER OF THAT PARTY WILL BECOME MORE POPULAR AND LESS CONTROVERSIAL
				for(int q = 0;q < increments.size();q++){
					if(electorate_significance_and_approach_increment == increments[q]){
						if(q == 0)
							labour_count++;
						if(q == 1)
							greens_count++;
						if(q == 2)
							liberal_count++;
					}
					//IF 3 OR MORE CLUSTERS IN AN ELECTORATE SHARE THE SAME VIEW AS THE PARTY, THE LOCAL CANDIDATE FOR THAT ELECTORATE WILL BECOME MORE FAVOURED DEPENDANT ALSO ON THE CAMPAIGN MANAGER & LEADER FOR THE PARTY
					if(labour_count >= 3){
						if(parties[0]->getCampaignManager()->getReliability() >= 5 && parties[0]->getLeader()->getControversyLevel() < 8){
							cout << "Due to the Labour Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, candidate " << electorates[y]->getLocalCandidates()[0]->getName() << " (" << electorates[y]->getLocalCandidates()[0]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[0]->setPopularity(2);
							electorates[y]->getLocalCandidates()[0]->setControversyLevel(-2);
						}
						else{
							cout << "Due to the Labour Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, candidate " << electorates[y]->getLocalCandidates()[0]->getName() << " (" << electorates[y]->getLocalCandidates()[0]->getParty()->getName() << ") has had an increase in popularity by 10% and have become 10% less controversial" << endl;
							electorates[y]->getLocalCandidates()[0]->setPopularity(1);
							electorates[y]->getLocalCandidates()[0]->setControversyLevel(-1);
						}
						labour_count = 0;
					}
					if(greens_count >= 3){
						if(parties[1]->getCampaignManager()->getReliability() >= 5 && parties[1]->getLeader()->getControversyLevel() < 8){
							cout << "Due to the Greens Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, the candidate " << electorates[y]->getLocalCandidates()[1]->getName() << " (" << electorates[y]->getLocalCandidates()[1]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[1]->setPopularity(2);
							electorates[y]->getLocalCandidates()[1]->setControversyLevel(-2);
						}
						else{
							cout << "Due to the Greens Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, candidate " << electorates[y]->getLocalCandidates()[1]->getName() << " (" << electorates[y]->getLocalCandidates()[1]->getParty()->getName() << ") has had an increase in popularity by 10% and have become 10% less controversial" << endl;
							electorates[y]->getLocalCandidates()[1]->setPopularity(1);
							electorates[y]->getLocalCandidates()[1]->setControversyLevel(-1);
						}
						greens_count = 0;
					}
					if(liberal_count >= 3){
						if(parties[2]->getCampaignManager()->getReliability() >= 5 && parties[2]->getLeader()->getControversyLevel() < 8){
							cout << "Due to the Liberal Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, candidate " << electorates[y]->getLocalCandidates()[2]->getName() << " (" << electorates[y]->getLocalCandidates()[2]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[2]->setPopularity(2);
							electorates[y]->getLocalCandidates()[2]->setControversyLevel(-2);
						}
						else{
							cout << "Due to the Liberal Party agreeing with " << electorates[y]->getName() <<  " on what should be done to fix the internet, candidate " << electorates[y]->getLocalCandidates()[2]->getName() << " (" << electorates[y]->getLocalCandidates()[2]->getParty()->getName() << ") has had an increase in popularity by 10% and have become 10% less controversial" << endl;
							electorates[y]->getLocalCandidates()[2]->setPopularity(1);
							electorates[y]->getLocalCandidates()[2]->setControversyLevel(-1);
						}
						liberal_count = 0;
					}
				}
			}
		}
	}
}
}
void National_Event::birdApocalypse(){
	int party_significance_and_approach_increment = 0;
	int electorate_significance_and_approach_increment = 0;
	vector<int> increments;
	cout << "An invasion of birds has taken over the nation! Leaders scramble to act on the threat posed!" << endl;
	int labour_count = 0;
	int greens_count = 0;
	int liberal_count = 0;
	//Generate a random increment number
	//For each party, add the increment to their approach and their significance value
	//For each electorate, add an increment to their significance and approach on high speed internet
	for(int x = 0;x < parties.size();x++){
		party_significance_and_approach_increment = events(randEng);
		increments.push_back(party_significance_and_approach_increment);
		parties[x]->getStances()[1].setSignificance(party_significance_and_approach_increment);
		parties[x]->getStances()[1].setApproach(party_significance_and_approach_increment);
	}
	//FOR EACH ELECTORATE
	for(int y = 0;y < electorates.size();y++){
		//FOR EACH OF THE SiX CLUSTERS
		for(int z = 0;z < electorates[y]->getStances().size();z++){
			//FOR THE CLUSTERS ON INTERNET ISSUE
			if(electorates[y]->getStances()[z].getIssue().getDescription() == "Large Bird Plauge"){
				electorate_significance_and_approach_increment = events(randEng);
				electorates[y]->getStances()[z].setSignificance(electorate_significance_and_approach_increment);
				electorates[y]->getStances()[z].setApproach(electorate_significance_and_approach_increment);
				//CHECK EACH OF THE PARTIES CHANGE IN VIEWS, IF THE MOST CLUSTERS IN THE ELECTORATE SHARE THE SAME VIEW, THEN THE LOCAL MEMBER OF THAT PARTY WILL BECOME MORE POPULAR AND LESS CONTROVERSIAL
				for(int q = 0;q < increments.size();q++){
					if(electorate_significance_and_approach_increment == increments[q]){
						if(q == 0)
							labour_count++;
						if(q == 1)
							greens_count++;
						if(q == 2)
							liberal_count++;
					}
					//IF 3 OR MORE CLUSTERS IN AN ELECTORATE SHARE THE SAME VIEW AS THE PARTY, THE LOCAL CANDIDATE FOR THAT ELECTORATE WILL BECOME MORE FAVOURED DEPENDANT ALSO ON THE CAMPAIGN MANAGER FOR THE PARTY
					if(labour_count >= 4){
						if(parties[0]->getCampaignManager()->getReliability() >= 8 && parties[0]->getLeader()->getControversyLevel() < 7){
							cout << "Due to the Labour Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[0]->getName() << " (" << electorates[y]->getLocalCandidates()[0]->getParty()->getName() << ") has had an increase in popularity by 40% and have become 40% less controversial" << endl;
							electorates[y]->getLocalCandidates()[0]->setPopularity(4);
							electorates[y]->getLocalCandidates()[0]->setControversyLevel(-4);
						}
						else{
							cout << "Due to the Labour Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[0]->getName() << " (" << electorates[y]->getLocalCandidates()[0]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[0]->setPopularity(2);
							electorates[y]->getLocalCandidates()[0]->setControversyLevel(-2);
						}
						labour_count = 0;
					}
					if(greens_count >= 4){
						if(parties[1]->getCampaignManager()->getReliability() >= 8 && parties[1]->getLeader()->getControversyLevel() < 7){
							cout << "Due to the Greens Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[1]->getName() << " (" << electorates[y]->getLocalCandidates()[1]->getParty()->getName() << ") has had an increase in popularity by 40% and have become 40% less controversial" << endl;
							electorates[y]->getLocalCandidates()[1]->setPopularity(4);
							electorates[y]->getLocalCandidates()[1]->setControversyLevel(-4);
						}
						else{
							cout << "Due to the Labour Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[1]->getName() << " (" << electorates[y]->getLocalCandidates()[1]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[1]->setPopularity(2);
							electorates[y]->getLocalCandidates()[1]->setControversyLevel(-2);
						}
						greens_count = 0;
					}
					if(liberal_count >= 4){
						if(parties[2]->getCampaignManager()->getReliability() >= 8 && parties[2]->getLeader()->getControversyLevel() < 7){
							cout << "Due to the Liberal Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[2]->getName() << " (" << electorates[y]->getLocalCandidates()[2]->getParty()->getName() << ") has had an increase in popularity by 40% and have become 40% less controversial" << endl;
							electorates[y]->getLocalCandidates()[2]->setPopularity(4);
							electorates[y]->getLocalCandidates()[2]->setControversyLevel(-4);
						}
						else{
							cout << "Due to the Liberal Party agreeing with " << electorates[y]->getName() <<  " on what should be to stop the invasion, candidate " << electorates[y]->getLocalCandidates()[2]->getName() << " (" << electorates[y]->getLocalCandidates()[2]->getParty()->getName() << ") has had an increase in popularity by 20% and have become 20% less controversial" << endl;
							electorates[y]->getLocalCandidates()[2]->setPopularity(2);
							electorates[y]->getLocalCandidates()[2]->setControversyLevel(-2);
						}
						liberal_count = 0;
					}
				}
			}
		}
	}
}


