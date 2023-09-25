#include "HashTableQuad.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

bool isPrime(int n) {
    if (n < 2) { // base case
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) { // check divisibility up to sqrt(n)
        if (n % i == 0) {
            return false; //If i divides n, n is not prime, so return false
        }
    }
    return true;
}

int nextPrime(int n) { //finds the next prime number greater than or equal to n.
    if (n < 2) { // ensure n is at least 2
        return 2; //If n is less than 2, return 2 as the smallest prime is 2.
    }
    n += (n % 2 == 0); // If n is even, increment it to make sure n is odd
    while (!isPrime(n)) { // find next prime greater than n
        n += 2; //Increment n by 2 until it is prime.
    }
    return n;
}

HashTableQuad::HashTableQuad(int maxNum, double load) {
    int capacity = nextPrime(maxNum / load); // compute the required capacity
    table.resize(capacity, -1); // create a vector of specified capacity and fill with -1
    size = capacity; //Set the size of the hash table to the computed capacity
    maxkey = static_cast<int>(size * load); // compute the maximum number of keys
    maxload = load; //Set the maximum load factor of the hash table to the input load factor.
    keys = 0; //Set the number of keys currently stored in the hash table to 0.
}


void HashTableQuad::insert(int n) {
    if (!isIn(n)) { // if n is not already in the table
        int i = n % size; // compute the initial index
        int count = 1; // initialize the probe count
        while (table[i] != -1) { // quadratic probing until an empty slot is found
            i = (n % size + count * count) % size;
            count++;
            if (count > size) { // if the table is full, return
                return;
            }
        }
        table[i] = n; // insert n into the empty slot
        keys++; // increment the number of keys in the table
        if (keys > maxkey) { // if the load factor is exceeded, rehash
            rehash();
        }
    }
}

