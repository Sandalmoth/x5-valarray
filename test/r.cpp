#include <iostream>
#include <cassert>

#include "../x5-valarray.h"


using namespace std;


int main() {
  string s = "yo/i/herd/u/liek/mudkipz";
  for (auto ss: X5::split(s, "/"))
    cout << ss << ' ';
  cout << endl;
  string ss;
  while ((ss = X5::partial_split(s, "/")) != string())
    cout << ss << ' ' << s << endl;
}
