#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <limits>

using namespace std;

int moves[4][2]= {{0,1}, {1,0}, {0,-1}, {-1,0}};
char map[4] = {'0', '1', '3', '2'};

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

string get_hvalue(int order, unsigned x, unsigned y) {
	string res, tmp;
	int i, j, v;

	for (i = 0; i < order; ++i) {
		v = ((x & 1) << 1) + (y & 1);
		tmp += map[v];
		x >>= 1;
		y >>= 1;
	}
	
	reverse(tmp);
	for (i = 0; i < tmp.size(); ++i) {
		if (tmp[i] == '0') {
			for (j = i + 1; j < tmp.size(); ++j) {
				if (tmp[j] == '1')
					tmp[j] = '3';
				else if (tmp[j] == '3')
					tmp[j] = '1';
			}
		}
		else if (tmp[i] == '3') {
			for (j = i + 1; j < tmp.size(); ++j) {
				if (tmp[j] == '0')
					tmp[j] = '2';
				else if (tmp[j] == '2')
					tmp[j] = '0';
			}
		}
	}

	for (i = 0; i < tmp.size(); ++i) {
		if (tmp[i] == '0')
			res += "00";
		else if (tmp[i] == '1')
			res += "01";
		else if (tmp[i] == '2')
			res += "10";
		else 
			res += "11";
	}

	return res;
}

string get_next_hvalue(string& hvalue) {
	string res;
	int carry;
	int sum;
	int i;

	carry = 1;
	for (i = hvalue.size() - 1; i >= 0; --i) {
		sum = (hvalue[i] - '0') + carry;
		res += (sum % 2) + '0';
		carry = sum / 2;
	}
	if (carry > 0)
		res += carry + '0';
	
	reverse(res);

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

bool parse_command_line(char **argv, int& order, 
	unsigned& x, unsigned& y) {
	unsigned o;

	if (!check(argv[2])) {
		return false;
	}
	sscanf(argv[2][0] == '+' ? argv[2] + 1 : argv[2], "%u", &o);
	if (o > 32) {
		cout<<"order should be no more than 32"<<endl;
		return false;
	}
	order = o;

	unsigned upper = (1LL << order) - 1;
	unsigned t;
	for (unsigned i = 0; i < 2; ++i) {
		if (!check(argv[3 + i]))
			return false;
		sscanf(argv[3 + i][0] == '+' ? argv[3 + i] + 1 : argv[3 + i], 
			"%u", &t);
		if (t > upper) {
			cout<<t<<": out of range"<<endl;
			return false;
		}
		if (i == 0)
			x =  t;
		else
			y = t;
	}	

	return true;
}

bool inrange(int order, long long x, long long y) {
	unsigned upper = (1LL << order) - 1;

	if (x < 0 || x > upper)
		return false;
	if (y < 0 || y > upper)
		return false;
	return true;
}

bool islast(int order, string &hvalue) {
	string last;

	order = order * 2;
	while (order--) {
		last += '1';
	}
	return hvalue == last;
}

int main(int argc, char **argv) {
	int order;
	unsigned x, y;

	if (!parse_command_line(argv, order, x, y))
		exit(1);
	string hvalue = get_hvalue(order, x, y);
	if (islast(order, hvalue)) {
		cout<<"The input point is the last one"<<endl;
		return false;
	}

	string next_hvalue = get_next_hvalue(hvalue);
	string value;
	long long nx, ny;

	for (int i = 0; i < 4; ++i) {
		nx = 1LL * x + moves[i][0];
		ny = 1LL * y + moves[i][1];
		if (!inrange(order, nx, ny))
			continue;
		value = get_hvalue(order, (unsigned)nx, (unsigned)ny);
		if (value == next_hvalue) {
			cout<<nx<<" "<<ny<<endl;
			break;
		}
	}

	return 0;
}
