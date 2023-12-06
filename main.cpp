#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
using namespace std;

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

int	main() {
	int score = 0;
	while (true) 
	{
		vector<string> uuids(100);
		generate(uuids.begin(), uuids.end(), generate_uuid);

		char missing_char = "0123456789abcdef"[rand() % 16];
		uuids.erase(remove_if(uuids.begin(), uuids.end(), [missing_char](const string& uuid) 
		{
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
			cout << "Time is up. Score counting is stopped" << endl;
			cout << "Your final score is " << score << endl;
			cout << "Thanks for trying the game. You can try the game by typing 'start'" << endl;
			break;
		}

		char guess = future.get();
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = end - start;
		int remaining_time = 30 - static_cast<int>(elapsed.count());

		if (guess == missing_char)
		{
			score += remaining_time;
			cout << "\nGood Work. The remaining time is being converted to score point(s) " << endl;
			cout << "Your are reworded with " << score << " score point(s) \n" << endl;
		}
		else
		{
			cout << "Wrong guess. The correct character was: " << missing_char << endl;
			cout << "Unfortunately this the end\nFinal score is being calculated" << endl;
			cout << "Your final score is: " << score << endl;
			cout << "Thanks for trying the game. You can try the game by typing 'try again'\n" << endl;
			break;
		}
	}
}