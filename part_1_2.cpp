#include<iostream>
#include<sstream>
using namespace std;
enum class TOKENTYPE {
    ENDOFLINE = 0,
    INTEGER,
    MINUS,
    PLUS
};

ostream &operator<<(ostream &out, const TOKENTYPE &t) {
    string repr;
    switch (t) {
        case TOKENTYPE::ENDOFLINE:
            repr = "ENDOFLINE";
            break;
        case TOKENTYPE::INTEGER:
            repr = "INTEGER";
            break;
        case TOKENTYPE::MINUS:
            repr = "MINUS";
            break;
        case TOKENTYPE::PLUS:
            repr = "PLUS";
            break;
    }
    out << repr;
    return out;
}

struct Token {
    Token(TOKENTYPE t, long v) : type{t}, value{v} {}

    TOKENTYPE type;
    long value;

    friend ostream &operator<<(ostream &out, const Token &t);
};

ostream &operator<<(ostream &out, Token &t) {
    out << "Token(" << t.type << ", " << t.value << ")";
    return out;
}

class Interpreter {
public:
    Interpreter(string &text);

    long expression();

private:
    string &_text;
    size_t _pos;
    Token _current_token;

    void eat(TOKENTYPE token_type);

    Token get_next_token();

    long minus(long left_value);

    long plus(long left_value);
};

Interpreter::Interpreter(std::string &text) : _text{text}, _pos{0},
                                              _current_token{TOKENTYPE::ENDOFLINE, 0} {}

long Interpreter::expression() {

    _current_token = get_next_token();
    Token left = _current_token;
    long result = left.value;
    eat(TOKENTYPE::INTEGER);

    while(_current_token.type != TOKENTYPE::ENDOFLINE) {
        if(_current_token.type == TOKENTYPE::MINUS) {
            eat(TOKENTYPE::MINUS);
            result = minus(result);
        } else if(_current_token.type == TOKENTYPE::PLUS) {
            eat(TOKENTYPE::PLUS);
            result = plus(result);
        } else {
            ostringstream out;
            out << "Error: can't resolve " << _current_token;
            throw invalid_argument(out.str());
        }
    }

    return result;

}

void Interpreter::eat(TOKENTYPE token_type) {
    if(_current_token.type== token_type) {
        _current_token = get_next_token();
    } else {
        ostringstream  out;
        out << "Error parsing input. Wanted: " << token_type;
        throw invalid_argument(out.str());
    }
}

Token Interpreter::get_next_token() {
    while (_pos < _text.length() && isspace(_text[_pos])) {
        _pos++;
    }

    if(_pos >= _text.length()) {
        Token token(TOKENTYPE::ENDOFLINE, 0);
        cerr << token << endl;
        return token;
    }

    char current_char = _text[_pos];

    if(isdigit(current_char)) {
        long total = 0;
        while (isdigit(_text[_pos])) {
            total *= 10;
            total += (_text[_pos] - '0');
            _pos++;
        }

        Token token(TOKENTYPE::INTEGER, total);
        cerr << token << endl;
        return token;
    }

    if(current_char == '+') {
        Token token(TOKENTYPE::PLUS, '+');
        cerr << token << endl;
        _pos++;
        return token;
    }

    if(current_char == '-') {
        Token token(TOKENTYPE::MINUS, '-');
        cerr << token << endl;
        _pos++;
        return token;
    }

    ostringstream out;
    out << "Error parsing input. Got: " << current_char;
    throw invalid_argument(out.str());
}

long Interpreter::minus(long left_value) {
    Token right = _current_token;
    eat(TOKENTYPE::INTEGER);
    return left_value - right.value;
}

long Interpreter::plus(long left_value) {
    Token right = _current_token;
    eat(TOKENTYPE::INTEGER);
    return left_value + right.value;
}

int main() {
    string text;
    while(cin) {
        cout << "calc> ";
        getline(cin, text);

        try {
            Interpreter interpreter(text);
            long result = interpreter.expression();
            cout << result << endl;
        }
        catch (const char* error){
            cerr << error << endl;
            break;
        }
    }
    return EXIT_SUCCESS;
}











