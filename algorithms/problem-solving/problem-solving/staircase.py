
def get_num_step_possibilities(n: int):
    if n == 0:
        return 0
    
    return get_num_step_possibilities_recur(n, 0, {})

def get_num_step_possibilities_recur(n: int, curr_sum: int, possibilities_cache: dict[int, int]):
    if curr_sum > n:
        return 0
    if curr_sum in possibilities_cache:
        print(f"Cache hit for curr_sum={curr_sum}")
        return possibilities_cache[curr_sum]
    if curr_sum == n:
        return 1

    one_step = get_num_step_possibilities_recur(n, curr_sum + 1, possibilities_cache)
    two_step = get_num_step_possibilities_recur(n, curr_sum + 2, possibilities_cache)
    three_step = get_num_step_possibilities_recur(n, curr_sum + 3,  possibilities_cache)

    downstream_possibilities =  one_step + two_step + three_step
    possibilities_cache[curr_sum] = downstream_possibilities
    return downstream_possibilities

print(f"n = 0 == {get_num_step_possibilities(0)}")
print(f"n = 1 == {get_num_step_possibilities(1)}")
print(f"n = 2 == {get_num_step_possibilities(2)}")
print(f"n = 3 == {get_num_step_possibilities(3)}")
print(f"n = 4 == {get_num_step_possibilities(4)}")
print(f"n = 5 == {get_num_step_possibilities(5)}")
print(f"n = 6 == {get_num_step_possibilities(6)}")

