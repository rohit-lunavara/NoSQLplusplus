#include <iostream>
#include <memory>

#include "Client.h"
#include "HashClient.h"

using namespace std;


int main()
{
	{
		unique_ptr<Client<int, string>> client 
		 = make_unique<HashClient<int, string>>();

		cout << client->set(1, "world", 'x') << '\n';
		cout << client->set(1, "change", 'x') << '\n';

		try {
			cout << client->get(1) << "\n";
		} catch (const std::exception& e) {
			cout << e.what() << '\n';
		}
	}


	{
		unique_ptr<Client<string, string>> client 
		 = make_unique<HashClient<string, string>>();

		cout << client->set("hello", "world", 'n') << '\n';
		cout << client->set("hello", "change", 'n') << '\n';

		try {
			cout << client->get("hello") << "\n";
		} catch (const std::exception& e) {
			cout << e.what() << '\n';
		}
	}

	return 0;
}