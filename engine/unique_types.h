/**
 * @file unique_types.h
 * @brief Defines essential types and utility functions for the project.
 */

#ifndef UNIQUE_TYPES_H
#define UNIQUE_TYPES_H

#include <vector>
#include <functional>
#include <string>
#include <thread>

 /**
  * @struct KeyValuePair
  * @brief Defines a key-value pair, where both key and value are stored as strings.
  */
struct KeyValuePair {
    std::string key; /**< The key string */
    std::string value; /**< The value string */

    /**
     * @brief Constructs a new KeyValuePair object with given key and value.
     * @param key The key string
     * @param value The value string
     */
    KeyValuePair(std::string key, std::string value) {
        this->key = key;
        this->value = value;
    }
};

/**
 * @struct Delegate
 * @brief Defines a delegate, which stores a list of lambda functions for later execution.
 */
struct Delegate {
private:
    std::vector<std::function<void()>> executionList; /**< The list of lambda functions */

public:
    /**
     * @brief Adds a lambda function to the execution list.
     * @param func The lambda function to be added
     */
    void AddLambda(std::function<void()> func);

    /**
     * @brief Executes all the lambda functions in the execution list.
     */
    void Execute();
};

/**
 * @struct ThreadContext
 * @brief Defines a thread context, which can be used to spawn, wait, sleep or kill a thread.
 */
struct ThreadContext {
private:
    std::thread worker; /**< The thread instance */

public:
    /**
     * @brief Spawns a new thread with the given function.
     * @param func The function to be executed by the thread
     */
    void Spawn(std::function<void()> func);

    /**
     * @brief Waits until the current frame is finished.
     */
    void WaitUntil();

    /**
     * @brief Sleeps the current thread for the given amount of seconds.
     * @param amount The amount of seconds to sleep
     */
    void Sleep(int amount);

    /**
     * @brief Kills the current thread.
     */
    void Kill();
};

/**
 * @class Essentials
 * @brief Defines a collection of essential utility functions for the project.
 */
class Essentials {
public:
    /**
     * @brief Splits a given string into a vector of substrings using a delimiter.
     * @param string The string to be split
     * @param split The delimiter character to split on
     * @return A vector of substrings
     */
    static std::vector<std::string> Split(std::string string, char split);
};

#endif // UNIQUE_TYPES_H