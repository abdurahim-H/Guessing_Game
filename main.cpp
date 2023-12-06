#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
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

int main() {
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

		char guess;
		cin >> guess;

		if (guess == missing_char) 
		{
			score += 30;
			cout << "Correct! Your score is now " << score << "." << endl;
		} else {
			cout << "Incorrect. Your final score is " << score << "." << endl;
			break;
		}
	}

	return 0;
}