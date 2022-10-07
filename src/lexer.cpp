/**
 *  CPSC 323 Compilers and Languages
 *
 *  Dalton Caron, Teaching Associate
 *  dcaron@fullerton.edu, +1 949-616-2699
 *  Department of Computer Science
 */
#include <lexer.h>

#include <fstream>
#include <sstream>
#include <algorithm>

Scanner::Scanner(const std::string file_path)
: file_path(file_path), current_column(1), current_line(1),
    current_character(0) {
        std::ifstream infile(file_path);

        if (!infile.is_open()) {
            ERROR_LOG("Failed to open file %s", file_path.c_str());
            exit(EXIT_FAILURE);
        }

        std::stringstream buffer;
        buffer << infile.rdbuf();
        this->source = buffer.str();
    }

char Scanner::next() {
    if (this->current_character < this->source.length()) {
        const char next_character = this->source[this->current_character++];
        if (next_character == '\n') {
            this->current_line++;
            this->current_column = 1;
        } else {
            this->current_column++;
        }
        return next_character;
    } else {
        return 0;
    }
}

char Scanner::peek() const {
    if (this->current_character < this->source.length()) {
        return this->source[this->current_character];
    } else {
        return 0;
    }
}

ScanningTable::ScanningTable(const CsvReader &table) {
    // We read the table in column major order as to only call the
    // map_string_to_char function once per column iteration.
    int current_column = 1;
    while (current_column < table.getColumns()) {

        const char current_symbol =
            this->map_string_to_char(table.get(0, current_column));

        int current_row = 1;
        while (current_row < table.getRows()) {

            const state_t current_state =
                atoi(table.get(current_row, 0).c_str());

            // This models the transition function ((state, symbol), state).
            const std::string next_state = table.get(current_row, current_column);

            // The map is only populated if there is a valid transition.
            if (!next_state.empty()) {

                this->state_symbol_to_state[std::make_pair(
                    current_state, current_symbol
                )] = atoi(next_state.c_str());

            }

            current_row++;
        }

        current_column++;
    }
}

char ScanningTable::getNextState(const state_t state, const char symbol) {
    return this->state_symbol_to_state[std::make_pair(state, symbol)];
}

bool ScanningTable::containsNextState(const state_t state, const char symbol) {
    return this->state_symbol_to_state
        .count(std::make_pair(state, symbol)) > 0;
}

char ScanningTable::map_string_to_char(const std::string &str) const {
    // Map to special strings, otherwise take the first character.
    if (str == "0x0A" || str == "0x0a" || str == "\\n") {
        return '\n';
    } else if (str == "0x0D" || str == "0x0d" || str == "\\r") {
        return '\r';
    } else if (str == "0x20") {
        return ' ';
    } else if (str == "0x09" || str == "\\t") {
        return '\t';
    } else if (str == "comma" || str == "Comma") {
        return ',';
    } else {
        return str[0];
    }
}

TokenTable::TokenTable(const CsvReader &table) {
    // The token table has two columns:
    // 1. Accepting states and
    // 2. Token class numbers.

    // Read each row one by one.
    int row = 0;
    while (row < table.getRows()) {
        // Insert the state-token class pair into the token table map.
        const state_t final_state = atoi(table.get(row, 0).c_str());
        const token_class_t type_recognized =
            (token_class_t) atoi(table.get(row, 1).c_str());
        this->state_to_token_type[final_state] = type_recognized;
        row++;
    }
}

token_class_t TokenTable::getTokenTypeFromFinalState(
    const state_t final_state
) {
    return this->state_to_token_type[final_state];
}

bool TokenTable::isStateFinal(const state_t state) const {
    return this->state_to_token_type.count(state) > 0;
}

Lexer::Lexer()
: scanning_table(ScanningTable(CsvReader(this->scanning_table_path, ','))),
    token_table(TokenTable(CsvReader(this->token_table_path, ',')))
{}

Lexer::~Lexer() {}

token_stream_t Lexer::lex(const std::string &file_path) {
    token_stream_t token_stream;
    Scanner scanner = Scanner(file_path);
    token_t token;

    // While the scannr contains input, lex token by token.
    while (scanner.peek() != 0) {
        token = this->scan_token(scanner);
        // Ignore all whitespace.
        if (token.type != WHITESPACE) {
            token_stream.push_back(token);
        }
    }

    // Add a token to denoate the end of the file.
    token.lexeme = "";
    token.type = END_OF_FILE;

    token_stream.push_back(token);

    return token_stream;
}

