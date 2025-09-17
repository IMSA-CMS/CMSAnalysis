#ifndef GET_ALL_COMBINATIONS_HH
#define GET_ALL_COMBINATIONS_HH

#include <vector>

// Templated function to generate all combinations of 'k' elements from 'particles'.
// The result is stored in 'allCombinations' as vectors of T.
template <typename T>
void getAllCombinations(
    int offset,                          // Current index to start selection from
    int k,                               // Number of elements left to pick
    std::vector<T>& combination,         // Temporary vector holding current combination
    const std::vector<T>& particles,     // Original list of elements to choose from
    std::vector<std::vector<T>>& allCombinations // Output: all combinations found
)
{
    // Base case: if k == 0, we've selected a full combination
    if (k == 0)
    {
        allCombinations.push_back(combination);  // Store the current combination
        return;
    }

    // Iterate over the remaining elements, starting at 'offset'
    // Ensure there's enough room to pick 'k' more elements
    for (int i = offset; i <= static_cast<int>(particles.size()) - k; ++i)
    {
        combination.push_back(particles[i]);      // Include particles[i] in the current combo
        getAllCombinations(i + 1, k - 1, combination, particles, allCombinations); // Recurse
        combination.pop_back();                   // Backtrack: remove last element to try next
    }
}

#endif // GET_ALL_COMBINATIONS_HH 
