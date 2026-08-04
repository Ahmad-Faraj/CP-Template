#include "../../core.h"

/*
 * Topic: Data Structures - Monotonic Stack
 * Description: Finds the previous smaller element for each element in an array using a stack.
 */

void prev_smaller(vector<int> &arr) {
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
    prev_smaller(arr);
    return 0;
}