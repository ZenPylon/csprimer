use std::ops::{Add, Div, Mul, Sub};

// Just experimenting with trait bounds to limit inputs to unsigned integers
trait FromUnsigned {
    fn from_unsigned(val: u64) -> Self;
}
impl FromUnsigned for u8 {
    fn from_unsigned(val: u64) -> Self {
        val as u8
    }
}
impl FromUnsigned for u16 {
    fn from_unsigned(val: u64) -> Self {
        val as u16
    }
}
impl FromUnsigned for u32 {
    fn from_unsigned(val: u64) -> Self {
        val as u32
    }
}
impl FromUnsigned for u64 {
    fn from_unsigned(val: u64) -> Self {
        val as u64
    }
}
impl FromUnsigned for usize {
    fn from_unsigned(val: u64) -> Self {
        val as usize
    }
}

trait UnsignedInt:
    Copy
    + Add<Output = Self>
    + Mul<Output = Self>
    + Div<Output = Self>
    + Sub<Output = Self>
    + FromUnsigned
{
}
impl UnsignedInt for u8 {}
impl UnsignedInt for u16 {}
impl UnsignedInt for u32 {}
impl UnsignedInt for u64 {}

// Summation formula
fn sum_sequence<T: UnsignedInt>(n: T) -> T {
    n * (n + T::from_unsigned(1)) / T::from_unsigned(2)
}

fn fizzbuzz_sum<T: UnsignedInt>(n: T) -> T {
    let three: T = T::from_unsigned(3);
    let five: T = T::from_unsigned(5);
    let fifteen: T = T::from_unsigned(15);

    // Subtract the "overlapping" numbers, where 3 and 5 are both divisors
    return three * sum_sequence(n / three) + five * sum_sequence(n / five)
        - fifteen * sum_sequence(n / fifteen);
}

fn main() {
    // Structure:
    // (input, expected_output)
    let test_cases = [(1u64, 0u64), (3u64, 3u64), (15u64, 60u64), (500u64, 58418u64)];
    for case in test_cases {
        assert_eq!(fizzbuzz_sum(case.0), case.1);
    }
}
