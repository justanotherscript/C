#include<stdio.h>
int main() {
   int a, b, choice;
   printf("Enter two integers: ");
   scanf("%d %d", &a, &b);
   printf("Enter choice: 1.Addition 2.Subtraction 3.Multiplication 4.Division\n");
   scanf("%d", &choice);
   switch(choice) {
      case 1: printf("Result: %d", a + b);
         break;
      case 2: printf("Result: %d", a - b);
         break;
      case 3: printf("Result: %d", a * b);
         break;
      case 4: if(b != 0)
                 printf("Result: %.2f", (float)a / b);
              else
                 printf("Error! Division by zero not possible.");
         break;
      default: printf("Error! Invalid choice.");
         break;
   }
   return 0;
}
