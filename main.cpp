#include <iostream>
#include "cstring"

template<typename T> class stack
{
#define STACK_CAPACITY 2048

private:
	T elem[STACK_CAPACITY];
	int current = -1;

public:
	void push(T x)
	{
		if (current < STACK_CAPACITY - 1)
		{
			current++;
			this->elem[current] = x;
		}
		else
		{
			std::cout << "STACK OVERFLOW" << std::endl;
		}
	}
	
	T pop(void)
	{
		if (current >= 0)
		{
			return this->elem[current--];
		}
		else
		{
			std::cout << "STACK EMPTY" << std::endl;
			return -1;
		}
	}
	
	T top(void)
	{
		if (current >= 0)
		{
			return this->elem[current];
		}
		else
		{
			std::cout << "STACK EMPTY" << std::endl;
			return -1;
		}
	}
	
	bool isEmpty(void)
	{
		return (current == -1);
	}
};

#define isAND(c) (c == '.' || c == '*' || c == '&' || c == '^')
#define isOR(c) (c == '+' || c == 'V' || c == 'v' || c == '|')
#define isNOT(c) (c == '!' || c == '-' || c == '~')

void exprToRPN(const char *expr, char *rpn)
{
	stack<char> op;
	int i, j;
	char c;
	
	j = 0;
	for (i = 0; (c = expr[i]) != '\0'; i++)
	{
		if (c == '1' || c == '0')
			rpn[j++] = c;
		else if (c == 'T')
			rpn[j++] = '1';
		else if (c == 'F')
			rpn[j++] = '0';
		else if (c == '(')
			op.push(c);
		else if (c == ')')
			while ((c = op.pop()) != '(') rpn[j++] = c;
		else if (isNOT(c))
			op.push(c);
		else if (isAND(c))
		{
			while (!op.isEmpty() && (isNOT(op.top()) || isAND(op.top())))
				rpn[j++] = op.pop();
			op.push(c);
		}
		else if (isOR(c))
		{
			while (!op.isEmpty() && (isNOT(op.top()) || isAND(op.top()) || isOR(op.top())))
				rpn[j++] = op.pop();
			op.push(c);
		}
		else if (c >= 'A' && c <= 'Z')
			rpn[j++] = c;
	}
	while (!op.isEmpty())
		rpn[j++] = op.pop();
	rpn[j] = '\0';
}

int calcRPN(const char rpn[])
{
	stack<char> op;
	char c;
	
	for (int i = 0; (c = rpn[i]) != 0; i++)
	{
		if (c == '1' || c == '0')
			op.push(c - '0');
		else if (c == 'T')
			op.push(1);
		else if (c == 'F')
			op.push(0);
		else if (isNOT(c))
			op.push(!op.pop());
		else if (isOR(c))
			op.push(op.pop() | op.pop());
		else if (isAND(c))
			op.push(op.pop() & op.pop());
	}
	return op.pop();
}

int getBit(int bin, int pos)
{
	return ((bin >> pos) & 1);
}

int replaceByVal(char *rpn, const char *origin, char name, int val)
{
	for (int i = 0; origin[i] != '\0'; i++)
	{
		if (origin[i] == name) rpn[i] = (char)val;
	}
}

void printTruthTable(const char *rpn_expr, const char *var_list)
{
	char rpn[2048];
	int nvar = (int)strlen(var_list);
	int nexpl = 1 << nvar;
	
	strcpy(rpn, rpn_expr);
	std::cout << std::endl;
	for (int i = 0; i < nvar; i++)
	{
		std::cout << var_list[i] << '\t';
	}
	std::cout << "VAL" << std::endl;
	
	for (int i = 0; i < nexpl; i++)
	{
		for (int j = 0; j < nvar; j++)
		{
			std::cout << getBit(i, nvar - j - 1) << '\t';
			replaceByVal(rpn, rpn_expr, var_list[j], '0' + getBit(i, nvar - j - 1));
		}
		std::cout << calcRPN(rpn) << std::endl;
	}
}

int main()
{
	char input[2048];
	char var_list[26];
	char rpn_expr[2048];
	while (1)
	{
		if (!(std::cin >> var_list)) break;
		std::cin >> input;
		exprToRPN(input, rpn_expr);
		std::cout << rpn_expr << std::endl;
		//std::cout<<calcRPN(rpn_expr);
		printTruthTable(rpn_expr, var_list);
	}
	return 0;
}
