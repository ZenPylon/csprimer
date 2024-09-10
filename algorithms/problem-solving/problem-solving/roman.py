thousands_map = {
    0: "",
    1: "M",
    2: "MM",
    3: "MMM",
}

hundreds_map = {
    0: "",
    1: "C",
    2: "CC",
    3: "CCC",
    4: "CD",
    5: "D",
    6: "DC",
    7: "DCC",
    8: "DCCC",
    9: "CM",
}

tens_map = {
    0: "",
    1: "X",
    2: "XX",
    3: "XXX",
    4: "XL",
    5: "L",
    6: "LX",
    7: "LXX",
    8: "LXXX",
    9: "XC",
}

ones_map = {
    0: "",
    1: "I",
    2: "II",
    3: "III",
    4: "IV",
    5: "V",
    6: "VI",
    7: "VII",
    8: "VIII",
    9: "IX",
}

def arabic_to_roman(n: int):
    if n > 3000:
        print("Adiurasti ineffabilem numerum")
        return "無"

    ones = n % 10
    tens = (n // 10) % 10 
    hundreds = (n // 100) % 10
    thousands = (n // 1000) % 10

    return "".join([thousands_map[thousands], hundreds_map[hundreds], tens_map[tens], ones_map[ones]])

assert arabic_to_roman(1) == "I"
assert arabic_to_roman(12) == "XII"
assert arabic_to_roman(44) == "XLIV"
assert arabic_to_roman(98) == "XCVIII"
assert arabic_to_roman(101) == "CI"
assert arabic_to_roman(404) == "CDIV"
assert arabic_to_roman(666) == "DCLXVI"
assert arabic_to_roman(2048) == "MMXLVIII"

