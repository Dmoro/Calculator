//============================================================================
// Name        : FirstProject.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

void findOper(string input, string oper, int numOfOper[], int locOfOper[4][4]){
	int col;

	if(oper == "+"){
		col = 0;
	} else if(oper == "-"){
		col = 1;
	} else if(oper == "*"){
		col = 2;
	} else if(oper == "/"){
		col = 3;
	}

	while(input.find(oper, locOfOper[numOfOper[col]-1][col]+1) < input.size()){
		locOfOper[numOfOper[col]][col] = input.find(oper, locOfOper[numOfOper[col]-1][col]+1);
		numOfOper[col]++;
	}
}

class Expression{
private:
	vector<string> opers;
	vector<double> nums;

public:
	vector<double> getNums(){return this->nums;};
	vector<string> getOpers(){return this->opers;};

	Expression(vector<string> opers, vector<double> nums){
		this->opers = opers;
		this->nums = nums;
	}

	void removeNum(int index){
		nums.erase(nums.begin()+index);
	}

	void removeOper(int index){
		opers.erase(opers.begin()+index);
	}

	void insertNum(int index, double value){
		nums.insert(nums.begin() + index, value);
	}

	string toString(){
		string out = "";
		string nl = "\n";

		if(opers.size() <= 0){
			out.append(static_cast<ostringstream*>( &(ostringstream() << nums.at(0)) )->str() + nl);
		} else {
			out.append(nl + "----------" + nl);
			for(int i = 0; i < opers.size(); i++){
				out.append(opers.at(i) + " ");
			}
			out.append(nl);

			for(int i = 0; i < nums.size(); i++){
				out.append(static_cast<ostringstream*>( &(ostringstream() << nums.at(i)) )->str() + " ");
			}
			out.append(nl + "----------" + nl);
		}

		return out;
	}
};

int findOper(string oper, vector<string> vect){
	for(int i = 0; i < vect.size(); i++){
		if(vect.at(i).compare(oper) == 0){
			return i;
		}
	}

	return -1;
}

Expression calculate(Expression exps){

	//cout << exps.toString(); //FOR DEBUGGING

	if(exps.getOpers().size() < 1){
		exps.getNums().at(0);
		return exps;

	} else if (findOper("*", exps.getOpers()) >= 0){
		int locOfOper = findOper("*", exps.getOpers());
		double multNum = exps.getNums().at(locOfOper) * exps.getNums().at(locOfOper+1);
		//erase
		exps.removeNum(locOfOper);
		exps.removeNum(locOfOper);
		exps.removeOper(locOfOper);
		//insert new
		exps.insertNum(locOfOper, multNum);
		//recursion
		exps = calculate(exps);
	}  else if (findOper("/", exps.getOpers()) >= 0){
		int locOfOper = findOper("/", exps.getOpers());

		//CHECK FOR /0
		if(exps.getNums().at(locOfOper+1) == 0){
			throw invalid_argument("Can't divide by 0");
		}

		double divNum = (double) exps.getNums().at(locOfOper) / (double) exps.getNums().at(locOfOper+1);
		exps.removeNum(locOfOper);
		exps.removeNum(locOfOper);
		exps.removeOper(locOfOper);
		//insert new
		exps.insertNum(locOfOper, divNum);
		//recursion
		exps = calculate(exps);
	} else if (findOper("-", exps.getOpers()) >= 0){
		int locOfOper = findOper("-", exps.getOpers());
		double subNum = exps.getNums().at(locOfOper) - exps.getNums().at(locOfOper+1);
		//erase
		exps.removeNum(locOfOper);
		exps.removeNum(locOfOper);
		exps.removeOper(locOfOper);
		//insert new
		exps.insertNum(locOfOper, subNum);
		//recursion
		exps = calculate(exps);
	} else if (findOper("+", exps.getOpers()) >= 0){
		int locOfOper = findOper("+", exps.getOpers());
		double addNum = exps.getNums().at(locOfOper) + exps.getNums().at(locOfOper+1);
		//erase
		exps.removeNum(locOfOper);
		exps.removeNum(locOfOper);
		exps.removeOper(locOfOper);
		//insert new
		exps.insertNum(locOfOper, addNum);
		//recursion

		exps = calculate(exps);
	}

	return exps;
}

int main() {
	//Simple calculator
	string input;
	bool isDone = false;
	cout << "Welcome to the easy calculator. \nEnter what you would like to calculate: " <<endl;


	while(!isDone){
		cout << "---------------------" << endl;
		getline(cin, input);

		vector<string> opers;
		vector<double> nums;
		string out;

		const string NUMBERS = ".0123456789";
		const string OPERS = "+-*/";



		//parse input
		string currNum = "";
		for(int i = 0; i < input.length(); i++){
			if(NUMBERS.find(input[i]) < 100){
				currNum+=input[i];

			} else if (OPERS.find(input[i]) < 100){
				string s(1, input[i]);
				//opers.push_back("_");
				opers.push_back(s);
				nums.push_back(atof(currNum.c_str()));
				currNum = "";
			} else {
				cout << "Invalid input. Exiting..." << endl;
				isDone = true;
				return 0;
			}
		}
		if(currNum.length() > 0){
			nums.push_back(atof(currNum.c_str()));
			currNum = "";
		}

		Expression exps(opers, nums);

		try{
			exps = calculate(exps);
			cout << endl << "  " << exps.toString();
		} catch (const invalid_argument& e){
			cout << endl << "  " << e.what() << endl;
		}

	}

	return 0;
};

//  Extra Code
//	int locOfOper[4][4];
//	int numOfOper[4];
//
//	//clear out
//	for(int i = 0; i < 4; i++){
//		numOfOper[i] = 0;
//	}
//	for(int i = 0; i < 4; i++){
//		for(int j= 0; j < 4; j++){
//			locOfOper[i][j] = 0;
//		}
//	}
//
//	cout << input << endl << endl;
//
//	//find operators
//	findOper(input, "+", numOfOper, locOfOper);
//	findOper(input, "-", numOfOper, locOfOper);
//	findOper(input, "*", numOfOper, locOfOper);
//	findOper(input, "/", numOfOper, locOfOper);
//
//	//print
//	for(int i = 0; i < 4; i++){
//		cout << numOfOper[i] << " ";
//	}
//	cout << endl << "-------" << endl;
//	for(int i = 0; i < 4; i++){
//		for(int j= 0; j < 4; j++){
//				cout << locOfOper[i][j] << " ";
//		}
//		cout << endl;
//	}
