#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>
#include <cassert>

using namespace std;

class Domain
{
public:
	Domain(string  name)
			:
			name_(std::move(name))
	{
		reverse(name_.begin(), name_.end());
		name_ += '.';
	}

	bool operator==(const Domain& other) const
	{
		return name_ == other.name_;
	}

	bool operator<(Domain& other) const
	{
		return lexicographical_compare(name_.begin(), name_.end(), other.name_.begin(), other.name_.end());
	}

	bool IsSubdomain(Domain& other) const
	{
		if (other.name_.size() < name_.size()) return false;
		return name_ == other.name_.substr(0, name_.size());
	}

private:
	string name_;
};

class DomainChecker
{
public:
	template<typename iter>
	DomainChecker(iter begin, iter end)
			:
			fDomains_(begin, end)
	{
		sort(fDomains_.begin(), fDomains_.end());
		auto last = unique(fDomains_.begin(), fDomains_.end(), [](Domain& lhs, Domain& rhs)
		{ return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs); });
		fDomains_.erase(last, fDomains_.end());
	}

	bool IsForbidden(Domain testDomain)
	{
		auto iter = upper_bound(fDomains_.begin(), fDomains_.end(), testDomain);
		if (iter != fDomains_.begin() && (iter - 1)->IsSubdomain(testDomain)) return true;
		return false;
	}

private:
	vector<Domain> fDomains_;
};

vector<Domain> ReadDomains(istream& in, size_t num)
{
	vector<Domain> result;
	for (size_t i = 0; i < num; ++i)
	{
		string line;
		getline(in, line);
		result.emplace_back(line);
	}
	return result;
}

template<typename Number>
Number ReadNumberOnLine(istream& input)
{
	string line;
	getline(input, line);

	Number num;
	std::istringstream(line) >> num;

	return num;
}

void tests()
{
	{
		Domain testDomain1("bd.gdz.ru");
		Domain testDomain2("gdz.ru");
		Domain testDomain3("gdz.ru");

		assert(testDomain2.IsSubdomain(testDomain1));
		assert(!testDomain1.IsSubdomain(testDomain2));
		assert(!(testDomain1 == testDomain2));
		assert(testDomain2 == testDomain3);
		assert(testDomain2 < testDomain1);
		assert(!(testDomain1 < testDomain2));
	}

	{
		istringstream in1("gdz.ru\n"
						  "maps.me\n");
		const std::vector<Domain> forbidden_domains = ReadDomains(in1, 4);

		assert(forbidden_domains[0] == Domain("gdz.ru"));
		assert(forbidden_domains[1] == Domain("maps.me"));
	}

	{
		istringstream in1("gdz.ru\n"
						  "maps.me\n"
						  "m.gdz.ru\n"
						  "com");
		const std::vector<Domain> forbidden_domains = ReadDomains(in1, 4);
		DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

		assert(checker.IsForbidden(Domain("gdz.ru")));
		assert(checker.IsForbidden(Domain("maps.me")));
		assert(checker.IsForbidden(Domain("as.fg.db.m.gdz.ru")));
		assert(checker.IsForbidden(Domain("maps.com")));
		assert(!checker.IsForbidden(Domain("ttt.ru")));
	}
	cout << "Tests completed" << endl;
}

int main()
{
	tests();

	/*const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
	DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

	const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
	for (const Domain& domain: test_domains)
	{
		cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
	}*/
}