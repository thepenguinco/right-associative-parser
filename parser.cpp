#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <cmath>

enum TokenType { NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, LPAREN, RPAREN, END };

// Token structure
struct Token {
    TokenType type;
    double value;  // Used if the token is a number
};

// Tokenizer class
class Tokenizer {
public:
    explicit Tokenizer(const std::string& input) : input(input), pos(0) {}
    
    Token getNextToken() {
        while (pos < input.size() && isspace(input[pos])) pos++;  // Skip spaces
        
        if (pos >= input.size()) return Token{END, 0};  // End of input
        
        char current = input[pos];
        
        if (isdigit(current) || current == '.') {
            double number = parseNumber();
            return Token{NUMBER, number};
        }
        
        pos++;
        
        switch (current) {
            case '+': return Token{PLUS, 0};
            case '-': return Token{MINUS, 0};
            case '*': return Token{MULTIPLY, 0};
            case '/': return Token{DIVIDE, 0};
            case '(': return Token{LPAREN, 0};
            case ')': return Token{RPAREN, 0};
            default:
                throw std::runtime_error("Invalid character in input");
        }
    }

private:
    std::string input;
    size_t pos;
    
    double parseNumber() {
        size_t start = pos;
        while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) pos++;
        return std::stod(input.substr(start, pos - start));
    }
};

// Parser class implementing recursive descent with right-associativity and unary negation
class Parser {
public:
    explicit Parser(Tokenizer& tokenizer) : tokenizer(tokenizer), currentToken(tokenizer.getNextToken()) {}

    double parse() {
        return parseExpression();
    }

private:
    Tokenizer& tokenizer;
    Token currentToken;

    void consume(TokenType type) {
        if (currentToken.type == type) {
            currentToken = tokenizer.getNextToken();
        } else {
            throw std::runtime_error("Unexpected token");
        }
    }

    double parseExpression() {
        double result = parseTerm();
        if (currentToken.type == PLUS) {
            consume(PLUS);
            return result + parseExpression();
        } else if (currentToken.type == MINUS) {
            consume(MINUS);
            return result - parseExpression();
        } else if (currentToken.type == MULTIPLY) {
            consume(MULTIPLY);
            return result * parseExpression();
        } else if (currentToken.type == DIVIDE) {
            consume(DIVIDE);
            return result / parseExpression();
        }
        return result;
    }

    double parseTerm() {
        if (currentToken.type == NUMBER) {
            double value = currentToken.value;
            consume(NUMBER);
            return value;
        }
        if (currentToken.type == LPAREN) {
            consume(LPAREN);
            double result = parseExpression();
            consume(RPAREN);
            return result;
        }
        if (currentToken.type == MINUS) {
            consume(MINUS);
            double result = currentToken.value;
            consume(NUMBER);
            return -result;
        }
        throw std::runtime_error("invalid syntax");
    }

};

// Helper function to evaluate expressions
double evaluate(const std::string& expression) {
    Tokenizer tokenizer(expression);
    Parser parser(tokenizer);
    return parser.parse();
}

// Main function to test the parser
int main() {
    std::string expression;
    std::cout << "Enter an expression: ";
    std::getline(std::cin, expression);
    
    try {
        double result = evaluate(expression);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
