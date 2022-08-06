#pragma once
#ifndef MY_SCANNER
#define MY_SCANNER

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

enum FLAG
{
    FG_NORMAL, FG_EOF
};

class scanner
{
public:
	scanner(const char* file_path);
	~scanner();
    void printFile();
    void printTokens();
private:
    int flag;
	vector<string> token_buf;
    ifstream file;
    /*
    * Scan token
    */
    string getToken();
    char getChar();
    char peekChar();

    /*
    * Check char method
    */
    int isWhiteSpace(char c);
    int isOP(char c);
    int isDIGIT(char c);
    int isALPHA(char c);

};

scanner::scanner(const char* file_path)
{
    flag = FG_NORMAL;
    file.open(file_path, ifstream::in);
}

scanner::~scanner()
{
    file.close();
}

string scanner::getToken()
{
    char c, c_tmp = 0;
    string token = "";
    c = peekChar();

    /*
    * Skip WhiteSpace
    */
    if (flag != FG_EOF && isWhiteSpace(c))
    {
        do
        {
            getChar();
            c = peekChar();

        } while (flag != FG_EOF && isWhiteSpace(c));
    }

    /*
    * Skip Comment
    */

    if (c == '/')
    {
        c_tmp = c;
        getChar();
        c = peekChar();

        if (c_tmp == '/' && c == '/')
        {
            do
            {
                getChar();
                c = peekChar();
            } while (flag != FG_EOF && c != '\n');

            return token;
        }
        else if (c_tmp == '/' && c == '*')
        {
            getChar(); // get '*'

            do
            {
                c = peekChar();
                getChar();
                c_tmp = c;
                c = peekChar();
            } while (flag != FG_EOF && !(c_tmp == '*' && c == '/'));

            getChar(); // ger '/'

            return token;
        }
        else
        {
            // is OP
            token += c_tmp;
            while (flag != FG_EOF && isOP(c))
            {
                getChar();
                token += c;
                c = peekChar();
            }                       // �@��Ū�줣�OOP����

            return token;
        }

    }

    if (flag == FG_EOF)
        return token;

    /*
    * Token Start
    */

    if (c == '\"') { // string = ".."                            // �p�G�O "�A�N��r��}�Y,
        do
        {
            getChar();
            token += c;
            c_tmp = c;
            c = peekChar();

        } while (flag != FG_EOF && ((c_tmp =='\\' && c == '\"') || c != '\"'));

        getChar();
        token += c; // '\"' string tail
    }
    else if (isOP(c)) { // OP , ex : ++, --, <=, >=, .// �p�G�OOP(+-*/<=>!���Ÿ�)

        do
        {

            getChar();
            token += c;
            c = peekChar();

        } while (flag != FG_EOF && isOP(c));                        // �@��Ū�줣�OOP����
    }
    else if (isDIGIT(c)) { // number, ex : 312, 77568,// �p�G�O�Ʀr
        do
        {
            getChar();
            token += c;
            c = peekChar();
        } while (flag != FG_EOF && isDIGIT(c));                     // �@��Ū�줣�O�Ʀr����
    }
    else if (isALPHA(c)) { // name, ex : int, sum,     for, if,
        do
        {
            getChar();
            token += c;
            c = peekChar();
        } while (flag != FG_EOF && (isALPHA(c) || isDIGIT(c)));   // �p�G�O�^��r��,�@��Ū�줣�O�^�Ʀr����

    }
    else // some other symbol, such as #
    {
        getChar();
        if (flag != FG_EOF)
            token += c;// �Ǧ^��@�r��
    }

    return token;                                       // �Ǧ^token���J
}


char scanner::getChar() {
    char c;
    c = file.get();
    return c;
}

char scanner::peekChar() {
    char c;
    c = file.peek();

    if (file.eof())
        flag = FG_EOF;

    return c;
}

int scanner::isWhiteSpace(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

int scanner::isOP(char c)
{
    switch (c)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '=':
        case '>':
        case '!':
        case '&':
        case '|':
            return 1;
            break;
        default:
            return 0;
            break;
    }

    return 0;
}

int scanner::isDIGIT(char c)
{
    if (c >= '0' && c <= '9')
        return 1;

    return 0;
}

int scanner::isALPHA(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;

    return 0;
}


void scanner::printTokens()
{
    string token;
    while (flag != FG_EOF)
    {
        token = getToken();
        if(token != "")
            cout << "token:" << token << endl;
    }
}

void scanner::printFile()
{
    char c;
    while (file.get(c))                  // loop getting single characters
        cout << c;

    if (file.eof())                      // check for EOF
        cout << "\n[EoF reached]\n";
    else
        cout << "[error reading]\n";

}

#endif // !MY_SCANNER
