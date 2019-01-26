#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[])
{
    vector<int> vec;
    vec.push_back(2);

    bool x = (vec[0] == vec[1]);

    for (int i=0; i < vec.size(); i++) cout << vec[i] << endl;
    cout << x << endl;
    return 0;
}
