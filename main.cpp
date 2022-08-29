#include <iostream>
#include <string>
using namespace std;

// Token types
enum class TOKENTYPE {
    ENDOFLINE = 0, // EOF can't be used as it is already defined in std
    INTEGER,
    MUL,
    DIV
};

class Interpreter;

class Token {
public:
    Token(TOKENTYPE t, long v):_type{t},_value{v}{};

    TOKENTYPE _type;
    long _value;
    friend ostream& operator <<(ostream& out, Token token);
};

ostream & operator<<(ostream& out, TOKENTYPE t){
    string repr;
    switch (t) {
        case TOKENTYPE::ENDOFLINE:
            repr = "ENDOFLINE";
            break;
        case TOKENTYPE::INTEGER:
            repr = "INTEGER";
            break;
        case TOKENTYPE::MUL:
            repr = "DIV";
            break;
        case TOKENTYPE::DIV:
            repr = "MUL";
            break;
    }
    out << repr;
    return out;
}

ostream& operator<<(ostream& out, Token token){
    out << "Token(" << token._type << ", " << token._value << ")";
    return out;
}

class Lexer {
private:
    string _text;
    long _pos;
    char _current_char;
public:
    Lexer(string& text):_text{text}, _pos{0}, _current_char{_text[_pos]}{};
    void error(){
        string message = "Invalid character";
        throw message.c_str();
    }

    void advance(){
        ++_pos;
        if(_pos > _text.length() - 1) {
            _current_char = '\0';
        }else{
            _current_char = _text[_pos];
        }
    }

    void skip_whitespace(){
        while (_current_char != '\0' && isspace(_current_char)){
            advance();
        }
    }

    long integer(){
        long result = 0;
        while (_current_char != '\0' && isdigit(_current_char)){
            result = result * 10 + (_current_char - '0');
            advance();
        }
        return result;
    }

    inline Token get_next_token();
};

inline Token Lexer::get_next_token() {
    while(_current_char != '\0'){
        if(isspace(_current_char)){
            skip_whitespace();
            continue;
        }
        if(isdigit(_current_char)){
            return Token(TOKENTYPE::INTEGER, integer());
        }
        if(_current_char == '*'){
            advance();
            return Token(TOKENTYPE::MUL, '*');
        }
        if(_current_char == '/') {
            advance();
            return Token(TOKENTYPE::DIV, '/');
        }

        error();
        return Token(TOKENTYPE::ENDOFLINE, 0);
    }
}


class Interpreter{
private:
    Lexer _lexer;
    Token _current_token = _lexer.get_next_token();
public:
    Interpreter(Lexer& lexer): _lexer{lexer}{};
    void error(){
        string message = "Invalid syntax";
        throw message.c_str();
    }

    void eat(TOKENTYPE tokentype){
        if(_current_token._type == tokentype){
            _current_token = _lexer.get_next_token();
        } else{
            error();
        }
    }
    long factor(){
        long value = _current_token._value;
        eat(TOKENTYPE::INTEGER);
        return value;
    }

    long expr(){
        long result = factor();

        while(_current_token._type == TOKENTYPE::MUL
              || _current_token._type == TOKENTYPE::DIV){
            Token token = _current_token;
            if(token._type == TOKENTYPE::MUL){
                eat(TOKENTYPE::MUL);
                result *= factor();
            }else if(token._type == TOKENTYPE::DIV){
                eat(TOKENTYPE::DIV);
                result /= factor();
            }
        }
        return result;
    }
};

int main(){
    string text;
    while (cin){
        cout << "calc> ";
        getline(cin, text);
        Lexer lexer(text);
        Interpreter interpreter(lexer);
        long result = interpreter.expr();
        cout << result << endl;
    }
    return EXIT_SUCCESS;
}


