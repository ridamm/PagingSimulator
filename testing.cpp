#include <bits/stdc++.h>

using namespace std;

int main()
{
	std::vector<pair<int,int>*> v(2);
	pair<int,int> a={1,2},b={10,20},c={55,66};
	v[0]=&a;
	v[1]=&b;

	std::vector<pair<int,int>*>::iterator it=v.begin();
	*it=&c;
	for(it=v.begin();it!= v.end();it++)
	{
		cout<<(*it)->first;
	}
	return 0;
}