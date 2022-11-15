#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, char del) {
  vector<string> result;
  string temp = "";
	
  for(int i = 0; i < (int)str.size(); i++) {
    if (str[i] != del) {
	  	temp += str[i];
    } else {
      result.push_back(temp);
      temp = "";
    }
  }
	
  result.push_back(temp);
  return result;
}

class cvsdata {
  string date;
  string country;
  int cases;
  int deaths;
	public:
  	cvsdata(string date, string country, int cases, int deaths) {
    	this->date = date;
    	this->country = country;
    	this->cases = cases;
    	this->deaths = deaths;
  	}

		// getters()
		string getDate() {return date;}
		string getCountry() {return country;}
		int getCases() {return cases;}
		int getDeaths() {return deaths;}
};

class DataEntry {
    private:
      string date;
      string country;
      int c_cases;
      int c_deaths;
    public:
			// constructor
			DataEntry() {}

			void load(cvsdata data) {
			 	date = data.getDate();
			 	country = data.getCountry();
			 	c_cases = data.getCases();
			 	c_deaths = data.getDeaths();
			}

      // getters()
			string getDate() {return date;}
			string getCountry() {return country;}
			int getCases() {return c_cases;}
			int getDeaths() {return c_deaths;}

			// setters()
			void setDate(string date) {this->date = date;}
			void setCountry(string country) {this->country = country;}
			void setCases(int cases) {c_cases = cases;}
			void setDeaths(int deaths) {c_deaths = deaths;}
};

class CovidDB {
	private:
    int tableSize = 17;
    vector<vector<DataEntry>> dataBase;

	public:
		CovidDB () {
      dataBase = vector<vector<DataEntry>>(tableSize);
		}

		int hash(string country) { 
			int sum = 0; 
  		for (int i = 0; i < country.length(); i++) {
				// cout << (i + 1) << " x " << int(country[i]) << endl;
				sum += (i + 1) * int(country[i]);
    	} 
  		return sum % tableSize;
		}

	// helper function to check the index contain the country given
		int ifContain(string country, int index) {
			for (int i = 0; i < dataBase[index].size(); i++) {
				if (dataBase[index][i].getCountry() == country) {
					return i;
				}
			}
			return -1;
		}

	// helper functions to check if date is later than existing one

		// split the date string so it can be compared
		vector<int> parseDate(string date) {
			vector<int> breakdown;
			string temp = "";

			for(int i = 0; i < (int)date.size(); i++) {
				if (date[i] != '/'){
	  			temp += date[i];
    		} else {
      		breakdown.push_back(stoi(temp));
      		temp = "";
				}
			}
			
			breakdown.push_back(stoi(temp));
			return breakdown;
		}

		// if date1 is later than date2 return true, false otherwise
		bool compareDate(string date1, string date2) {
			vector<int> first = parseDate(date1);
			vector<int> second = parseDate(date2);
			
			if (first[2] < second[2]) {			// compare year
				return false;
			} else if (first[2] > second[2]) {
				return true;
			} else {
				if (first[0] < second[0]) {		// compare month
					return false;
				} else if (first[0] > second[0]) {
					return true;
				} else {
					if (first[1] <= second[1]) {		// compare day
						return false;
					} else {
						return true;
					}
				}
			}
		}

		bool add(DataEntry entry) {
			int index = hash(entry.getCountry());
			int inner = ifContain(entry.getCountry(), index);
			if (inner == -1) {
				dataBase[index].push_back(entry);
			} else {
				string date = dataBase[index][inner].getDate();
				if (compareDate(entry.getDate(), date)) {
					dataBase[index][inner].setDate(entry.getDate());
					dataBase[index][inner].setCases(entry.getCases() + dataBase[index][inner].getCases());
					dataBase[index][inner].setDeaths(entry.getDeaths() + dataBase[index][inner].getDeaths());
				} else {
					return false;
				}
			}
			return true;
		}

		void createInitial(vector<cvsdata> list) {
			for (int i = 0; i < list.size(); i++) {
				DataEntry entry;
				entry.load(list[i]);
				add(entry);
			}
		}

