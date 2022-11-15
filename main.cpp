#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, char del) {
    vector<string> result;
    // declaring temp string to store the curr "word" upto del
    string temp = "";
    for(int i =0; i<(int)str.size(); i++){
        // If current char is not ',', then append it to the "word", otherwise you have completed the word, and start the next word
        if (str[i] != del) {
	    temp += str[i];
        } else {
            result.push_back(temp);
            temp = "";
        }
    }
    // push the last substring after the last ',' to the result
    result.push_back(temp);
    return result;
}

class cvsdata {
  	string date;
  	string country;
  	int cases;
  	int deaths;
	public:
  	cvsdata(string date, string country, int cases, int deaths){
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
			DataEntry(cvsdata data) {
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
    vector<DataEntry> dataBase;

  public:
    // constructor
    CovidDB () {
      dataBase = vector<DataEntry>(tableSize);
			for(int i = 0; i < tableSize; i++) {
				// dataBase[i] = NULL;
      }
		}

    // hash function
    int hash(string country) { 
			int sum = 0; 
  		for (int i = 0; i < country.length(); i++) {
				sum += (i + 1) * int(country[i]);
    	} 
  		return sum % tableSize;
		}

    // add a key to hash table
    bool add(DataEntry entry) {
			int index = hash(entry.getCountry());
			return true;
		}

		DataEntry get(string country) {
			return dataBase[hash(country)];
		}

    // delete a key from hash table
		void remove(string country) {
			dataBase[hash(country)] = NULL;
		}
    
      // Display the contents
    void display() {
      cout << "Hash Table" << endl;
      for(int i = 0; i < tableSize; i++) {
        cout << i << "-> " ;
      }
		}
};

void userInterface(vector<cvsdata> recordList) {
	int choice = 1;

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

		if (choice == 1) {
			
		} else if (choice == 2) {
			
		} else if (choice == 3) {
			
		} else if (choice == 4) {
			
		} else if (choice == 5) {
			
		} else {
			cout << "Thank you for using" << endl;
		}
	}
}

int main() {
	
  vector<cvsdata> recordList;

  // open the file
  ifstream file("WHO-COVID-Data-Test.csv");

  if(file.is_open()){

    cout << "Open File Success" << endl;

    string line;

    while(getline(file, line)){
      
      // split the content in each line
      vector<string> result = split(line, ',');

      // wrap up all the data in the string to a cvsdata type and push it to the vector
      cvsdata oneEntry = cvsdata(result[0], result[1], std::stoi(result[2]), std::stoi(result[3]));
      recordList.push_back(oneEntry);
    }
    cout << "There are " << recordList.size() << " records in total." << endl;
    
  }else{
    cout << "Open file failed" << endl;
  }

	// userInterface(recordList);
	
  return 0;
	
}