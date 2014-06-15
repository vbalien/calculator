#include <stdio.h>
#include <string.h>

#define MAX_DIGIT   50
#define NUM_SIZE   MAX_DIGIT+1
#define VAR_MAX      10

void printNumber(int num[NUM_SIZE]);
void numZeroSet(int num[NUM_SIZE]);
void numCopy(int lvalue[NUM_SIZE], int rvalue[NUM_SIZE]);

void print_var(int vars[VAR_MAX][NUM_SIZE], char vars_name[VAR_MAX])
{
   int count = 0;
   while(vars_name[count] != '\0')
   {
      printf("%c = ", vars_name[count]);
      printNumber(vars[count]);
      count++;
   }
}

void register_var(int vars[VAR_MAX][NUM_SIZE], char vars_name[VAR_MAX], int item[NUM_SIZE], char name, int idx)
{
   if(idx >= VAR_MAX)
      return;
   numCopy(vars[idx], item);
   vars_name[idx] = name;
}

void clear_var(int vars[VAR_MAX][NUM_SIZE], char vars_name[VAR_MAX])
{
   int i;
   for(i=0;i<VAR_MAX;++i)
   {
      numZeroSet(vars[i]);
      vars_name[i] = '\0';
   }
}

int find_var(char vars_name[VAR_MAX], char name)
{
   int i;
   for(i=0;i<VAR_MAX;++i)
   {
      if(vars_name[i] == name)
         return i;
   }
   return -1;
}

int numDigit(int num[NUM_SIZE])
{
   int i;
   for(i=MAX_DIGIT-1;i>=0;--i)
      if(num[i] != 0)
         return i+1;
   return i+1;
}

int numCmp(int lvalue[NUM_SIZE], int rvalue[NUM_SIZE])
{
   int i;
   for(i=MAX_DIGIT-1;i>=0;--i)
      if((0 != lvalue[i]) || (0 != rvalue[i]))
         if(lvalue[i] > rvalue[i])
            return 1;
         else if(lvalue[i] < rvalue[i])
            return -1;
   return 0;
}

void printNumber(int num[NUM_SIZE])
{
   int i, isPrint = 0;
   if(num[MAX_DIGIT])
      putchar('-');
   for(i=MAX_DIGIT-1;i>=0;--i)
   {
      if(!isPrint && num[i] != 0)
         isPrint = 1;
      if(isPrint)
      {
         putchar(num[i]+'0');
      }
   }
   if(!isPrint)
      putchar('0');
   putchar('\n');
   isPrint = 0;
}

void numShift(int num[NUM_SIZE], int value)
{
   int i, isStart = 0;
   if(value == 0 || value >= MAX_DIGIT) return;
   else if(value > 0)
   {
      for(i=MAX_DIGIT-1-value;i>=0;--i)
      {
         if(!isStart && num[i] != 0)
            isStart = 1;
         if(isStart)
         {
            num[i+value] = num[i];
            num[i] = 0;
         }
      }
   }
   else
   {
      for(i=-value;i<MAX_DIGIT;++i)
      {
         num[i+value] = num[i];
         num[i] = 0;
      }
   }
}

void numCopy(int lvalue[NUM_SIZE], int rvalue[NUM_SIZE])
{
   int i;
   for(i=0;i<NUM_SIZE;++i)
      lvalue[i] = rvalue[i];
}

void numZeroSet(int num[NUM_SIZE])
{
   int i;
   for(i=0;i<NUM_SIZE;++i)
      num[i] = 0;
}