		DataEntry get(string country) {
			int index = hash(country);
			int inner = ifContain(country, index);
			if (inner == -1) {
				DataEntry null;
				return null;
			} else {
				return dataBase[index][inner];
			}
		}

		void remove(string country) {
			int index = hash(country);
			int inner = ifContain(country, index);
			if (inner == -1) {
				cout << "\nThere is no such country\n" << endl;
			} else {
				dataBase[index].erase(dataBase[index].begin() + inner);
				cout << "Entry of " << country << " successfully deleted\n" << endl;
			}
		}

		void display() {
			cout << "\nCovid Database" << endl;
			for (int i = 0; i < tableSize; i++) {
				cout << "slot " << i << ": ";
				for (int j = 0; j < dataBase[i].size(); j++) {
					if (j != 0) {
						cout << "-> ";
					}
					cout << dataBase[i][j].getDate() << ", ";
					cout << dataBase[i][j].getCountry() << ", ";
					cout << dataBase[i][j].getCases() << ", ";
					cout << dataBase[i][j].getDeaths() << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
};

void userInterface(vector<cvsdata> recordList) {
	CovidDB databaseC;
	int choice = 1;
	bool initial = false;

	while (choice != 0) {
		cout << "Covid Tracker" << endl;
		cout << "1. Create the initial hash table" << endl;
		cout << "2. Add a new data entry" << endl;
		cout << "3. Get a data entry" << endl;
		cout << "4. Remove a data entry" << endl;
		cout << "5. Display hash table" << endl;
		cout << "0. Quit the system" << endl;
		cout << "Please choose the operation you want: ";
		cin >> choice;

		if (choice == 1) {					// create initial hash table
			if (!initial) {
				databaseC.createInitial(recordList);
				cout << "\nInitial hash table successfully created\n" << endl;
				initial = true;
			} else {
				cout << "\nFailed to create, hash table already exist\n" << endl;
			}
			
		} else if (choice == 2) {		// add data
			DataEntry addition;
			string date;
			string country;
			int cases;
			int deaths;

			cout << "\nEnter the date (mm/dd/yyyy) > ";
			cin >> date;
			addition.setDate(date);
			cout << "Enter the country > ";
			cin >> country;
			addition.setCountry(country);
			cout << "Enter number of cases > ";
			cin >> cases;
			addition.setCases(cases);
			cout << "Enter number of death cases > ";
			cin >> deaths;
			addition.setDeaths(deaths);
			
			if (databaseC.add(addition)) {
				cout << "Entry successfully added\n" << endl;
			} else {
				cout << "Invalid date, failed to add entry\n" << endl;
			}
			
		} else if (choice == 3) {		// get data
			string country;
			cout << "\nEnter the country name > ";
			cin >> country;
			DataEntry result = databaseC.get(country);

			cout << "Entry found: ";
			cout << result.getDate() << ", ";
			cout << result.getCountry() << ", ";
			cout << result.getCases() << ", ";
			cout << result.getDeaths() << " ";
			
		} else if (choice == 4) {		// remove data
			string country;
			cout << "\nEnter the country name > ";
			cin >> country;
			databaseC.remove(country);
			
		} else if (choice == 5) {		// display hash table
			databaseC.display();
			
		} else {
			cout << "Quitting..." << endl;
			choice = 0;
		}
	}
}

int main() {
  vector<cvsdata> recordList;

  // open the file "WHO-COVID-Data.csv"
  ifstream file("Afganistan.csv");

  if (file.is_open()) {

    cout << "Open File Success" << endl;

    string line;

    while(getline(file, line)){
      
      // split the content in each line
      vector<string> result = split(line, ',');

      // wrap up all the data in the string to a cvsdata type and push it to the vector
      cvsdata oneEntry = cvsdata(result[0], result[1], std::stoi(result[2]), std::stoi(result[3]));
      recordList.push_back(oneEntry);
    }
    cout << "There are " << recordList.size() << " records in total.\n" << endl;
    
  } else {
    cout << "Open file failed" << endl;
  }
	
	userInterface(recordList);
	
  return 0;
}