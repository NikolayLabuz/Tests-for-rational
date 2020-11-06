#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        }
        catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        }
        catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

int GCD(int a, int b) {
    if (b == 0) {
        return a;
    }
    else {
        return GCD(b, a % b);
    }
}
class Rational {
public:
    Rational() {
        numerators = 0;
        denominators = 1;
    }

    Rational(int numerator, int denominator) {
        if (denominator == 0) {
            throw invalid_argument("Invalid argument");
        }
        const int NOD = GCD(numerator, denominator);
        denominator /= NOD;
        numerator /= NOD;
        if (denominator < 0) {
            numerator *= -1;
            denominator *= -1;
        }
        denominators = denominator;
        numerators = numerator;
    }

    int Numerator() const {
        return numerators;
    }

    int Denominator() const {
        return denominators;
    }
private:
    int numerators;
    int denominators;
};
Rational operator + (const Rational& lhs, const Rational& rhs) {
    return{
        lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()
    };
}
Rational operator - (const Rational& lhs, const Rational& rhs) {
    return{
        lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()
    };
}
bool operator == (const Rational& lhs, const Rational& rhs) {
    if (lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator()) {
        return true;
    }
    else {
        return false;
    }

}
ostream& operator << (ostream& stream, const Rational& Rationals) {
    return stream << Rationals.Numerator() << "/" << Rationals.Denominator();
}
istream& operator >> (istream& stream, Rational& Rationals) {
    int n, d;
    char c;
    stream >> n >> c >> d;
    if (stream && c == '/') {
        Rationals = Rational(n, d);
    }
    return stream;
}
Rational operator * (const Rational& lhs, const Rational& rhs) {
    return{
        lhs.Numerator() * rhs.Numerator(),
        lhs.Denominator() * rhs.Denominator()
    };
}
Rational operator / (const Rational& lhs, const Rational& rhs) {
    if (rhs.Numerator() == 0) {
        throw domain_error("Division by zero");
    }
    return{
        lhs.Numerator() * rhs.Denominator(),
        rhs.Numerator() * lhs.Denominator()
    };
}
bool operator < (const Rational& lhs, const Rational& rhs) {
    if ((lhs - rhs).Numerator() < 0) {
        return true;
    }
    else {
        return false;
    }
};
void TestPredefined() {
    AssertEqual(Rational(1, 1).Numerator(), 1, "Canonical form of 1/1 is 1/1");
    AssertEqual(Rational(1, 1).Denominator(), 1, "Canonical form of 1/1 is 1/1");
    AssertEqual(Rational(3, 5).Numerator(), 3, "Canonical form of 3/5 is 3/5");
    AssertEqual(Rational(3, 5).Denominator(), 5, "Canonical form of 3/5 is 3/5");
    AssertEqual(Rational(2147483647, 2147483647).Numerator(), 1, "Canonical form of 2147483647/2147483647 is 1/1");
    AssertEqual(Rational(2147483647, 2147483647).Denominator(), 1, "Canonical form of 2147483647/2147483647 is 1/1");
    AssertEqual(Rational(-5, 4).Numerator(), -5, "Canonical form of -5/4 is -5/4");
    AssertEqual(Rational(-5, 4).Denominator(), 4, "Canonical form of -5/4 is -5/4");
    AssertEqual(Rational(5, -4).Numerator(), -5, "Canonical form of 5/-4 is -5/4");
    AssertEqual(Rational(5, -4).Denominator(), 4, "Canonical form of 5/-4 is -5/4");
    AssertEqual(Rational(-6, -2).Numerator(), 3, "Canonical form of -6/-2 is 3/1");
    AssertEqual(Rational(-6, -2).Denominator(), 1, "Canonical form of -6/-2 is 3/1");
    AssertEqual(Rational(21, 56).Numerator(), 3, "Canonical form of 21/56 is 3/8");
    AssertEqual(Rational(21, 56).Denominator(), 8, "Canonical form of 21/56 is 3/8");
    AssertEqual(Rational(0, 100).Numerator(), 0, "Canonical form of 0/100 is 0/1");
    AssertEqual(Rational(0, 100).Denominator(), 1, "Canonical form of 0/100 is 0/1");
    AssertEqual(Rational().Numerator(), 0, "Canonical form of default constructed is 0/1");
    AssertEqual(Rational().Denominator(), 1, "Canonical form of default constructed is 0/1");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestPredefined, "TestPredefined");
    return 0;
}
