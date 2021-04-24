int lucas(int n)
{
    int a = 0, b = 2, c = 1;
    if (n < 0)
    {
        return -1;
    }
    if (n == 0)
    {
        return 2;
    }
    while (--n)
    {
        a = b;
        b = c;
        c = a + b;
    }
    return c;
}
