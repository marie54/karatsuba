#include "ArbitraryInt.h"
#include <string>

using namespace std;

ArbitraryInt::ArbitraryInt () {}

ArbitraryInt::ArbitraryInt (int number) {
    if (number < 0) {
        is_negative = true;
        number = ~number + 1;
    }
    
    while (number) {
        digits.push_back(number % 10);
        number /= 10;
        length++;
    }
}

ArbitraryInt::ArbitraryInt (string number_string) {
    parse_string(number_string);
}

ArbitraryInt::ArbitraryInt (vector<byte> & from_digits, bool negative) {
    digits = from_digits;
    is_negative = negative;
    length = from_digits.size();
}

ArbitraryInt::ArbitraryInt (const ArbitraryInt & other) {
    is_negative = other.is_negative;
    length = other.length;
    
    for (auto digit : other.digits)
        digits.push_back(digit);
}

void ArbitraryInt::parse_string (string number_string) {
    if (number_string[0] == '-')
        is_negative = true;
    
    for (int i = 0 + is_negative; i < number_string.length(); i++) {
        digits.push_back(number_string[i] - '0');
        length++;
    }
    
    reverse(digits.begin(), digits.end());
}

string ArbitraryInt::repr () const {
    string temp = "";
    
    if (is_negative)
        temp += '-';
    
    for (int idx = (int) length - 1; idx >= 0; idx--) {
        temp += digits[idx] + '0';
    }
    
    return temp;
}

bool ArbitraryInt::is_neg () const {
    return is_negative;
}

size_t ArbitraryInt::number_length () const {
    return length;
}

int ArbitraryInt::to_integer () const {
    return atoi(repr().c_str());
}

void ArbitraryInt::negate () {
    is_negative = !is_negative;
}

ArbitraryInt ArbitraryInt::abs () const {
    ArbitraryInt temp(*this);
    temp.is_negative = false;
    
    return temp;
}


ArbitraryInt sum (const ArbitraryInt & lhs, const ArbitraryInt & rhs) {
    size_t new_length;
    size_t length_lhs = lhs.length,
    length_rhs = rhs.length;
    
    if (length_lhs > length_rhs)
        new_length = length_lhs + 1;
    else
        new_length = length_rhs + 1;
    
    vector<byte> temp (new_length);
    
    byte digit_lhs, digit_rhs, carry = 0;
    for (int idx = 0; idx < new_length; idx++) {
        digit_lhs = (idx < length_lhs) ? lhs.digits[idx] : 0;
        digit_rhs = (idx < length_rhs) ? rhs.digits[idx] : 0;
        
        temp[idx] = digit_rhs + digit_lhs + carry;
        carry = temp[idx] / 10;
        temp[idx] %= 10;
    }
    
    if (temp.back() == 0)
        temp.resize(new_length - 1);
    
    return ArbitraryInt(temp, false);
}

OperandComparation compare_operands (const ArbitraryInt & lhs, const ArbitraryInt & rhs) {
    if (lhs.length > rhs.length)
        return OperandComparation::LHS_GREATER;
    
    if (rhs.length > lhs.length)
        return OperandComparation::RHS_GREATER;
    
    OperandComparation result = EQUAL;
    size_t length = lhs.length;
    
    for (int idx = (int) length - 1; idx >= 0; idx--) {
        if (lhs.digits[idx] > rhs.digits[idx]) {
            result = LHS_GREATER;
            break;
        } else if (lhs.digits[idx] != rhs.digits[idx]) {
            result = RHS_GREATER;
            break;
        }
    }
    
    return result;
}

ArbitraryInt difference_proxy (const ArbitraryInt & lhs, const ArbitraryInt & rhs) {
    OperandComparation comparation_result = compare_operands(lhs, rhs);
    
    size_t new_length;
    
    if (lhs.length > rhs.length)
        new_length = lhs.length;
    else
        new_length = rhs.length;
    
    switch (comparation_result) {
        case EQUAL:
            return ArbitraryInt("0");
            break;
        case LHS_GREATER:
            return difference(lhs, rhs, new_length, false);
            break;
        case RHS_GREATER:
            return difference(rhs, lhs, new_length, true);
            break;
    }
}

ArbitraryInt difference (const ArbitraryInt & lhs, const ArbitraryInt & rhs, size_t length, bool negate) {
    vector<byte> temp(length);
    vector<byte> lhs_temp(lhs.digits);
    
    for (int idx = 0; idx < length; idx++) {
        if (idx < (length - 1)) {
            lhs_temp[idx + 1]--;
            temp[idx] += 10 + lhs_temp[idx];
        } else {
            temp[idx] += lhs_temp[idx];
        }
        
        byte digit_rhs = (idx < rhs.length) ? rhs.digits[idx] : 0;
        
        temp[idx] -= digit_rhs;
        
        if (temp[idx] / 10 > 0) {
            temp[idx + 1]++;
            temp[idx] %= 10;
        }
    }
    
    int zeroes = 0;
    for (int i = (int) temp.size() - 1; i >= 0; i--) {
        if (temp[i] != 0)
            break;
        else
            zeroes++;
    }
    
    temp.resize(length - zeroes);
    
    return ArbitraryInt(temp, negate);
}

ArbitraryInt operator+ (const ArbitraryInt & lhs, const ArbitraryInt & rhs) {
    if (!lhs.is_neg() && rhs.is_neg())
        return difference_proxy(lhs, rhs.abs());
    
    if (lhs.is_neg() && !rhs.is_neg()) {
        auto temp = difference_proxy(rhs, lhs.abs());
        return temp;
    }
    
    auto positive_sum = sum(lhs.abs(), rhs.abs());
    
    if (lhs.is_neg() && rhs.is_neg()) positive_sum.negate();
    return positive_sum;
}


ArbitraryInt operator- (const ArbitraryInt & lhs, const ArbitraryInt & rhs) {
    if (!lhs.is_neg() && rhs.is_neg())
        return sum(lhs, rhs.abs());
    
    if (lhs.is_neg() && !rhs.is_neg()) {
        auto temp = sum(rhs.abs(), lhs);
        temp.negate();
        return temp;
    }
    
    if (lhs.is_neg() && rhs.is_neg())
        return difference_proxy(rhs.abs(), lhs.abs());
    
    return difference_proxy(lhs, rhs);
}

pair<ArbitraryInt, ArbitraryInt> split_integer_at (const ArbitraryInt & number, int position) {
    vector<byte> high_digits, low_digits;
    
    for (int i = 0; i < number.length; i++) {
        if (i < position)
            low_digits.push_back(number.digits[i]);
        else
            high_digits.push_back(number.digits[i]);
    }
    
    return make_pair(ArbitraryInt(high_digits, number.is_neg()),
                     ArbitraryInt(low_digits, number.is_neg()));
}

ArbitraryInt shift (const ArbitraryInt & arg, int amount) {
    vector<byte> temp(arg.length + amount);
    
    copy(arg.digits.begin(), arg.digits.end(), temp.begin() + amount);
    
    return ArbitraryInt(temp, arg.is_neg());
}

