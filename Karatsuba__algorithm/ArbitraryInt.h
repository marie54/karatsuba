#ifndef ArbitraryInt_h
#define ArbitraryInt_h

#include <vector>
#include <algorithm>

using namespace std;

typedef char byte;

enum OperandComparation {
    EQUAL = 0,
    LHS_GREATER,
    RHS_GREATER
};

class ArbitraryInt {
private:
    vector<byte> digits;
    bool is_negative = false;
    size_t length = 0;
    
    void parse_string (string numberString);
    
public:
    friend ArbitraryInt sum (const ArbitraryInt &, const ArbitraryInt &);
    friend ArbitraryInt difference (const ArbitraryInt &, const ArbitraryInt &, size_t length, bool negate);
    friend ArbitraryInt difference_proxy (const ArbitraryInt &, const ArbitraryInt &);
    friend OperandComparation compare_operands (const ArbitraryInt &, const ArbitraryInt &);
    friend pair<ArbitraryInt, ArbitraryInt> split_integer_at (const ArbitraryInt &, int);
    friend ArbitraryInt shift (const ArbitraryInt &, int);
    
    ArbitraryInt ();
    ArbitraryInt (int number);
    ArbitraryInt (string numberString);
    ArbitraryInt (vector<byte> & fromDigits, bool negative);
    ArbitraryInt (const ArbitraryInt & other);
    
    string repr () const;
    bool is_neg () const;
    size_t number_length () const;
    int to_integer () const;
    
    void negate ();
    ArbitraryInt abs () const;
};

ArbitraryInt operator+ (const ArbitraryInt &, const ArbitraryInt &);
ArbitraryInt operator- (const ArbitraryInt &, const ArbitraryInt &);

#endif /* ArbitraryInt_h */
