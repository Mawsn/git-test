#include <iostream>
#include <fstream>
#include "Header.h"
using std::ifstream;
using std::stoi;
using std::invalid_argument;
using std::cerr;
using std::endl;
using std::cout;
using std::string;
using std::vector;

int main(int argc, char *argv[]){
	int days;
	int number_of_electorates;
	const int number_of_parties = 3;
	//Create read streams to access data files
	ifstream issues_read_stream;
	ifstream leaders_read_stream;
	ifstream candidates_read_stream;
	ifstream electoral_managers_read_stream;
	ifstream campaign_manager_read_stream;
	ifstream finnancial_manager_read_stream;
	ifstream electorate_read_stream;
	ifstream party_range_read_stream;
	
	//Check that there are 3 arguments like (./APE 3 7), if not throw and error and quit the program
	checkArgumentsExist(argc);
	try {
		number_of_electorates = stoi(argv[2]);
		days = stoi(argv[1]);
	}
	catch(invalid_argument& e){
		cerr << "Please enter two integers to run the program!" << endl;
		exit(EXIT_FAILURE);
	}
	//Check that days > 30 and electorates <= 10
	checkInputConstraints(days,number_of_electorates);

	//Open all data files required and check if each file has successfully opened.
	issues_read_stream.open("Issues.txt");
	leaders_read_stream.open("Leaders.txt");
	candidates_read_stream.open("Candidates.txt");
	electoral_managers_read_stream.open("Electoral_Managers.txt");
	campaign_manager_read_stream.open("National_Managers.txt");
	finnancial_manager_read_stream.open("Finnancial_Managers.txt");
	electorate_read_stream.open("Electorates.txt");
	party_range_read_stream.open("Party_Ranges.txt");
	checkIfFilesOpened(issues_read_stream,leaders_read_stream,candidates_read_stream,electoral_managers_read_stream,campaign_manager_read_stream,finnancial_manager_read_stream,electorate_read_stream,party_range_read_stream);

	//Create lists to store data related to electorates, party members, issues and parties
	vector<Political_Party*> parties;
	vector<Electorate*> electorates;
	vector<Issue*> issues;
	vector<string> party_names;
	vector<string> electorate_names;
	vector<Leader*> leaders_list;
	vector<Electoral_Candidate*> candidates_list;
	vector<Electorate_Manager*> electoral_manager_list;
	vector<Campaign_Manager*> campaign_manager_list;
	vector<Finnancial_Manager*> finnancial_manager_list;

	vector<int> labour_range;
	vector<int> greens_range;
	vector<int> liberal_range;
	//Create names for each of the three parties and push them into a vector for later usage
	party_names.push_back("Labour");
	party_names.push_back("Greens");
	party_names.push_back("Liberal");

	string data;
	int count = 0;
	//Push back issue, leader, candidate, electorate manager, campaign manager, finnancial manager and electorate objects into coresponding vectors
	try{
	while(getline(issues_read_stream,data))
		issues.push_back(createNationalIssue(data));
	while(getline(leaders_read_stream,data))
		leaders_list.push_back(createLeader(data,issues));
	while(getline(candidates_read_stream,data))
		candidates_list.push_back(createCandidate(data,issues));
	while(getline(electoral_managers_read_stream,data))
		electoral_manager_list.push_back(createElectorateManager(data));
	while(getline(campaign_manager_read_stream,data))
		campaign_manager_list.push_back(createCampaignManager(data));
	while(getline(finnancial_manager_read_stream,data))
		finnancial_manager_list.push_back(createFinnancialManager(data));
	while(getline(electorate_read_stream,data))
		electorate_names.push_back(data);
	while(getline(party_range_read_stream,data)){
		if(count == 0)
			labour_range = createPartyRanges(data);
		if(count == 1)
			greens_range = createPartyRanges(data);
		if(count == 2)
			liberal_range = createPartyRanges(data);
		count++;
	}
	}

	catch(invalid_argument& e){
		clearMemory(electorates,parties,leaders_list,candidates_list,issues,electoral_manager_list,campaign_manager_list,finnancial_manager_list);
		return -1;
	}


	int number = 0;
	//Create party objects and push back into the parties vector for storage
	for(int x = 0;x < number_of_parties;x++){
		vector<int> range;
		if(x == 0)
			range = labour_range;
		if(x == 1)
			range = greens_range;
		if(x == 2)
			range = liberal_range;

		string name = party_names[x];
		
		Leader* leader = leaders_list[x];
		vector<Electoral_Candidate*> party_candidates;
		vector<Electorate_Manager*> party_electoral_managers;
		Campaign_Manager* party_campaign_manager = campaign_manager_list[x];
		Finnancial_Manager* party_finnancial_manager = finnancial_manager_list[x];
		//Based on the number of electorates, a party will need 1 + N number of candidates and electorate managers who will represent the party in each electorate
		for(int y = 0;y < number_of_electorates;y++){
			party_candidates.push_back(candidates_list[number++]);
			party_electoral_managers.push_back(electoral_manager_list[number]);
		}
		Political_Party* party = new Political_Party(name,leader,party_candidates,party_electoral_managers,party_campaign_manager,party_finnancial_manager,issues,range);
		parties.push_back(party);
	}
	number = 0;
	//Create electorates and add to a vector
	for(int x = 0;x < number_of_electorates;x++){
		string name = electorate_names[x];
		vector<Electoral_Candidate*> local_candidates;
		vector<Electorate_Manager*> local_managers;
		//For each party, assign their candidates and electoral managers to an electorate
		for(int y = 0;y < parties.size();y++){
			local_candidates.push_back(parties[y]->getCandidates()[number]);
			local_managers.push_back(parties[y]->getElectorateManagers()[number]);
		}
		number++;
		Electorate* e = new Electorate(name,local_candidates,local_managers,issues);
		electorates.push_back(e);
	}
	//Output information on the state of the nation and the break down of all parties
	cout << outputPartyBreakdown(parties,electorates,issues);
	//Create a Votes object to store votes each party accumulates during the campaigning days. These votes act as a way to check how much support each party has from each electorate essentially
	Votes total_party_votes;
	//Vectors to store data about the parties finnances 
	vector<double> initial_funds;
	vector<double> income;
	vector<double> total_income;
	
	cout << "\n[CAMPAIGNING DAYS BEGIN]\n" << endl;
	//For each day in the campaign, this loop will be run
	for(int a = 0;a < days;a++){
		cout << "Day " << a + 1 << ":\n------ " << endl;
		//For each party, capture the initial available funds. Next, distribute the funds amongst the electorate managers. Next make the electorate managers spend their funding on their assigned electorate
		for(int b = 0;b < parties.size();b++){
			initial_funds.push_back(parties[b]->getAvailableFunds());
			parties[b]->distributeFunding();
			for(int c = 0;c < parties[b]->getElectorateManagers().size();c++){
				parties[b]->getElectorateManagers()[c]->spendFunding();
			}
			//For each electorate, see how many votes/support they have given to each party. This will be used to determine how much money the party will bring in!
			for(int x = 0;x < electorates.size();x++){
			total_party_votes.voteLabour(electorates[x]->getPartyVotes().getLabourVotes());
			total_party_votes.voteGreens(electorates[x]->getPartyVotes().getGreensVotes());
			total_party_votes.voteLiberal(electorates[x]->getPartyVotes().getLiberalVotes());
			}
			//Make the finnancial manager bring in some money based on his effectivness and the support/current number of votes for the party. Then store this profit in the income vector
			parties[b]->getFinnancialManager()->earnMoney(total_party_votes);
			income.push_back(parties[b]->getAvailableFunds());
		}
		//For each electorate, get their perspectives on each party. This function is where party support/votes are incremented depending on whether the party and electorate align closely or vice versa
		for(int d = 0;d < electorates.size();d++){
			electorates[d]->getPerspectivesOnParties(parties);
		}
		//For each electorate, get their perspectives on each candidate. This will increase the votes/support for each candidate and by the end of the campaign, their total votes will determine whether they win or not
		for(int e = 0;e < electorates.size();e++){
			electorates[e]->getPerspectivesOnCandidates(); 
		}
		//Create a national event object, this object may begin a national event, there is a 30% chance
		National_Event n_event(electorates,parties); 
		
		
		for(int f = 0;f < electorates.size();f++){
			//For each electorate, a local_event object is created, the constructor of this class has a 20% chance of spawning a local event
			Local_Event l_event(electorates[f]);
		}
		//Output a daily report on party finances , escape sequence adds some novelty detail :D !
		cout << "------\n\u2691 DAILY REPORT \u2691\n------" << endl;
		for(int g = 0;g < parties.size();g++){
			cout << parties[g]->getName() << " Finnances: Initial = $" << initial_funds[g] << ", Income = $" << income[g] << ", Expenditure = $" << initial_funds[g] << ", Remaining = $";
			income.clear();
			initial_funds.clear();
			cout << parties[g]->getAvailableFunds() << endl;
		}
		cout << endl;
		//The day before the election takes place, report on the state of the nation. Then commence the election!
		if(a == days - 1){
			dayBeforeElectionReport(electorates,issues,parties);
		}
	}
		//Commence the election!
		electionResultsReport(electorates,parties);
		//CLEANS UP ALL ALLOCATED MEMORY TO PREVENT MEMORY LEAKAGE!
		clearMemory(electorates,parties,leaders_list,candidates_list,issues,electoral_manager_list,campaign_manager_list,finnancial_manager_list);
		return 0;

}