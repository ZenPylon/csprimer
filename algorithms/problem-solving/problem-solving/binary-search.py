
def find_entry(input: list[int], value: int) -> int | None:
    """
    Uses binary search to an item in the ordered list `input` that is equal to `value`, and returns its index in the list.
    If value is not found, None is returned
    """
    if len(input) == 0:
        return None
    
    left = 0
    right = len(input)
    matching_index = None

    while left < right:
        item_index = (right + left) // 2

        if input[item_index] == value:
            matching_index = item_index
            break
        elif input[item_index] > value:
            right = item_index
        else:
            left = item_index + 1
        
    return matching_index

# Odd vs. even size
assert find_entry([], 0) == None
assert find_entry([1, 2, 3, 4, 5], 0) == None
assert find_entry([-1, 2, 3, 4, 5, 6], -1) == 0
assert find_entry([-1, 2, 3, 4, 5, 6, 7], 6) == 5
assert find_entry([-10, -10, -10, 10, 100, 101, 102, 103, 104, 104, 104, 104, 104], 102) == 6