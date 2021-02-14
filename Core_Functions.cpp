#include "Header.h"
using std::ifstream;
using std::stoi;
using std::to_string;
using std::cerr;
using std::endl;
using std::cout;
using std::string;
using std::vector;

void checkArgumentsExist(int argc){
	//If there are less than or greater than 3 arguments, quit the program and throw a message to cerr
	if(argc < 3 || argc > 3){
		cerr << "Too few arguments to run the program!" << endl;
		exit(EXIT_FAILURE);
	}
}
void checkInputConstraints(int days,int number_of_electorates){
	//Check that days > 30 and electorates <= 10
	if(days > 30 || days < 1){
		cerr << "There can only be between 1 and 30 days in the election" << endl;
		exit(EXIT_FAILURE);
	}
	if(number_of_electorates > 10 || number_of_electorates < 1){
		cerr << "There can only be between 1 and 10 electorates" << endl;
		exit(EXIT_FAILURE);
	}
}
void checkIfFilesOpened(ifstream &a,ifstream &b,ifstream &c,ifstream &d,ifstream &e,ifstream &f,ifstream &g,ifstream &h){
	//Perform check to make sure all data files are accessible, if not quit immediately!
	if(!a) {
        cerr << "Issues.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!b) {
        cerr << "Leaders.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!c) {
        cerr << "Candidates.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!d) {
        cerr << "Electoral_Managers.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!e) {
        cerr << "National_Managers.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!f) {
        cerr << "Finnancial_Managers.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!g) {
        cerr << "Electorates.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if(!h) {
        cerr << "Party_Ranges.txt failed to open" << endl;
        exit(EXIT_FAILURE);
    }
}
vector<int> createPartyRanges(string data){
	vector<int> range_values;
	string temp = data;
	
	char text;
	int colon_count = 0;
	int braces = 0;
	//Perform checks to ensure file data is valid! If not cerr an error message and quit
	for(int x = 0;x < data.length();x++){
		text = data[x];
		if(text == ':'){
			colon_count++;
		}
		if(text == '[' || text == ']'){
			braces++;
		}
	}
	if(colon_count == 15 && braces == 10){
		for(int x = 0;x < 5;x++){
		try{
			data = temp;
			data = data.substr(data.find("[") + 1,data.find("]") - 1);
			int low_sig = stoi(data.substr(0,data.find(":")));
			data = data.substr(data.find(":") + 1,data.length());
			int upp_sig = stoi(data.substr(0,data.find(":")));
			data = data.substr(data.find(":") + 1,data.length());
			int low_app = stoi(data.substr(0,data.find(":")));
			data = data.substr(data.find(":") + 1,data.length());
			int upp_app = stoi(data.substr(0,data.find(":")));
			temp = temp.substr(temp.find(",") + 1,temp.length());

			range_values.push_back(low_sig);
			range_values.push_back(upp_sig);
			range_values.push_back(low_app);
			range_values.push_back(upp_app);
		}
		catch(const std::invalid_argument& e){
			cerr << "Error in file: The format of a party range in the file is incorrect!" << endl;
			throw std::invalid_argument("Invalid data file!");
			
		}
		}
		return range_values;
	}
	else{
		cerr << "Error in file: The format of a party range in the file is incorrect!" << endl;
		throw std::invalid_argument("Invalid data file!");
	}

}
//Create an issue pointer ready to be added to the vector in main
Issue* createNationalIssue(string data){return new Issue(data);}
//Create an leader pointer based on file data ready to be added to the vector in main
Leader* createLeader(string data,vector<Issue*> issues){
	string name;
	int popularity;
	int controversy;

	char text;
	int colon_count = 0;
	int full_stops = 0;
	//Perform checks to ensure file data is valid! If not cerr an error message and quit
	for(int x = 0;x < data.length();x++){
		text = data[x];
		if(text == ':'){
			colon_count++;
		}
		if(text == '.'){
			full_stops++;
		}
	}

	if(colon_count == 2 && full_stops == 1){
		name = data.substr(0, data.find(":"));
		data = data.substr(data.find(":") + 1,data.length());
		try{
		popularity = stoi(data.substr(0, data.find(":")));
		data = data.substr(data.find(":") + 1,data.length());

		controversy = stoi(data.substr(0,data.find(".")));
		}
		catch(const std::invalid_argument& e){
			cerr << "Error in file: The format of a leader in the file is incorrect!" << endl;
			throw std::invalid_argument("Invalid data file!");
		}
		return new Leader(name,issues,popularity,controversy);
	}
	else{
		cerr << "Error in file: The format of a leader in the file is incorrect!" << endl;
		throw std::invalid_argument("Invalid data file!");
	}
}	
Electoral_Candidate* createCandidate(string data,vector<Issue*> issues){
	string name;
	int popularity;
	int controversy;
	char text;
	int colon_count = 0;
	int full_stops = 0;
	for(int x = 0;x < data.length();x++){
		text = data[x];
		if(text == ':'){
			colon_count++;
		}
		if(text == '.'){
			full_stops++;
		}
	}
	if(colon_count == 2 && full_stops == 1){
		name = data.substr(0, data.find(":"));
		data = data.substr(data.find(":") + 1,data.length());
		try{
		popularity = stoi(data.substr(0, data.find(":")));
		data = data.substr(data.find(":") + 1,data.length());

		controversy = stoi(data.substr(0,data.find(".")));
		}
		catch(const std::invalid_argument& e){
			cerr << "Error in file: The format of a candidate in the file is incorrect!" << endl;
			throw std::invalid_argument("Invalid data file!");
		}
		return new Electoral_Candidate(name,issues,popularity,controversy);
	}
	else{
		cerr << "Error in file: The format of a candidate in the file is incorrect!" << endl;
		throw std::invalid_argument("Invalid data file!");
	}
}
Electorate_Manager* createElectorateManager(string data){
	return new Electorate_Manager(data);
}
Campaign_Manager* createCampaignManager(string data){
	return new Campaign_Manager(data);
}
Finnancial_Manager* createFinnancialManager(string data){
	int effectiveness;
	string name;
	char text;
	int colon_count = 0;
	int full_stops = 0;
	for(int x = 0;x < data.length();x++){
		text = data[x];
		if(text == ':'){
			colon_count++;
		}
		if(text == '.'){
			full_stops++;
		}
	}
	if(colon_count == 1 && full_stops == 1){
		name = data.substr(0, data.find(":"));
		data = data.substr(data.find(":") + 1,data.length());
		try{
		effectiveness = stoi(data.substr(0, data.find(".")));
		}
		catch(const std::invalid_argument &e){
			cerr << "Error in file: The format of a financial manager in the file is incorrect!" << endl;
			throw std::invalid_argument("Invalid data file!");
		}
		return new Finnancial_Manager(name,effectiveness);
	}
	else{
		cerr << "Error in file: The format of a financial manager in the file is incorrect!" << endl;
		throw std::invalid_argument("Invalid data file!");
	}
}
//Return a breakdown of the parties and electorates as string! This was originally done so that it could be placed in a data file if needed
string outputPartyBreakdown(vector<Political_Party*> &parties,vector<Electorate*> &electorates,vector<Issue*> &issues){
	string text = "PARTIES\n-------\n";
	for(int x = 0;x < parties.size();x++){
		text += "The " + parties[x]->getName() + " Party" + "\n-----------------\n" + "Leader: " + parties[x]->getLeader()->getName() + " is " + to_string(parties[x]->getLeader()->getPopularity()) + "/10 popular and " + to_string(parties[x]->getLeader()->getControversyLevel()) + "/10 controversial.\nCandidates: ";
		for(int y = 0;y < parties[x]->getCandidates().size();y++)
			if(y == (parties[x]->getCandidates().size() - 1))
				text += parties[x]->getCandidates()[y]->getName() + " is " + to_string(parties[x]->getCandidates()[y]->getPopularity())  + "/10 popular and " + to_string(parties[x]->getCandidates()[y]->getControversyLevel()) + "/10 controversial.\nElectorate Managers: ";
			else
				text += parties[x]->getCandidates()[y]->getName() + " is " + to_string(parties[x]->getCandidates()[y]->getPopularity())  + "/10 popular and " + to_string(parties[x]->getCandidates()[y]->getControversyLevel()) + "/10 controversial, ";
		
		for(int z = 0;z < parties[x]->getElectorateManagers().size();z++)
			if(z == (parties[x]->getElectorateManagers().size() - 1))
				text += parties[x]->getElectorateManagers()[z]->getName() + ".\nFinnancial Manager: ";
			else
				text += parties[x]->getElectorateManagers()[z]->getName() + ", ";
		text += parties[x]->getFinnancialManager()->getName() + ".\nCampaign Manager: ";
		text += parties[x]->getCampaignManager()->getName() + ".\nFunding Available: $";
		text += to_string((int) round(parties[x]->getAvailableFunds())) + "\n";
		for(int a = 0;a < parties[x]->getStances().size();a++){
			text += "Significance of " + parties[x]->getStances()[a].getIssue().getDescription() + ": " + to_string(parties[x]->getStances()[a].getSignificance() * 10) +"%\n";
			text += "Approach seriousness on " + parties[x]->getStances()[a].getIssue().getDescription() + ": " + to_string(parties[x]->getStances()[a].getApproach() * 10) +"%\n";
		}
		text += "\n";
	}
	text += "\nELECTORATES\n-----------\n";

	for(int x = 0;x < electorates.size();x++){		
		vector<int> overall_issue_significance = {0,0,0,0,0};
		vector<int> overall_issue_approach = {0,0,0,0,0};

		text += electorates[x]->getName() + "\n-----------\n";
		text += "Population: " +  to_string(electorates[x]->getPopulation()) + "\n";
		for(int y = 0;y < electorates[x]->getStances().size();y++){
			if(electorates[x]->getStances()[y].getIssue().getDescription() == "Crime Rates"){
				overall_issue_significance[0] += electorates[x]->getStances()[y].getSignificance();
				overall_issue_approach[0] += electorates[x]->getStances()[y].getApproach();
			}
			if(electorates[x]->getStances()[y].getIssue().getDescription() == "High Speed Internet Access"){
				overall_issue_significance[1] += electorates[x]->getStances()[y].getSignificance();
				overall_issue_approach[1] += electorates[x]->getStances()[y].getApproach();
			}
			if(electorates[x]->getStances()[y].getIssue().getDescription() == "Road Infastructure"){
				overall_issue_significance[2] += electorates[x]->getStances()[y].getSignificance();
				overall_issue_approach[2] += electorates[x]->getStances()[y].getApproach();
				}
			if(electorates[x]->getStances()[y].getIssue().getDescription() == "Food Shortages"){
				overall_issue_significance[3] += electorates[x]->getStances()[y].getSignificance();
				overall_issue_approach[3] += electorates[x]->getStances()[y].getApproach();
			}
			if(electorates[x]->getStances()[y].getIssue().getDescription() == "Large Bird Plauge"){
				overall_issue_significance[4] += electorates[x]->getStances()[y].getSignificance();
				overall_issue_approach[4] += electorates[x]->getStances()[y].getApproach();
			}
		}
		text += "Based on six cluster samples taken from " + electorates[x]->getName() + ": \n";
				for(int q = 0;q < overall_issue_significance.size();q++){
					text += issues[q]->getDescription() + ":  Significance = " + to_string((overall_issue_significance[q] * 100)/60) + "%,  Approach = " + to_string((overall_issue_approach[q] * 100)/60) + "%\n";
				}
				text += "\n";
	}
	return text;
}
//The day before the election takes place/last campaigning day, output information on the parties and electorates.
void dayBeforeElectionReport(vector<Electorate*> &electorates,vector<Issue*> &issues,vector<Political_Party*> &parties){
	cout << "\u26A0 IT IS THE DAY BEFORE THE ELECTION! \u26A0\n------------------\nELECTORATE INFORMATION\n------------------" << endl;
			for(int x = 0;x < electorates.size();x++){
				
				vector<int> overall_issue_significance = {0,0,0,0,0};
				vector<int> overall_issue_approach = {0,0,0,0,0};

				cout << electorates[x]->getName() << " has a population of " << electorates[x]->getPopulation() << endl;
				//For each electorate, get the significance and approach of each issue within each cluster
				//Since there are 6 clusters in an electorate by design, the total significance and approach an issue holds will be something like x/60
				for(int y = 0;y < electorates[x]->getStances().size();y++){
					if(electorates[x]->getStances()[y].getIssue().getDescription() == "Crime Rates"){
						overall_issue_significance[0] += electorates[x]->getStances()[y].getSignificance();
						overall_issue_approach[0] += electorates[x]->getStances()[y].getApproach();
					}
					if(electorates[x]->getStances()[y].getIssue().getDescription() == "High Speed Internet Access"){
						overall_issue_significance[1] += electorates[x]->getStances()[y].getSignificance();
						overall_issue_approach[1] += electorates[x]->getStances()[y].getApproach();
					}
					if(electorates[x]->getStances()[y].getIssue().getDescription() == "Road Infastructure"){
						overall_issue_significance[2] += electorates[x]->getStances()[y].getSignificance();
						overall_issue_approach[2] += electorates[x]->getStances()[y].getApproach();
					}
					if(electorates[x]->getStances()[y].getIssue().getDescription() == "Food Shortages"){
						overall_issue_significance[3] += electorates[x]->getStances()[y].getSignificance();
						overall_issue_approach[3] += electorates[x]->getStances()[y].getApproach();
					}
					if(electorates[x]->getStances()[y].getIssue().getDescription() == "Large Bird Plauge"){
						overall_issue_significance[4] += electorates[x]->getStances()[y].getSignificance();
						overall_issue_approach[4] += electorates[x]->getStances()[y].getApproach();
					}
				}
				//Based on the total significance and approach across each cluster in an electorate, multiply x/60 by 100
				cout << "Based on six cluster samples taken from " << electorates[x]->getName() << ": " << endl;
				for(int q = 0;q < overall_issue_significance.size();q++){
					cout << issues[q]->getDescription() << ":  Significance = " << ((overall_issue_significance[q] * 100)/60) << "%,  Approach = " << ((overall_issue_approach[q] * 100)/60) << "% " << endl;
				}
				cout << endl;
			}
			//Output information on each party, such as their stances and the people who belong to their party
			cout << "\nPARTY INFORMATION\n------------------" << endl;
			for(int z = 0;z < parties.size();z++){
				cout << "[The " << parties[z]->getName() << " Party]: " << endl;
				cout << "Leader: ";
				cout << parties[z]->getLeader()->getName() << " is " << parties[z]->getLeader()->getPopularity() << "/10 popular and " << parties[z]->getLeader()->getControversyLevel() << "/10 controversial" << endl;
				cout << "Candidates: " << endl;
				for(int x = 0;x < parties[z]->getCandidates().size();x++){
					cout << parties[z]->getCandidates()[x]->getName() << " is " << parties[z]->getCandidates()[x]->getPopularity() << "/10 popular and " << parties[z]->getCandidates()[x]->getControversyLevel() << "/10 controversial" << endl;
				}
				for(int w = 0;w < parties[z]->getStances().size();w++){
					cout << parties[z]->getStances()[w].getIssue().getDescription() << ": Significance = "  << (parties[z]->getStances()[w].getSignificance() * 10) << "%, Approach = " << (parties[z]->getStances()[w].getApproach() * 10) << "%" << endl;
				}
				cout << endl;
			}		
}

