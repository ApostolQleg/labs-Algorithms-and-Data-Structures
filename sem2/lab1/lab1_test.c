#include <stdio.h>
#include <math.h>

/*
 * Ітеративна (циклічна) функція для обчислення суми ряду.
 */
double calculate_sum_iterative(int n, double x)
{
    if (n < 1)
    {
        return 0.0;
    }

    double current_F = 1.0; // Перший член ряду (F_1 = 1)
    double sum = 1.0;       // Початкова сума, яка дорівнює першому члену

    // Починаємо цикл з другого члена ряду (i = 2) і йдемо до n
    for (int i = 2; i <= n; i++)
    {
        // Обчислюємо поточний член ряду на основі попереднього
        current_F = -current_F * x * (3.0 * i - 5.0) / (3.0 * i - 3.0);
        
        // Додаємо обчислений член до загальної суми
        sum += current_F;
    }

    return sum;
}

int main()
{
    int n;
    double x;

    printf("Enter the number of terms (n >= 1): ");
    scanf("%d", &n);

    if (n < 1)
    {
        printf("Error: n must be a positive integer.\n");
        return 1;
    }

    printf("Enter the value of x (|x| < 1):     ");
    scanf("%lf", &x);

    if (fabs(x) >= 1.0)
    {
        printf("Error: |x| must be strictly less than 1.\n");
        return 1;
    }

    // Обчислення суми ряду циклічним алгоритмом
    double result = calculate_sum_iterative(n, x);

    // Обчислення точного значення функції
    double exact_value = 1.0 / cbrt(1.0 + x);

    // Обчислення похибки
    double error = fabs(result - exact_value);

    printf("\nYou entered n = %d,\nand x = %.8f\n", n, x);

    printf("\nResults:\n");
    printf("The sum of the first %d terms of the series (Iterative):  %.8f\n", n, result);
    printf("The exact value of 1 / cbrt(1 + x):                      %.8f\n", exact_value);
    printf("Absolute error:                                          %.8f\n", error);

    return 0;
}