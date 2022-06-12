#include <bits/stdc++.h>

typedef struct Polynom
{
    int coeff;
    int pow;
    struct Polynom *link;
} poly;

poly *tichdathuc(poly *px)
{
    if (px->link == NULL)
    {
        px->link = (poly *)malloc(sizeof(poly));
        px->link->coeff = 2 * px->coeff;
        px->link->pow = px->pow + 1;
        px->link->link = NULL;
    }
    else
    {
        px->link = tichdathuc(px->link);
        px->link->coeff += 2 * px->coeff;
    }

    return px;
}
