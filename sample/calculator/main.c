#include "../../include/init.c"
#include "../../include/input.c"

#define BUFFER_LEN 128

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

Ck_ValidationRes arithmetic_validator(char* input) {
    size_t cursor = 0;
    size_t parentheses_opened_count = 0;
    size_t parentheses_closed_count = 0;
    bool is_within_dot = 0;

    while (input[cursor] != 0) {
        char ch = input[cursor];
        char ch_next = input[cursor + 1];
        char ch_prev = input[cursor - 1];
        bool is_end = ch_next == 0;

        /** Skip white spaces */
        if (ch == ' ' || ch == '\t') {
            cursor++;
            continue;
        }
        if (!(is_digit(ch) || is_operator(ch) || ch == '.' || ch == '(' || ch == ')')) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if (cursor == 0 && ch != '-' && is_operator(ch)) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if (is_operator(ch) && is_operator(ch_next)) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if ((ch == '.' && ch_next == '.') || (ch == '.' && is_within_dot)) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        } else if (ch == '.' && !is_within_dot) {
            is_within_dot = true;
        }

        if (ch == ')' && parentheses_opened_count == parentheses_closed_count) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        } else if (ch == ')') {
            parentheses_closed_count++;
        }

        if ((is_digit(ch) && ch_next == '(') || (ch == '(' && is_operator(ch_next))) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        } else if (ch == '(') {
            parentheses_opened_count++;
        }

        if (is_operator(ch) && ch_next == ')') {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if (ch == '(' && is_operator(ch_next)) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if (is_operator(ch) && is_end) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_WARN,
                .message = NULL,
            };
        }

        if (ch == ')' && is_digit(ch_next)) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_ERROR,
                .message = NULL,
            };
        }

        if (is_operator(ch) && is_end) {
            return (Ck_ValidationRes){
                .status = CK_VALIDATION_WARN,
                .message = NULL,
            };
        }

        if (is_operator(ch)) {
            is_within_dot = false;
        }

        cursor++;
    }

    if (parentheses_opened_count != parentheses_closed_count) {
        return (Ck_ValidationRes){
            .status = CK_VALIDATION_WARN,
            .message = NULL,
        };
    }

    if (cursor == 0) {
        return (Ck_ValidationRes){
            .status = CK_VALIDATION_ERROR,
            .message = NULL,
        };
    }

    return (Ck_ValidationRes){
        .status = CK_VALIDATION_OK,
        .message = NULL,
    };
}

int main(void) {
    char buffer[BUFFER_LEN] = {0};

    ck_input(
        buffer,
        BUFFER_LEN,
        (Ck_InputParams){
            .prompt = "[INPUT]: ",
            .validator = arithmetic_validator,
            .on_after_prompt = NULL,
            .on_before_prompt = NULL,
            .use_max_length = false,
            .skip_end_new_line = false,
        }

    );

    return 0;
}