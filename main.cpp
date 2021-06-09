#include <bits/stdc++.h>

using namespace std;

inline int ctoi(char c)
{
    return c - '0';
}

inline char itoc(int i)
{
    return '0' + i;
}

void equalize(string &a, string &b)
{
    while (a.length() < b.length())
    {
        a = '0' + a;
    }
    while (b.length() < a.length())
    {
        b = '0' + b;
    }
}

struct bigNum
{
    char sign;
    string num;

    inline bigNum opp()
    {
        return {itoc('1' - sign), num};
    }

    bool operator>(bigNum other)
    {
        if (sign > other.sign)
        {
            return true;
        }
        if (sign < other.sign)
        {
            return false;
        }
        string l = num, r = other.num;
        equalize(l, r);
        if (sign == '0')
        {
            for (int i = 0; i < l.length(); ++i)
            {
                if (l[i] < r[i])
                {
                    return true;
                }
                else if (l[i] > r[i])
                {
                    return false;
                }
            }
        }
        if (sign == '1')
        {
            for (int i = 0; i < l.length(); ++i)
            {
                if (l[i] > r[i])
                {
                    return true;
                }
                else if (l[i] < r[i])
                {
                    return false;
                }
            }
        }

        cout << "equal";
        return false;
    }

    bigNum operator+(bigNum other)
    {
        bigNum res;
        string l = num, r = other.num;
        equalize(l, r);
        if (sign == other.sign)
        {
            res.sign = sign;
            int rem = 0;
            for (int i = l.length() - 1; i >= 0; --i)
            {
                int tmp = ctoi(l[i]) + ctoi(r[i]) + rem;
                if (tmp < 10)
                {
                    rem = 0;
                }
                else
                {
                    rem = 1;
                    tmp -= 10;
                }
                res.num = itoc(tmp) + res.num;
            }
            if (rem == 1)
            {
                res.num = '1' + res.num;
            }
        }
        else
        {
            if (sign == '0')
            {
                if (l.compare(r) > 0)
                {
                    res.sign = '0';
                }
                else
                {
                    res.sign = '1';
                    swap(l, r);
                }
            }
            else
            {
                if (l.compare(r) < 0)
                {
                    res.sign = '0';
                    swap(l, r);
                }
                else
                {
                    res.sign = '1';
                }
            }
            int rem = 0;
            for (int i = l.length() - 1; i >= 0; --i)
            {
                int tmp = ctoi(l[i]) - ctoi(r[i]) - rem;
                if (tmp >= 0)
                {
                    rem = 0;
                }
                else
                {
                    rem = 1;
                    tmp += 10;
                }
                res.num = itoc(tmp) + res.num;
            }
        }

        if (res.num == "")
        {
            res.num = "0";
        }
        return res;
    }

    bigNum operator-(bigNum other)
    {
        bigNum tmp = {this->sign, this->num};
        return tmp + other.opp();
    }

    bigNum operator*(bigNum other)
    {
        bigNum res = {'1', "0"};
        string l = num, r = other.num;
        for (int i = 0; i < l.length(); ++i)
        {
            string submul = "";
            int rem = 0;
            for (int j = r.length() - 1; j >= 0; --j)
            {
                int tmp = rem + ctoi(l[i]) * ctoi(r[j]);
                if (tmp < 10)
                {
                    rem = 0;
                }
                else
                {
                    rem = tmp / 10;
                    tmp %= 10;
                }
                submul = itoc(tmp) + submul;
            }
            if (rem > 0)
            {
                submul = itoc(rem) + submul;
            }
            for (int j = 1; j < l.length() - i; ++j)
            {
                submul += '0';
            }
            bigNum tmp = {'1', submul};
            res = res + tmp;
        }
        if (sign == other.sign)
        {
            res.sign = '1';
        }
        else
        {
            res.sign = '0';
        }

        return res;
    }
};

bigNum operator*(int i, bigNum other)
{
    bigNum tmp;
    tmp.sign = '1';
    while (i > 0)
    {
        tmp.num = itoc(i % 10) + tmp.num;
        i /= 10;
    }
    if (tmp.num == "")
    {
        tmp.num = "0";
    }

    return tmp * other;
}

int main()
{
    while (1)
    {
        bigNum x;
        string _x;
        cin >> _x;
        x.sign = '1';
        x.num += _x;

        bigNum B = x * x;
        cout << "B = ";
        if (B.sign == '0')
        {
            cout << "-";
        }
        cout << B.num << ", ";
        bigNum N = {'1', "179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541085196591615128057575940752635007475935288710823649949940771895617054361149474865046711015101563940680527540071584560878577663743040086340742855278549092581"};
        if (B > N)
        {
            cout << "more than N is: ";
            bigNum tmp = B - N;
            if (tmp.sign == '0')
            {
                cout << "-";
            }
            cout << tmp.num << "." << endl;
        }
        else
        {
            cout << "less than N is: ";
            bigNum tmp = N - B;
            if (tmp.sign == '0')
            {
                cout << "-";
            }
            cout << tmp.num << "." << endl;
        }
    }

    return 0;
}
