// Carmel Laurentia
// assignment4.cpp
// 11/14/2022
// Purpose: Implement our own version of Covid Data Tracker.
//		This assignment will practice your STL in C++ (vector), file 
//		operation in C++, hash table operations, and collision resolving. 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// This function parse a line in the input file by seperator character
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

// A class that contain the details of cvsdata
class cvsdata {
  string date;
  string country;
  int cases;
  int deaths;
	public:
		// constructor
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

// A class that contain the detail of an entry
class DataEntry {
    private:
      string date;
      string country;
      int c_cases;
      int c_deaths;
    public:
			// constructor
			DataEntry() {}

			// This function load the detail from cvsdata type into DataEntry type
			// pre: the cvsdata given is valid
			// post: load the detail in cvsdata into current DataEntry
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

// A class for covid database, implementing hash table with separate chaining
class CovidDB {
	private:
    int tableSize = 17;
    vector<vector<DataEntry>> dataBase;

	public:
	// constructor
		CovidDB () {
      dataBase = vector<vector<DataEntry>>(tableSize);
		}

		// This function calculate the index of data position
		// pre: the string given is valid
		// post: return the index of where the data should be stored
		int hash(string country) { 
			int sum = 0; 
  		for (int i = 0; i < country.length(); i++) {
				sum += (i + 1) * int(country[i]);
    	} 
  		return sum % tableSize;
		}

	// helper function to check the index contain the country given
		// return index if found, -1 otherwise
		int ifContain(string country, int index) {
			for (int i = 0; i < dataBase[index].size(); i++) {
				if (dataBase[index][i].getCountry() == country) {
					return i;
				}
			}
			return -1;
		}

		// check the order of the date
		// pre: the string given are valid
		// post: return true if date1 is later than date2, false otherwise
		bool compareDate(string date1, string date2) {
			vector<string> first = split(date1, '/');
			vector<string> second = split(date2, '/');
			
			if (stoi(first[2]) < stoi(second[2])) {			// compare year
				return false;
			} else if (stoi(first[2]) > stoi(second[2])) {
				return true;
			} else {
				if (stoi(first[0]) < stoi(second[0])) {		// compare month
					return false;
				} else if (stoi(first[0]) > stoi(second[0])) {
					return true;
				} else {
					if (stoi(first[1]) <= stoi(second[1])) {		// compare day
						return false;
					} else {
						return true;
					}
				}
			}
		}

	// add a DataEntry type into the database
		// return true if successfully inserted, false otherwise
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

	// create initial hash table based on the data read
		void createInitial(vector<cvsdata> list) {
			for (int i = 0; i < list.size(); i++) {
				DataEntry entry;
				entry.load(list[i]);
				add(entry);
			}
		}

	// get a data entry from the database
		// return the data entry if found, return null otherwise
		DataEntry get(string country) {
			int index = hash(country);
			int inner = ifContain(country, index);
			if (inner == -1) {
				DataEntry null;
				null.setCases(0);
				return null;
			} else {
				return dataBase[index][inner];
			}
		}

	// remove a data entry from the database
		void remove(string country) {
			int index = hash(country);
			int inner = ifContain(country, index);
			if (inner == -1) {
				cout << "Invalid input, there is no such entry\n" << endl;
			} else {
				dataBase[index].erase(dataBase[index].begin() + inner);
				cout << "Entry of " << country << " successfully deleted\n" << endl;
			}
		}

	// display all the data entry stored in the database
		void display() {
			cout << "\nCovid Database" << endl;
			for (int i = 0; i < tableSize; i++) {
				cout << "\tSLOT " << i << ": ";
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

// A user interface that allow the user to interact with the database
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
			if (initial) {
				DataEntry addition;
				string date;
				string country;
				int cases;
				int deaths;

				cout << "\nEnter the date (mm/dd/yyyy) > ";
				cin >> date;
				addition.setDate(date);
				cout << "Enter the country > ";
				cin.ignore();
				getline(cin, country);
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
			} else {
				cout << "\nInvalid choice, initial table hasn't been created\n" << endl;
			}
			
		} else if (choice == 3) {		// get data
			if (initial) {
				string country;
				cout << "\nEnter the country name > ";
				cin.ignore();
				getline(cin, country);
				DataEntry result = databaseC.get(country);

				if (result.getCases() == 0) {
					cout << "Not found, there is no such entry\n" << endl;
				} else {
					cout << "Entry found: ";
					cout << result.getDate() << ", ";
					cout << result.getCountry() << ", ";
					cout << result.getCases() << ", ";
					cout << result.getDeaths() << "\n" << endl;
				}
				
			} else {
				cout << "\nInvalid choice, initial table hasn't been created\n" << endl;
			}
			
		} else if (choice == 4) {		// remove data
			if (initial) {
				string country;
				cout << "\nEnter the country name > ";
				cin.ignore();
				getline(cin, country);
				databaseC.remove(country);
			} else {
				cout << "\nInvalid choice, initial table hasn't been created\n" << endl;
			}
			
		} else if (choice == 5) {		// display hash table
			if (initial) {
				databaseC.display();
			} else {
				cout << "\nInvalid choice, initial table hasn't been created\n" << endl;
			}
			
		} else {
			cout << "\nQuitting..." << endl;
			choice = 0;
		}
	}
}

int main() {
  vector<cvsdata> recordList;

  // open the file "WHO-COVID-Data.csv"
  ifstream file("WHO-COVID-Data.csv");

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