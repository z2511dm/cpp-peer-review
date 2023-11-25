
struct ArgLoadPerson
{
	int db_connection_timeout = 0;
	bool db_allow_exceptions = false;
	DBLogLevel db_log_level;
	int min_age = 0;
	int max_age = 150;
	string_view name_filter;

	ArgLoadPerson& SetDbConnectionTimeout(int val)
	{
		db_connection_timeout = val;
		return *this;
	}
	ArgLoadPerson& SetDbAllowExceptions(bool val)
	{
		db_allow_exceptions = val;
		return *this;
	}
	ArgLoadPerson& SetDbLogLevel(DBLogLevel val)
	{
		db_log_level = val;
		return *this;
	}
	ArgLoadPerson& SetMinAge(int val)
	{
		min_age = val;
		return *this;
	}
	ArgLoadPerson& SetMaxAge(int val)
	{
		max_age = val;
		return *this;
	}
	ArgLoadPerson& SetNameFilter(string_view val)
	{
		name_filter = val;
		return *this;
	}
};

vector <Person> LoadPersons(string_view db_name, ArgLoadPerson args )
{
	DBConnector connector(args.db_allow_exceptions, args.db_log_level);
	DBHandler db;
	if (db_name.starts_with("tmp."s))
	{
		db = connector.ConnectTmp(db_name, args.db_connection_timeout);
	}
	else
	{
		db = connector.Connect(db_name, args.db_connection_timeout);
	}
	if (!args.db_allow_exceptions && !db.IsOK())
	{
		return {};
	}

	ostringstream query_str;
	query_str << "from Persons "s
			  << "select Name, Age "s
			  << "where Age between "s << args.min_age << " and "s << args.max_age << " "s
			  << "and Name like '%"s << db.Quote(args.name_filter) << "%'"s;
	DBQuery query(query_str.str());

	vector <Person> persons;
	for (auto [name, age]: db.LoadRows<string, int>(query))
	{
		persons.push_back({ move(name), age });
	}
	return persons;
}