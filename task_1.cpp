#include <iostream>
#include <vector>

using namespace std;

class StimulRead
{
public:
	StimulRead()
	{
		reader_pages_.resize(100000);
		page_readers_.resize(1000);
	}

	void IncomingRequests(istream& in, ostream& out)
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
				for (int j = reader_pages_[idUser]; j < page; ++j)
				{
					++page_readers_[j];
				}
				reader_pages_[idUser] = page;
			}
			else
			{
				in >> idUser;
				page = reader_pages_[idUser];
				if (page == 0)
				{
					out << 0 << endl;
				}
				else if (page_readers_[0] == 1)
				{
					out << 1 << endl;
				}
				else
				{
					out << setprecision(6)
						<< (float)(page_readers_[0] - 1 - (page_readers_[page - 1] - 1)) / (page_readers_[0] - 1)
						<< endl;
				}
			}
		}
	}


private:
	vector<int> reader_pages_;
	vector<int> page_readers_;
};


int main()
{
	StimulRead stimulRead;
	stimulRead.IncomingRequests(std::cin, std::cout);
}