token_t Lexer::scan_token(Scanner &scanner) {

    // VARIABLES //

    ifstream file_path = scanner.getFilePath(); // Retrieving data from scanning_table.csv [scanning_table] (Token table is token_table.csv [token_table])
    int column = scanner.getColumn(), // The current location of the column being read in file
        line = scanner.getLine(); // The current location of the line being read in file
	state_t state = 1; // The current state of the machine; the state is 1 by default
	char cc; // Current character

	token_t tok; // Token {token_class_t type, string lexeme, string file, int line, int column}
    token_class_t tokenType = ""; // Symbol

    // Start scanning loop
    while ( scanner.peek() != EOF ) {
        cc = scanner.peek(); // Sets the latest character retrieved from scanner
        tokenType = scanning_table(state, cc); // MOVE, RECOGNIZE, or ERROR depending on scanning_table.get(state, cc);

        // If an identifier is present in the table, then the program will check which identifier that character is

        // Scanner moves to next line
        if ( scanning_table.map_string_to_char(cc) == "\n" ) {
            scanner.next();
            tok += cc;
            state = scanning_table.get(state, cc);
            continue;
        }
        // Program recognizes that there is an identifier present
        else if ( tokenType == IDENTIFIER ) {
            tokenType = token_table.get(state);

            // Checks if there is the identifier is a valid keyword
            switch( tokenType )
            {
                input = true;

                case CONST_KEYWORD: // 2
                    cout << "const";
                    break;
                case NUMBER_LITERAL:
                    cout << cc;
                    break;
                case COMMA: // 3
                    cout << scanning_table.map_string_to_char(cc);
                    break;
                case DEFINE_EQUALS: // 4
                    cout << "==";
                    break;
                case EQUALS: // 5
                    cout << "=";
                    break;
                case LEFT_PAREN: // 6
                    cout << "(";
                    break;
                case RIGHT_PAREN: // 7
                    cout << ")";
                    break;
                case ODD_OP: // 8
                    cout << "%";
                    break;
                case END_KEYWORD: // 9
                    cout << "end";
                    break;
                case BEGIN_KEYWORD: // 10
                    cout << "begin";
                    break;
                case CALL_KEYWORD: // 11
                    cout << "call";
                    break;
                case SEMICOLON: // 12
                    cout << ";";
                    break;
                case PROCEDURE_KEYWORD: // 13
                    cout << "procedure";
                    break;
                case IF_KEYWORD: // 14
                    cout << "if";
                    break;
                case THEN_KEYWORD: // 15
                    cout << "then";
                case WHILE_KEYWORD: // 17
                    cout << "while";
                    break;
                case DO_KEYWORD: // 18
                    cout << "if";
                    break;
                case ADD_OP: // 19
                    cout << "+";
                    break;
                case MUL_OP: // 20
                    cout << "*";
                    break;
                case COMPARE_OP: // 21

                    // "==" is not included in COMPARE_OP as it is already defined as its own individual value, DEFINE_EQUALS

                    if (cc == "<") {
                        cout << "<";
                    }
                    else if (cc == ">") {
                        cout << ">";
                    }
                    else if (cc == "<=")
                    {
                        cout << "<=";
                    }
                    else if (cc == ">=") {
                        cout << ">=";
                    }
                    else if (cc == "!=") {
                        cout << "!=";
                    }
                    break;
                case VAR_KEYWORD: // 22
                    cout << "var";
                    break;
                case WRITE_OP: // 23
                    cout << "<<";
                    break;
                case READ_OP: // 24
                    cout << ">>";
                    break;
                case PERIOD: // 25
                    cout << ".";
                    break;
                case END_OF_FILE: // 26
                    cout << "[end of file]";
                    break;
                case WHITESPACE: // 27
                    cout << scanning_table.map_string_to_char(cc);
                    break;
                }
            }

        // Displays an error message if detected
        else if (tokenType == ERROR) {
            throw "[ERROR] Invalid token.\nLocation: [LINE: " << line << ", COLUMN: " << column << "]" << endl;
        }
    } // End of scanning loop

    file_path.close();

    return tok; // {token, tokenType, file_path, column, line}
}
