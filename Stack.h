#pragma once
#include <string>

using namespace std;

template <class T>

class Stack
{
public:
	virtual T GetTop() { return 0; };
	virtual void Push(T object) { };
	virtual T Pop() { return 0; };
	virtual bool IsEmpty() { return true; };
	virtual int NumberOfElements() { return 0; };
};

template <class T>

class StackArray : public Stack<T>
{
protected:
	T* array;
	int size;
	int top;

public:
	StackArray(int size) {
		this->size = size;
		array = new T[size];
		top = -1;
	};
	~StackArray() { delete[] array; };
	bool IsEmpty() { return top == -1; };
	int NumberOfElements() { return top + 1; };
	void Push(T object) {
		if (top == size - 1)
			return;

		array[++top] = object;
	}
	T Pop() {
		if (top == -1)
			return 0;

		return array[top--];
	}
	T GetTop() {
		if (top == -1)
			return -1;

		return array[top];
	}

	// funkcije sa ispita
	// za infix u postfix
	int Prec(char op) {
		if (op == '*' || op == '/') return 2;
		if (op == '+' || op == '-') return 1;
		return 0;
	};
	bool IsOperator(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/';
	};
	int IDontKnow(string expr) { // racuna vrednost izraza	
		StackArray<char> operators(20);
		string result;

		for (int i = 0; i < expr.length(); i++)
		{
			char c = expr[i];

			if (c >= '0' && c <= '99')
				result += c;
			else if (c == '(')
				operators.Push('(');
			else if (c == ')') {
				while (operators.GetTop() != '(') {
					result += operators.GetTop();
					operators.Pop();
				}

				operators.Pop();
			}
			else {
				while (!operators.IsEmpty() && Prec(c) <= Prec(operators.GetTop())) {
					result += operators.GetTop();
					operators.Pop();
				}
				operators.Push(c);
			}
		}

		while (!operators.IsEmpty()) {
			result += operators.GetTop();
			operators.Pop();
		}

		// std::cout << result;

		StackArray<int> final(20);

		for (int i = 0; i < result.length(); i++) {
			char c = result[i];

			if (isdigit(c))
				final.Push(c - '0'); //pretvori u broj
			else
			{
				int a = final.GetTop();
				final.Pop();
				int b = final.GetTop();
				final.Pop();

				switch (c)
				{
				case '+':
					final.Push(a + b);
					break;
				case '-':
					final.Push(b - a);
					break;
				case '*':
					final.Push(a * b);
					break;
				case '/':
					final.Push(b / a);
					break;
				default:
					break;
				}
			}
		}

		return final.GetTop();
	}
	// ---------------------
	int CalculatePostfix(const char* exp) {
		// racuna izraz zadat u postfix notaciji, npr: 2 3 + 10 * 5 /
		StackArray<int> stack(10);

		int i = 0;
		while (exp[i] != '\0') {
			if (isdigit(exp[i])) {
				int num = 0;
				while (exp[i] != ' ') { // 243
					num = num * 10 + (exp[i] - '0');
					i++;
				}

				stack.Push(num);
			}
			else if (IsOperator(exp[i])) {
				// ako je operator, skidamo oba podatka sa steka, on ostaje prazan
				int a = stack.Pop();
				int b = stack.Pop();

				switch (exp[i])
				{
				case '+':
					stack.Push(a + b);
					break;
				case '-':
					stack.Push(b - a);
					break;
				case '*':
					stack.Push(a * b);
					break;
				case '/':
					stack.Push(b / a);
					break;
				default:
					break;
				}
			}

			i++;
		}

		return stack.Pop();
	}

	bool IsParenthesis(char c) {
		return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
	}
	bool CheckExpression(const char* exp) {
		// proverava da li je izraz ispravan po pitanju otvorenih i zatv. zagrada
		// mogu se pojaviti (, {, [, ], }, )

		// () - 1
		// {} - 2
		// [] - 3
		StackArray stack(30);

		int i = 0;
		int map;

		while (exp[i] != '\0') {
			if (!IsParenthesis(exp[i]) || exp[i] == ' ')
				i++;

			switch (exp[i])
			{
			case '(':
				stack.Push(1);
				break;
			case '{':
				stack.Push(2);
				break;
			case '[':
				stack.Push(3);
				break;

			case ')':
				map = stack.Pop();
				if (map != 1)
					return false;
				break;
			case '}':
				map = stack.Pop();
				if (map != 2)
					return false;
				break;
			case ']':
				map = stack.Pop();
				if (map != 3)
					return false;
				break;

			default:
				break;
			}

			i++;
		}

		return true;
	}

	void AddLong(const char* op1, const char* op2) {
		StackArray<char> stack1(20);
		StackArray<char> stack2(20);
		StackArray<char> result(30);

		int i = 0;
		while (op1[i] != '\0') {
			stack1.Push(op1[i]);
			i++;
		}

		i = 0;
		while (op2[i] != '\0') {
			stack2.Push(op2[i]);
			i++;
		}

		int carry = 0;
		while (!stack1.IsEmpty() || !stack2.IsEmpty()) {
			// pretpostavka da Pop vraca 0 kad je stek prazan
			int a = stack1.Pop() - '0';
			int b = stack2.Pop() - '0';
			int c = carry;
			int res = a + b + c;

			if (res >= 10) {
				carry = res / 10;
				int final = res % 10;
				result.Push(final + '0');
			} 
			else {
				char final = res + '0';
				result.Push(final);
				carry = 0;
			}
		}

		if (carry != 0)
			result.Push(carry + '0');

		while (!result.IsEmpty()) {
			char a = result.Pop();
			cout << a << " ";
		}
	}
	
	double CalculatePrefix(const char* exp) {
		// svi operandi su jednocifreni i da nema razmaka
		// npr: **45-934 = 216

		// isto kao postfix
	}


};