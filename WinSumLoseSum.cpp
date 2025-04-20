#include "WinSumLoseSum.h"
#include "error.h"
#include "vector.h"
using namespace std;

bool containsNegatives(const Vector<int>& elems) {
    for (int elem : elems) {
        if (elem < 0) return true;
    }
    return false;
}

Optional<Set<int>> findSubset(const Vector<int>& elems, int target, Vector<int>& current, int index, bool mustUseNegative) {
    if (target == 0) {
        // If we must use a negative, check if any negatives were used
        if (mustUseNegative) {
            for (int elem : current) {
                if (elem < 0) {
                    Set<int> result;
                    for (int e : current) result.add(e);
                    return result;
                }
            }
            return Nothing;
        }
        Set<int> result;
        for (int elem : current) result.add(elem);
        return result;
    }
    if (index == elems.size()) return Nothing;

    int elem = elems[index];

    // Option 1: Exclude current element
    Optional<Set<int>> result = findSubset(elems, target, current, index + 1, mustUseNegative);
    if (result != Nothing) return result;

    // Option 2: Include current element
    if (elem <= target || (elem < 0 && target > 0)) {
        current.add(elem);
        result = findSubset(elems, target - elem, current, index + 1, mustUseNegative);
        current.remove(current.size() - 1);
        if (result != Nothing) return result;
    }

    return Nothing;
}

Optional<Set<int>> makeTarget(const Vector<int>& elems, int target) {
    Vector<int> current;
    bool hasNegatives = containsNegatives(elems);

    if (hasNegatives) {
        // First try to find solution that uses at least one negative
        Optional<Set<int>> result = findSubset(elems, target, current, 0, true);
        if (result != Nothing) return result;

        // If no solution with negatives exists, return Nothing
        return Nothing;
    }

    // No negatives in input, proceed normally
    return findSubset(elems, target, current, 0, false);
}

/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"

STUDENT_TEST("Test with duplicate elements") {
    EXPECT_EQUAL(makeTarget({2, 2, 3}, 4), {2, 2});  // Now works with duplicates!
    EXPECT_EQUAL(makeTarget({2, 2, 3}, 5), {2, 3});
}

STUDENT_TEST("Test with negative numbers") {
    EXPECT_EQUAL(makeTarget({-1, 2, 3}, 1), {2, -1});
    EXPECT_EQUAL(makeTarget({-1, 2, 3}, 5), Nothing);
}

/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Works for an empty set of numbers.") {
    /* Can make 0, but not others. */
    EXPECT_EQUAL(makeTarget({ },  0), {});
    EXPECT_EQUAL(makeTarget({ },  1), Nothing);
    EXPECT_EQUAL(makeTarget({ }, -1), Nothing);
}

PROVIDED_TEST("Works for a one-element (singleton) set.") {
    /* Can make 0 and 137, but not others. */
    EXPECT_EQUAL(makeTarget({ 137 }, 0),   {});
    EXPECT_EQUAL(makeTarget({ 137 }, 137), {137});
    EXPECT_EQUAL(makeTarget({ 137 }, 1),   Nothing);
    EXPECT_EQUAL(makeTarget({ 137 }, -1),  Nothing);
}

PROVIDED_TEST("Works for a two-element (doubleton) set.") {
    EXPECT_EQUAL(makeTarget({ 1, 2 }, -1), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 0), {});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 1), {1});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 2), {2});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 3), {1, 2});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 4), Nothing);
}

PROVIDED_TEST("Works for a three-element set.") {

    /* Can make 0, 1, 3, 4, 5, 7, and 8, but not others. */
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 }, -1), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  0), {});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  1), {1});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  2), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  3), {3});
    EXPECT_NOT_EQUAL(makeTarget({ 1, 3, 4 }, 4), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  5), {1, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  6), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  7), {3, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  8), {1, 3, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  9), Nothing);
}
