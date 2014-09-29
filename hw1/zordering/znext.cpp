#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <limits>

using namespace std;

void reverse(string& str) {
	int i, j;

	i = 0;
	j = str.size() - 1;
	while (i < j) {
		swap(str[i], str[j]);
		i++;
		j--;
	}
}

string get_zvalue(int order, vector<unsigned>& coordinates) {
	string res;
	int i, j;
	for (i = 0; i < order; ++i) {
		for (j = coordinates.size() - 1; j >= 0; --j) {
			res += (coordinates[j] % 2) + '0';
			coordinates[j] /= 2;
		}
	}
	
	reverse(res);

	return res;
}

string get_next_zvalue(string& zvalue) {
	string res;
	int carry;
	int sum;
	int i;

	carry = 1;
	for (i = zvalue.size() - 1; i >= 0; --i) {
		sum = (zvalue[i] - '0') + carry;
		res += (sum % 2) + '0';
		carry = sum / 2;
	}
	if (carry > 0)
		res += carry + '0';
	
	reverse(res);

	return res;
}

vector<unsigned> get_coordinates(int order, int dim, string& next_zvalue) {
	vector<unsigned> res(dim, 0);
	int i, j, k;
	unsigned w;

	if (order == 0)
		w = 1;
	else
		w = 1 << (order - 1);
	j = 0;
	for (i = 0; i < order; ++i) {
		for (k = 0; k < dim; ++k, ++j) {
			res[k] += w * (next_zvalue[j] - '0');
		}
		w /= 2;
	}

	return res;
}

bool isdigit(char c) {
	return c >= '0' && c <= '9';
}

/* check if the input string represents a non-negative integer that 
 * is no more than 2^32 - 1
 */
bool check(char *str) {
	int len = strlen(str);
	int i;

	for (i = 0; i < len; ++i) {
		if (isdigit(str[i]))
			break;
	}
	
	if (i > 1 || (str[i] == '0' && i < len - 1)) {
		cout<<str<<": non-integer input"<<endl;
		return false;
	}

	if (i > 0) {
		if (str[0] == '-') {
			cout<<str<<": negative integer"<<endl;
			return false;
		}
		else if (str[0] == '+') {
			str++;
			len--;
		}
		else {
			cout<<str<<": non-integer input"<<endl;
			return false;
		}
	}

	i++;
	while (i < len) {
		if (!isdigit(str[i])) {
			cout<<str<<": non-integer input"<<endl;
			return false;
		}
		i++;
	}

	if (len > 32) {
		cout<<str<<": out of range"<<endl;
		return false;
	}

	if (len == 32 && strcmp(str, "4294967295") > 0) {
		cout<<str<<": out of range"<<endl;
		return false;
	}

	return true;
}

bool parse_command_line(int argv, char **argc, int& order, 
	vector<unsigned>& coordinates) {
	unsigned o, d;

	if (!check(argc[2])) {
		return false;
	}
	sscanf(argc[2][0] == '+' ? argc[2] + 1 : argc[2], "%u", &o);
	if (o > 32) {
		cout<<"order should be no more than 32"<<endl;
		return false;
	}
	order = o;

	if (!check(argc[4])) {
		return false;
	}
	sscanf(argc[4][0] == '+' ? argc[4] + 1 : argc[4], "%u", &d);
	if (d == 0) {
		cout<<"dimension should be a positive number"<<endl;
		return false;
	}

	coordinates.resize(d);
	unsigned cnt = 0;
	unsigned upper = (1LL << order) - 1;
	for (unsigned i = 0; i < d; ++i) {
		if (!check(argc[5 + i]))
			return false;
		cout<<"process "<<i<<" coordinate"<<endl;
		sscanf(argc[5 + i][0] == '+' ? argc[5 + i] + 1 : argc[5 + i], 
			"%u", &coordinates[i]);
		if (coordinates[i] > upper) {
			cout<<coordinates[i]<<": out of range"<<endl;
			return false;
		}
		if (coordinates[i] == upper)
			cnt++;
	}	
	if (cnt == d) {
		cout<<"the input point is the last one"<<endl;
		return false;
	}

	return true;
}

int main(int argv, char **argc) {
	vector<unsigned> coordinates;
	int order;

	if (!parse_command_line(argv, argc, order, coordinates)) {
		exit(1);
	}
	
	string zvalue = get_zvalue(order, coordinates);
	string next_zvalue = get_next_zvalue(zvalue);
	coordinates = get_coordinates(order, coordinates.size(), next_zvalue);

	cout<<argc[0];
	for (int i = 1; i < argv; ++i) {
		cout<<" "<<argc[i];
	}
	cout<<endl;

	bool is_first = true;
	for (unsigned c : coordinates) {
		if (is_first) {
			is_first = false;
		}
		else
			cout<<" ";
		cout<<c;
	}
	cout<<endl;
	return 0;
}
