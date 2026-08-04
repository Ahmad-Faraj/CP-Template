#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void prevSmaller(vector<int> &arr) {
    stack<int> s;

    for (int i = 0; i < arr.size(); i++) {
        while (!s.empty() && s.top() >= arr[i]) s.pop();

        if (s.empty())
            cout << "-1 ";
        else
            cout << s.top() << " ";

        s.push(arr[i]);
    }
}

int main() {
    vector<int> arr = {1, 5, 0, 3, 4, 5};
    prevSmaller(arr);
    return 0;
}