void calculator(char cOperator, int lvalue[NUM_SIZE], int rvalue[NUM_SIZE])
{
   int i;
   switch(cOperator)
   {
      case '+':
         if(lvalue[MAX_DIGIT] && !rvalue[MAX_DIGIT]) // lvalue가 음수이면
         {
            lvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('-', rvalue, lvalue);
            numCopy(lvalue, rvalue);
         }
         else if(!lvalue[MAX_DIGIT] && rvalue[MAX_DIGIT]) // rvalue가 음수이면
         {
            rvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('-', lvalue, rvalue);
         }
         else if(lvalue[MAX_DIGIT] && rvalue[MAX_DIGIT]) // 둘 다 음수이면
         {
            lvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            rvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('+', lvalue, rvalue);
            lvalue[MAX_DIGIT] = 1; // 음수로 바꿈
         }
         else
         {
            for(i=0;i<MAX_DIGIT;++i)
            {
               lvalue[i] += rvalue[i];
               lvalue[i+1] += lvalue[i]/10;
               lvalue[i] %= 10;
            }
         }
         break;
      case '-':
         if(lvalue[MAX_DIGIT] && !rvalue[MAX_DIGIT]) // lvalue가 음수이면
         {
            lvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('+', lvalue, rvalue);
            lvalue[MAX_DIGIT] = 1; // 음수로 바꿈
         }
         else if(!lvalue[MAX_DIGIT] && rvalue[MAX_DIGIT]) // rvalue가 음수이면
         {
            rvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('+', lvalue, rvalue);
         }
         else if(lvalue[MAX_DIGIT] && rvalue[MAX_DIGIT]) // 둘 다 음수이면
         {
            rvalue[MAX_DIGIT] = 0; // 양수로 바꿈
            calculator('+', lvalue, rvalue);
         }
         else if(numCmp(lvalue, rvalue) == -1)
         {
            calculator('-', rvalue, lvalue);
            numCopy(lvalue, rvalue);
            lvalue[MAX_DIGIT] = 1; // 음수로 바꿈
         }
         else
         {
            for(i=0;i<MAX_DIGIT;++i)
            {
               lvalue[i] -= rvalue[i];
               if(lvalue[i] < 0)
               {
                  --lvalue[i+1];
                  lvalue[i] += 10;
               }
            }
         }
         break;
      case '*':
      {
         int mbr = !(lvalue[MAX_DIGIT] == rvalue[MAX_DIGIT]);
         int j, tmp[2][NUM_SIZE] = {0,};
         for(i=0;i<MAX_DIGIT;++i)
         {
            numZeroSet(tmp[0]);
            for(j=0;j<MAX_DIGIT;++j)
            {
               tmp[0][j+i] += lvalue[j] * rvalue[i];
               tmp[0][j+i+1] += tmp[0][j+i]/10;
               tmp[0][j+i] %= 10;
            }
            calculator('+', tmp[1], tmp[0]);
         }
         numCopy(lvalue, tmp[1]);
         lvalue[MAX_DIGIT] = mbr;
         break;
      }
      case '/':
      {
         int mbr = !(lvalue[MAX_DIGIT] == rvalue[MAX_DIGIT]);
         lvalue[MAX_DIGIT] = 0;
         rvalue[MAX_DIGIT] = 0;
         switch(numCmp(lvalue, rvalue))
         {
            case 0:
               numZeroSet(lvalue);
               lvalue[0] = 1;
               break;
            case -1:
               numZeroSet(lvalue);
               break;
            case 1:
            {
               int shiftval, tmp[NUM_SIZE] = {0,};
               numShift(rvalue, shiftval = numDigit(lvalue) - numDigit(rvalue));
               for(i=0;i<=shiftval;++i)
               {
                  if(i!=0)
                     numShift(rvalue, -1);
                  while(i!=shiftval+1 && numCmp(lvalue, rvalue) != -1)
                  {
                     calculator('-', lvalue, rvalue);
                     ++tmp[shiftval-i];
                  }
               }
               numCopy(lvalue, tmp);
               lvalue[MAX_DIGIT] = mbr;
            }
            break;
         }
         break;
      }
      case '%':
      {
         if(rvalue[MAX_DIGIT])
         {
            /*  부호반전  */
            lvalue[MAX_DIGIT] = !lvalue[MAX_DIGIT];
            rvalue[MAX_DIGIT] = !rvalue[MAX_DIGIT];
            calculator('%', lvalue, rvalue);
            lvalue[MAX_DIGIT] = 1; // 음수로
         }
         else
         {
            switch(numCmp(lvalue, rvalue))
            {
               case 0:
                  numZeroSet(lvalue);
                  break;
               case -1:
                  if(lvalue[MAX_DIGIT])
                  {
                     lvalue[MAX_DIGIT] = 0;
                     calculator('-', rvalue, lvalue);
                     numCopy(lvalue, rvalue);
                  }
                  break;
               case 1:
               {
                  int shiftval = numDigit(lvalue) - numDigit(rvalue), lmbr = lvalue[MAX_DIGIT];
                  if(rvalue[MAX_DIGIT])
                  {
                     /*  부호반전  */
                     lvalue[MAX_DIGIT] = !lvalue[MAX_DIGIT];
                     rvalue[MAX_DIGIT] = !rvalue[MAX_DIGIT];
                     calculator('%', lvalue, rvalue);
                     lvalue[MAX_DIGIT] = 1; // 음수로
                  }
                  else
                  {
                     lvalue[MAX_DIGIT] = 0;
                     rvalue[MAX_DIGIT] = 0;
                     numShift(rvalue, shiftval);
                     for(i=0;i<=shiftval;++i)
                     {
                        if(i!=0)
                           numShift(rvalue, -1);
                        while(i!=shiftval+1 && numCmp(lvalue, rvalue) != -1)
                           calculator('-', lvalue, rvalue);
                     }
                     if(lmbr) // lvalue가 음수이면
                     {
                        calculator('-', rvalue, lvalue);
                        numCopy(lvalue, rvalue);
                     }
                  }
               }
               break;
            }
         }
         break;
      }
   }
}