void electionResultsReport(vector<Electorate*> &electorates, vector<Political_Party*> &parties){
	//Create a vector and add all electorate votes on candidates into it
	Votes candidate_wins;
		cout << "[\u2713-----ELECTION DAY-----\u2713]" << endl;
			for(int x =0;x < electorates.size();x++){
					if(electorates[x]->getCandidateVotes().determineWinner() == "Labour"){
						cout << electorates[x]->getLocalCandidates()[0]->getName() << " (" << electorates[x]->getLocalCandidates()[0]->getParty()->getName() << ") has won in the " << electorates[x]->getName() << " electorate!" << endl;
						candidate_wins.voteLabour(1);
					}
					if(electorates[x]->getCandidateVotes().determineWinner() == "Greens"){
						cout << electorates[x]->getLocalCandidates()[1]->getName() << " (" << electorates[x]->getLocalCandidates()[1]->getParty()->getName() << ") has won in the " << electorates[x]->getName() << " electorate!" << endl;
						candidate_wins.voteGreens(1);
					}
					if(electorates[x]->getCandidateVotes().determineWinner() == "Liberal"){
						cout << electorates[x]->getLocalCandidates()[2]->getName() << " (" << electorates[x]->getLocalCandidates()[2]->getParty()->getName() << ") has won in the " << electorates[x]->getName() << " electorate!" << endl;
						candidate_wins.voteLiberal(1);
					}
			}
			//If the majority (50%_) of electorates have been won by a party, declare that party as winner, else if there is a tie, declare a hung parliment!
			if(candidate_wins.determineWinner() != "None"){
				if(candidate_wins.determineWinner() == "Labour")
					cout << "\u2605 " << parties[0]->getLeader()->getName() << " (" << candidate_wins.determineWinner() << ") has won the election and is the new leader of the country! \u2605" << endl;
				if(candidate_wins.determineWinner() == "Greens")
					cout << "\u2605 " << parties[1]->getLeader()->getName() << " (" << candidate_wins.determineWinner() << ") has won the election and is the new leader of the country! \u2605" << endl;
				if(candidate_wins.determineWinner() == "Liberal")
					cout << "\u2605 " << parties[2]->getLeader()->getName() << " (" << candidate_wins.determineWinner() << ") has won the election and is the new leader of the country! \u2605" << endl;
			}
			else{
				if(candidate_wins.getLabourVotes() == candidate_wins.getLiberalVotes())
					cout << "\u2620 The election has resulted in a hung parliment between the Labour Party and the Liberal Party \u2620" << endl;

				if(candidate_wins.getLabourVotes() == candidate_wins.getGreensVotes())
					cout << "\u2620 The election has resulted in a hung parliment between the Labour Party and the Greens Party \u2620" << endl;

				if(candidate_wins.getLiberalVotes() == candidate_wins.getGreensVotes())
					cout << "\u2620 The election has resulted in a hung parliment between the Greens Party and the Liberal Party \u2620" << endl;
			}
}
void clearMemory(vector<Electorate*> &electorates,vector<Political_Party*> &parties,vector<Leader*> &leaders_list,vector<Electoral_Candidate*> &candidates_list,vector<Issue*> &issues,vector<Electorate_Manager*> &electoral_manager_list,vector<Campaign_Manager*> &campaign_manager_list,vector<Finnancial_Manager*> &finnancial_manager_list){
	for(int x = 0;x < electorates.size();x++)
		delete electorates[x];
	for(int x = 0;x < parties.size();x++)
		delete parties[x];
	for(int x = 0;x < leaders_list.size();x++)
		delete leaders_list[x];
	for(int x = 0;x < candidates_list.size();x++)
		delete candidates_list[x];
	for(int x = 0;x < issues.size();x++)
		delete issues[x];
	for(int x = 0;x < electoral_manager_list.size();x++)
		delete electoral_manager_list[x];
	for(int x = 0;x < campaign_manager_list.size();x++)
		delete campaign_manager_list[x];
	for(int x = 0;x < finnancial_manager_list.size();x++)
		delete finnancial_manager_list[x];
}