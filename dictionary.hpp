#ifndef DICTIONARY_HPP__
#define DICTIONARY_HPP__

#include <string>
#include <stdexcept>  // For exceptions

using std::string;

template <typename T>
class Dictionary {
    static const long FREE = -1;
    static const size_t DEFAULT_SIZE = 32;
    size_t size;
    T *table;
    long *hashes;
    string *keys;

 public:
    Dictionary() {
        size = DEFAULT_SIZE;
        table = new T[size];
        hashes = new long[size];
        keys = new string[size];
        for (size_t i = 0; i < size; i++) {
            hashes[i] = FREE;
        }
    }

    ~Dictionary() {
        delete[] keys;
        delete[] table;
        delete[] hashes;
    }

    T& operator[](string key) {
        long hash = Dictionary::hash(key);
        size_t pos = static_cast<size_t>(hash) % size;

        // search a free pos
        for (size_t full = 0; hashes[pos] != FREE; pos = (pos+1)%size, ++full) {
            // already in the table
            if ((hashes[pos] == hash) && (keys[pos] == key))
                break;
            if (full == size)
                throw std::overflow_error("The dictionary run out of space");
        }
        return table[pos];
    }

    T& operator[](string key) const {
        long hash = Dictionary::hash(key);
        size_t pos = static_cast<size_t>(hash) % size;

        for (size_t full = 0;
             (hashes[pos] != FREE) && (full != size);
             pos = (pos+1)%size, ++full) {
            if ((hashes[pos] == hash) && (keys[pos] == key))
                return table[pos];
        }
        throw std::out_of_range("No such element in the dictionary");
    }

    static long hash(string str) {
        long hash = 5381;

        for (size_t i = 0; i < str.length(); i++)
            hash = ((hash << 5) + hash) + str[i];  // hash * 33 + str[i]

        return hash;
    }
};

#endif  // DICTIONARY_HPP_

