#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <future>
using namespace std;

// ANSI escape codes for color
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

string generate_uuid()
{
	string	uuid;
	int	i;

	i = 0;
	while (i < 32)
	{
		uuid += "0123456789abcdf"[rand() % 16];
		i++;
	}
	return (uuid);
}

void guess_missing_char() {
	int score = 0;
	while (true) {
		vector<string> uuids(100);
		generate(uuids.begin(), uuids.end(), generate_uuid);

		char missing_char = "0123456789abcdef"[rand() % 16];
		uuids.erase(remove_if(uuids.begin(), uuids.end(), [missing_char](const string& uuid) {
			return uuid.find(missing_char) != string::npos;
		}), uuids.end());

		auto uuid_iter = uuids.begin();
		while (uuid_iter != uuids.end())
		{
			cout << *uuid_iter << endl;
			++uuid_iter;
		}

		auto start = chrono::high_resolution_clock::now();

		auto future = async(launch::async, []() {
			char guess;
			cout << endl;
			cin >> guess;
			return guess;
		});

		if (future.wait_for(chrono::seconds(30)) != future_status::ready) 
		{
			cout << RED << "Time is up. Score counting is stopped" << RESET << endl;
			cout << "Your final score is " << score << endl;
			cout << "Thanks for trying the game. You can try the game by typing 'start'" << endl;
			break;
		}

		char guess = future.get();
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = end - start;
		int remaining_time = 30 - static_cast<int>(elapsed.count());

		cout << YELLOW << "Remaining time: " << remaining_time << " seconds\n" << RESET;

		if (guess == missing_char)
		{
			score += remaining_time;
			cout << GREEN << "\nGood Work. The remaining time is being converted to score point(s) " << RESET << endl;
			cout << "Your are reworded with " << score << " score point(s) \n" << endl;
		}
		else
		{
			cout << RED << "Wrong guess. The correct character was: " << missing_char << RESET << endl;
			cout << RED << "Unfortunately this the end\nFinal score is being calculated" << RESET << endl;
			cout << RED << "Your final score is: " << score << RESET << endl;
			cout << GREEN << "Thanks for trying the game. You can try the game by typing 'start'\n" << RESET << endl;
			break;
		}
	}
}

int main() {
	string input;
	while (true) 
	{
		cout << YELLOW << "Type 'start' to start the game" << RESET << endl;
		cin >> input;
		if (input == "start") 
		{
			cout << GREEN << "\t\t\t\tGame started.\n\t\tPlease try to guess correct missing character\n" << RESET;
			guess_missing_char();
		}
		else 
		{
			cout << RED << "Invalid input. Please type 'start' to start the game" << RESET << endl;
		}
	}
	return 0;
}