void HashTableQuad::rehash() {
    int new_size = size * 2; // Double the size of the table
    int new_maxkey = new_size * maxload; // Compute the maximum number of keys for the new size

    std::vector<int> old_table = table; // Create a copy of the old table

    // Find the next prime number greater than or equal to the new size
    int prime_candidate = new_size + (new_size % 2 == 0);
    while (true) {
        bool is_prime = true;
        for (int i = 2; i <= sqrt(prime_candidate); i++) {
            if (prime_candidate % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            break;
        }
        prime_candidate += 2;
    }
    int new_prime_size = prime_candidate;

    // Create a new table with the new prime size and fill with -1
    std::vector<int> new_table(new_prime_size, -1);

    // Re-insert all the keys from the old table into the new table
    int new_keys = 0;
    for (int i = 0; i < old_table.size(); i++) {
        if (old_table[i] != -1) {
            int j = old_table[i] % new_prime_size;
            int probe_count = 1;
            while (new_table[j] != -1) {
                j = (old_table[i] % new_prime_size + probe_count * probe_count) % new_prime_size;
                probe_count++;
                if (probe_count > new_prime_size) {
                    break;
                }
            }
            if (new_table[j] == -1) {
                new_table[j] = old_table[i];
                new_keys++;
                if (new_keys >= new_maxkey) {
                    break;
                }
            }
        }
    }

    // Update the hash table properties
    size = new_prime_size;
    maxkey = new_maxkey;
    keys = new_keys;
    table = new_table;
}


bool HashTableQuad::isIn(int n) 
{
    const int max_iterations = size; // Define a constant variable for the maximum number of iterations
    int i = n % size; // Compute the initial index based on the hash code and the table size
    int step = 0; // Initialize the step size for quadratic probing
    do { // Start a do-while loop that continues until the element is found or the maximum iterations are reached
        if (table[i] == n) { // Check if the element at the current index matches the target value
            return true; // Return true if it does
        }
        i = (i + step * step) % size; // Compute the next index using quadratic probing, wrapping around the table if necessary
        step++; // Increment the step size for the next iteration
    } while (table[i] != NULL && step < max_iterations); // Continue the loop if the current index is not empty and the maximum iterations are not exceeded
    return false; // Return false if the element is not found
}


void HashTableQuad::printKeys() 
{
    int i = 0; 
    while (i < size) { //while loop that continues until the end of the table is reached
        if (table[i] != NULL) { //Check if the current index contains a non-null value
            cout << table[i] << ", "; //Print the value followed by a comma and a space
        }
        i++; 
    }
}


void HashTableQuad::printKeysAndIndexes() 
{
    int i = 0; 
    bool isFirstPrinted = false; //flag to keep track of whether the first non-null element has been printed
    while (i < size) { 
        if (table[i] != NULL) { // Check if the current index contains a non-null value
            if (isFirstPrinted) { // Check if this is not the first non-null element to be printed
                cout << ", "; 
            }
            else {
                isFirstPrinted = true; // Set the flag to true for the first non-null element
            }
            cout << i << ", " << table[i]; // Print the index and value of the non-null element
        }
        i++; 
    }
}

int HashTableQuad::getNumKeys() { 
    int i = 0; 
    int keycount = 0; 
    while (i < size) { //while loop that continues until the end of the table is reached
        if (table[i] != NULL) { //check if the current index contains a non-null value
            keycount++; //increment the key count for each non-null value found
        }
        i++; 
    }
    return keycount; 
}

int HashTableQuad::getTableSize() { 
    return size;
}

double HashTableQuad::getMaxLoadFactor() { 
    return maxload;
}

int HashTableQuad::insertCount(int n) { 
    int probe = 0; 
    int i = n % size; // Calculate the initial hash index
    bool inserted = false; //flag for whether the key has been inserted
    while (!inserted && probe < size) { //continues until the key is inserted or the table is full
        probe++; 
        if (table[i] == NULL) { // Check if the current index is empty
            table[i] = n; // Insert the key at the current index
            keys++; 
            inserted = true; // Set the flag to indicate that the key has been inserted
        }
        else if (table[i] == n) { // Check if the current index already contains the key
            inserted = true; // Set the flag to indicate that the key has been inserted
        }
        i = (i + probe * probe) % size; // Calculate the next hash index using quadratic probing
    }
    if (inserted && maxkey < keys) { // Check if a key was inserted and if the load factor has exceeded the maximum threshold
        rehash(); // Rehash the table to reduce the load factor
    }
    return probe; 
}


std::vector<double> HashTableQuad::simProbeSuccess()
{
    std::vector<double> result(9); // stores the resulting average probe counts
    int max_size = 100000;
    int unique_values[100000]; // stores the unique random values to be inserted
    double insert_count_sum = 0.0; // accumulates the total number of probes across all trials
    double trial_count = 100.0; // number of trials to average over
    double trial_size = 100000.0; // number of values to insert in each trial

    // Generate an array of 100,000 unique random values
    for (int i = 0; i < max_size; i++) {
        int value;
        bool is_unique = false;
        while (!is_unique) {
            is_unique = true;
            value = rand() * rand();
            for (int j = 0; j < i; j++) {
                if (value == unique_values[j]) {
                    is_unique = false;
                    break;
                }
            }
        }
        unique_values[i] = value;
    }

    // Insert the values into a hash table of different load factors and average the probe counts
    for (double load_factor = 0.1; load_factor < 0.99; load_factor += 0.1) {
        insert_count_sum = 0.0;
        for (int trial = 0; trial < trial_count; trial++) {
            HashTableQuad hashtable(max_size, load_factor);
            for (int i = 0; i < trial_size; i++) {
                insert_count_sum += hashtable.insertCount(unique_values[i]);
            }
        }
        double avg_probe_count = insert_count_sum / (trial_count * trial_size);
        result[static_cast<int>((load_factor - 0.1) / 0.1)] = avg_probe_count;
    }

    return result;
}