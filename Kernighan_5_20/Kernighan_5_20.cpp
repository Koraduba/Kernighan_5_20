
#include<stdio.h>
#include<string.h>
#include<ctype.h>

enum tt { NAME, PARENS, BRACKETS, E1, E2, E3, E4, E5 };

char* dtype[] = { "char", "int", "long", "double", "const" };

#define MAXTOKEN 100

char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];
int tokentype;

void dcl(void);
void dirdcl(void);
void func(void);

int gettoken(void);

int main()
{

	while (gettoken() != EOF)
	{
		
		if (!strcmp(token, "const"))
		{
			strcpy(datatype, "const ");
			gettoken();
		}
		else datatype[0] = '\0';

		if ((strcmp(token, "char")) && (strcmp(token, "int")))
		{
			printf("Error: wrong datatype!\n");
			while (gettoken() != '\n');
			continue;
		} 
		strcat(datatype, token);

		out[0] = '\0';
		dcl();
		switch (tokentype)
		{
		case E1:
			printf("error E1: more than one identificator!\n");
			break;
		case E2:
			printf("error E2: syntax\n");
			break;
		case E3:
			printf("error E3: missing )\n");
			break;
		case E4:
			printf("error E4: name or dcl expected\n");
			break;
		case E5:
			printf("error E5: wrong name\n");
			break;
		default:
			printf("%s: %s %s\n", name, out, datatype);
		}

	}

	return 0;
}

void dcl(void)
{
	int ns;
	for (ns = 0; gettoken() == '*';)ns++;
	dirdcl();
	while (ns-- > 0) strcat(out, " pointer to");

}

void dirdcl(void)
{
	int type;

	if (tokentype >= E1 && tokentype <= E5) return;

	if (tokentype == '(')
	{
		printf("!");
		dcl();
		if (tokentype >= E1 && tokentype <= E5) return;
		if (tokentype != ')')
		{
			while (tokentype != '\n') gettoken();
			tokentype = E3;
			return;
		}

	}
	else if (tokentype == NAME)
	{
		strcpy(name, token);
	}
	else if (isdigit(tokentype))
	{
		while (tokentype != '\n') gettoken();
		tokentype = E5;
		return;
	}
	else
	{
		while (tokentype != '\n') gettoken();
		tokentype = E4;
		return;
	}


	while ((type = gettoken()) == PARENS || type == BRACKETS || type == '(')
		if (type == PARENS) strcat(out, " function returning");
		else if (type ==BRACKETS)
		{
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
		else
		{
			func();
			if (tokentype == E2) return;
		}


	if (type != ')'&&type != '\n')
	{
		while (gettoken() != '\n');
		printf("!!");
		if (type == NAME)	tokentype = E1;
		else tokentype = E2;
	}

}

void func(void)
{
	int type;
	int i=0;
	char* temp;
	int metka = 0;

	strcat(out, " function with parameter(s)");

	type = gettoken();
	while (type != ')')
	{
		if (!(strcmp(token, "const")))

			if (metka == 1)
			{
				while (gettoken() != '\n');
				tokentype = E2;
				return;
			}
			else
			{
				printf("&");
				metka = 1;
				type=gettoken();
				printf("%d", type);

				continue;
			}	
		
		if (type == NAME)
		{
			printf("&");

			for (i = 0; i <=3; i++)
				if (!(strcmp(token, dtype[i])))
				{
					printf("&");

					temp = token;
					strcat(out, " ");
					while ((type = gettoken()) == '*') strcat(out, "pointer to ");
					if (metka)
					{
						metka = 0;
						strcat(out, "const ");
					}
					strcat(out, temp);
					break;
				}
			if (i == 4)
			{
				while (gettoken() != '\n');
				tokentype = E2;
				return;
			}

		}
		else if ((type == ',')&&(!metka))
		{
			strcat(out, " and"); 
			type=gettoken();
		}
		else
		{
			while (gettoken() != '\n');
			tokentype = E2;
			return;
		}
	}

	if (metka)
	{
			while (gettoken() != '\n');
			tokentype = E2;
			return;
	}

	strcat(out, " returning");

}


int gettoken(void)
{
	int getch(void);
	void ungetch(int);

	int c;
	char*p = token;

	while ((c = getch()) == ' ' || c == '\t');

	if (c == '(')
	{
		while ((c = getch()) == ' ');
		if (c == ')')
		{
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else
		{
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if (c == '[')
	{
		*p++ = c;
		while ((*p++ = getch()) == ' ');
		if (isdigit(*(p - 1))) while (isdigit(*p++ = getch()));
		if (*(p - 1) == ' ') while ((*p++ = getch()) == ' ');
		*p = '\0';
		if (*(p - 1) == ']') return tokentype = BRACKETS;
		else return tokentype = c;
	}
	else if (isalpha(c))
	{
		for (*p++ = c; (isalnum(c = getch()));)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}
	else return tokentype = c;
}

#define BUFSIZE 10
char buffer[BUFSIZE];
int bufp = 0;

int getch(void)
{
	int c;
	if (bufp == 0) c = getchar();
	else c = buffer[--bufp];
	return c;
}

void ungetch(int c)
{
	if (bufp == BUFSIZE) printf("Error");
	else
	{
		buffer[bufp++] = c;
	}
}