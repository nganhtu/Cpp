#include <stdio.h>
#include <math.h>

int cube(int x)
{
    //# trả về lập phương của x
    return pow(x, 3);
}

//# viết hàm tính lập phương của một số kiểu double
double cube(double x)
{
    return pow(x, 3);
}

int main()
{
    int n;
    double f;
    scanf("%d %lf", &n, &f);

    printf("Int: %d\n", cube(n));
    printf("Double: %.2lf\n", cube(f));

    return 0;
}
