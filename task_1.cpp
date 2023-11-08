#include <iostream>
#include <vector>

using namespace std;

class StimulRead
{
public:
	StimulRead()
	{
		id_page_.resize(100000);
		page_id_.resize(1000);
	}

	void IncomingRequests(istream& in)
	{
		int num, idUser, page;
		in >> num;
		for (int i = 0; i < num; ++i)
		{
			string event;
			in >> event;
			if (event == "READ")
			{
				in >> idUser;
				in >> page;
				for (int j = id_page_[idUser]; j < page; ++j)
				{
					++page_id_[j];
				}
				id_page_[idUser] = page;
			}
			else
			{
				in >> idUser;
				page = id_page_[idUser];
				if (page == 0)
				{
					cout << 0 << endl;
				}
				else if (page_id_[0] == 1)
				{
					cout << 1 << endl;
				}
				else
				{
					cout << (float)(page_id_[0] - 1 - (page_id_[page - 1] - 1)) /
								 (float)(page_id_[0] - 1)
							  << endl;
				}
			}
		}
	}


private:
	vector<int> id_page_;
	vector<int> page_id_;
};


int main()
{
	StimulRead stimulRead;
	stimulRead.IncomingRequests(std::cin);
}
