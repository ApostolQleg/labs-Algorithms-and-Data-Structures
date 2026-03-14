#include <stdio.h>
#include <math.h>

/*
 * Рекурсивна функція для обчислення на рекурсивному поверненні.
 * i - поточний крок
 * x - аргумент
 * current_F - вказівник для передачі значення поточного члена ряду "нагору"
 */
double sum_ascent_recursive(int i, double x, double *current_F)
{
    // Базовий випадок: доходимо до початку ряду (i = 1)
    if (i == 1)
    {
        *current_F = 1.0; // F_1 через вказівник
        return 1.0;       // сума першого члена
    }

    // Спуск: рекурсивний виклик для попереднього кроку (i - 1).
    // Коли ця функція завершиться, у *current_F буде записано значення F_{i-1}.
    double prev_sum = sum_ascent_recursive(i - 1, x, current_F);

    // Підйом: обчислюємо поточний F_i, оновлюючи значення за вказівником
    *current_F = -(*current_F) * x * (3.0 * i - 5.0) / (3.0 * i - 3.0);

    // Повертаємо загальну суму (сума попередніх + поточний член)
    return prev_sum + *current_F;
}

/*
 * Функція-обгортка
 */
double calculate_sum_ascent(int n, double x)
{
    if (n < 1)
    {
        return 0.0;
    }

    double f_val = 0.0; // Змінна, куди рекурсія записує поточний член ряду

    // Викликаємо рекурсію, передаючи адресу змінної f_val
    return sum_ascent_recursive(n, x, &f_val);
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

    // Обчислення суми ряду рекурсивним алгоритмом (підйом)
    double result = calculate_sum_ascent(n, x);

    // Обчислення точного значення функції
    double exact_value = 1.0 / cbrt(1.0 + x);

    // Обчислення похибки
    double error = fabs(result - exact_value);

    printf("\nYou entered n = %d,\nand x = %.8f\n", n, x);

    printf("\nResults:\n");
    printf("The sum of the first %d terms of the series (Ascent):   %.8f\n", n, result);
    printf("The exact value of 1 / cbrt(1 + x):                    %.8f\n", exact_value);
    printf("Absolute error:                                        %.8f\n", error);

    return 0;
}