int main(void)
{
   char cInput, cOperator[2] = {'\0', '\0'};
   int count = 0, pushIndex = 0, num[3][NUM_SIZE] = {0,}, i, is_makeval = 0, is_print = 1;
   int varcnt = 0, vars[VAR_MAX][NUM_SIZE] = {0,};
   char vars_name[VAR_MAX] = {0,}, makeval = '\0', buffer[5], buffcnt = 0;
   
   printf(">> ");
   while(cInput = getchar())
   {
      if(is_print || cInput == '\n')
      {
      switch(cInput)
      {
         case '+':case '-':
            buffcnt = 0;
            if(count == 0 && cInput == '-')
               num[pushIndex][MAX_DIGIT] = !num[pushIndex][MAX_DIGIT];
            else if(count == 0)
            {
               printf("Error!\n");
               is_print = 0;
            }
            else
            {
               if(cOperator[1] != '\0')
               {
                  calculator(cOperator[1], num[pushIndex-1], num[pushIndex]);
                  numZeroSet(num[pushIndex]);
                  cOperator[1] = '\0';
                  --pushIndex;
               }
               if(cOperator[0] != '\0') // 지금이 +나 -고 이전에 식이 있었다면 계산
               {
                  calculator(cOperator[0], num[pushIndex-1], num[pushIndex]);
                  numZeroSet(num[pushIndex]);
                  cOperator[0] = '\0';
                  --pushIndex;
               }
               if(cOperator[0] == '\0')
               {
                  cOperator[0] = cInput;
                  if(pushIndex == 0)
                     pushIndex = 1;
               }
            }
            count = 0;
            break;
         case '*':case '/':case '%':
            buffcnt = 0;
            if(count == 0)
            {
               printf("Error!\n");
               is_print = 0;
            }
            else
            {
               if((cOperator[0] == '+' || cOperator[0] == '-') && cOperator[1] == '\0') // 이전에 +나 -였다면
               {
                  cOperator[1] = cInput;
                  if(pushIndex == 1)
                     pushIndex = 2;
               }
               else if(cOperator[0] != '\0' && cOperator[0] != '+' && cOperator[0] != '-' && cOperator[1] == '\0')
               {
                  calculator(cOperator[0], num[pushIndex-1], num[pushIndex]);
                  numZeroSet(num[pushIndex]);
                  cOperator[0] = '\0';
               }
               else if(cOperator[1] != '\0')
               {
                  calculator(cOperator[1], num[pushIndex-1], num[pushIndex]);
                  numZeroSet(num[pushIndex]);
                  cOperator[1] = cInput;
               }
               if(cOperator[0] == '\0')
               {
                  cOperator[0] = cInput;
                  if(pushIndex == 0)
                     pushIndex = 1;
               }
            }
            count = 0;
            break;
         case '\n':
            if(count != 0 && cOperator[1] != '\0')
            {
               calculator(cOperator[1], num[pushIndex-1], num[pushIndex]);
               numZeroSet(num[pushIndex]);
               cOperator[1] = '\0';
               --pushIndex;
            }
            if(count != 0 && cOperator[0] != '\0')
            {
               calculator(cOperator[0], num[pushIndex-1], num[pushIndex]);
               numZeroSet(num[pushIndex]);
               cOperator[0] = '\0';
            }
            if(is_makeval)
            {
               int idx;
               if((idx=find_var(vars_name, makeval)) == -1)
                  register_var(vars, vars_name, num[0], makeval, varcnt++);
               else
                  register_var(vars, vars_name, num[0], makeval, idx);
            }
            else
            {
               int idx;
               buffer[buffcnt] = '\0';
               if(strcmp(buffer, "VAL") == 0)
               {
                  print_var(vars, vars_name);
                  is_print = 0;
               }
               else if(strcmp(buffer, "CLEAR") == 0)
               {
                  clear_var(vars, vars_name);
                  is_print = 0;
               }
               else if(strcmp(buffer, "END") == 0)
               {
                  return 0;
               }
               else if(buffcnt > 1)
               {
                  printf("Error!\n");
                  is_print = 0;
               }
               else if(buffer[0] != '\0' && (idx=find_var(vars_name, buffer[0])) == -1)
               {
                  printf("undefined.\n");
                  is_print = 0;
               }
            }
            if(is_print)
            {
               // 이 곳에 결과 출력
               putchar('=');
               printNumber(num[0]);
            }
            // 초기화
            numZeroSet(num[0]);
            numZeroSet(num[1]);
            numZeroSet(num[2]);
            cOperator[0] = '\0';
            cOperator[1] = '\0';
            makeval = '\0';
            count = 0;
            buffcnt = 0;
            pushIndex = 0;
            is_makeval = 0;
            is_print = 1;
            printf(">> ");
            break;
         case '=':
            buffcnt = 0;
            is_makeval = 1;
            break;
         default:
            if('0' <= cInput && cInput <= '9' && count < MAX_DIGIT)
            {
               int nInput = cInput - '0';
               if(count > 0)
                  for(i=count;i>0;--i)
                     num[pushIndex][i] = num[pushIndex][i-1];
               num[pushIndex][0] = nInput;
               ++count;
            }
            else if(('A' <= cInput && cInput <= 'Z') ||
                  ('a' <= cInput && cInput <= 'z'))
            {
               int idx;
               if('a' <= cInput && cInput <= 'z')
                  cInput -= ('a'-'A');
               if(!is_makeval)
               {
                  makeval = cInput;
                  buffer[buffcnt++] = cInput;
				  ++count;
               }
               if((idx=find_var(vars_name, cInput)) != -1)
                  numCopy(num[pushIndex], vars[idx]);
                  
            }
            break;
      }
      }
   }
   return 0;
}
