#include "../../core.h"

/*
 * Topic: Data Structures - Binary Trie (Array-based)
 * Description: A Binary Trie implemented using dynamically allocated vectors.
 *              Supports insert, erase, max_xor, and min_xor.
 */

struct BinaryTrie_array {
    int max_bit;
    vector<array<int, 2>> child_node;
    vector<int> frequency;

    
    BinaryTrie_array(int max_bit = 30) : max_bit(max_bit), child_node(1, {0, 0}), frequency(1, 0) {}

    
    void insert(int number) {
        int current_node = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (!child_node[current_node][bit]) {
                child_node[current_node][bit] = child_node.size();
                child_node.push_back({0, 0});
                frequency.push_back(0);
            }
            current_node = child_node[current_node][bit];
            frequency[current_node]++;
        }
    }

    
    void erase(int number) {
        int current_node = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (!child_node[current_node][bit]) return;
            current_node = child_node[current_node][bit];
            frequency[current_node]--;
        }
    }

    
    int max_xor(int number) {
        int current_node = 0, answer = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (child_node[current_node][bit ^ 1] && frequency[child_node[current_node][bit ^ 1]]) {
                answer |= (1LL << i);
                current_node = child_node[current_node][bit ^ 1];
            } else {
                current_node = child_node[current_node][bit];
            }
        }
        return answer;
    }

    
    int min_xor(int number) {
        int current_node = 0, answer = 0;
        for (int i = max_bit; i >= 0; i--) {
            int bit = (number >> i) & 1;
            if (child_node[current_node][bit] && frequency[child_node[current_node][bit]]) {
                current_node = child_node[current_node][bit];
            } else {
                answer |= (1LL << i);
                current_node = child_node[current_node][bit ^ 1];
            }
        }
        return answer;
    }
};
