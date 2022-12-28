#include <cstdio>
#include <vector>

using namespace std;

typedef signed char digit;

class Snafu
{
private:
    const static digit InvalidDigit = -100;
    vector<digit> digits;

public:
    Snafu()
    {
        digits.push_back(0);
    }

    Snafu(const char* input)
    {
        int i = 0;
        while(CharToDigit(input[i]) != InvalidDigit) i++;

        i--;
        while(i >= 0)
        {
            digits.push_back(CharToDigit(input[i]));
            i--;
        }

        if (digits.size() == 0)
        {
            digits.push_back(0);
        }
    }

    void Print()
    {
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            printf("%c", DigitToChar(digits[i]));
        }
        printf("\n");
    }

    void Add(const Snafu& s)
    {
        while(digits.size() <= s.digits.size()) digits.push_back(0);

        for(int i = 0; i < s.digits.size(); i++)
        {
            digits[i] += s.digits[i];
        }

        for(int i = 0; i < digits.size() - 1; i++)
        {
            if (digits[i] < -2)
            {
                digits[i+1]--;
                digits[i] += 5;
            }
            else if (digits[i] > 2)
            {
                digits[i+1]++;
                digits[i] -= 5;
            }
        }

        while (digits.back() == 0 && digits.size() > 1) digits.pop_back();
    }

private:
    digit CharToDigit(char c)
    {
        switch(c)
        {
            case '=': return -2;
            case '-': return -1;
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            default: return InvalidDigit;
        }
    }

    char DigitToChar(digit d)
    {
        switch(d)
        {
            case -2: return '=';
            case -1: return '-';
            case 0: return '0';
            case 1: return '1';
            case 2: return '2';
            default: return '#';
        }
    }
};

int main()
{
    Snafu sum;
    char buffer[1024];

    while(NULL != fgets(buffer, 1024, stdin))
    {
        Snafu number(buffer);
        sum.Add(number);
    }

    sum.Print();

    return 0;
}