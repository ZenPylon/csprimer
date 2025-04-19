pub mod basic_calculator {
    use std::collections::{HashMap, HashSet};

    pub const SYNTAX_ERROR_MSG: &str = "Invalid input: syntax error";
    pub const EMPTY_EXPR_MSG: &str = "Invalid input: empty expression";

    pub enum ExpressionType {
        Literal,
        UnaryOpPrefix,
        BinaryOpInfix,
        StarOp,
        LParen,
    }

    pub struct ParseFrame {
        chars: Vec<char>,
        expr_type: ExpressionType,
    }

    pub fn eval_string_expr<T>(input: &str) -> Result<T, &'static str> {
        if input.is_empty() {
            return Err(EMPTY_EXPR_MSG);
        }

        let literals: Vec<char> = vec!['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];
        let mut parse_stack: Vec<ParseFrame> = Vec::new();

        for input_char in input.chars() {
            if parse_stack.is_empty() {
                let expr_type: ExpressionType;
                let mut chars = vec![input_char];

                if literals.contains(&input_char) {
                    expr_type = ExpressionType::Literal;
                } else if input_char == '-' {
                    expr_type = ExpressionType::UnaryOpPrefix;
                } else if input_char == '(' {
                    expr_type = ExpressionType::LParen
                } else {
                    return Err(SYNTAX_ERROR_MSG);
                }
                parse_stack.push(ParseFrame { chars, expr_type });
                continue;
            }
            let top_index = parse_stack.len() - 1;
            let mut stack_top = parse_stack[top_index];

            match stack_top.expr_type {
                ExpressionType::LParen => {
                    let expr_type: ExpressionType;

                    // Empty parenthesis and binary operators are invalid
                    if literals.contains(&input_char) {
                        expr_type = ExpressionType::Literal;
                    } else if input_char == '-' {
                        expr_type = ExpressionType::UnaryOpPrefix;
                    } else if input_char == '(' {
                        expr_type = ExpressionType::LParen
                    }
                    parse_stack.push(ParseFrame {
                        chars: vec![input_char],
                        expr_type,
                    });
                }
                ExpressionType::UnaryOpPrefix => {
                    if !literals.contains(&input_char) {
                        return Err(SYNTAX_ERROR_MSG);
                    }
                    parse_stack.push(ParseFrame {
                        chars: vec![input_char],
                        expr_type: ExpressionType::Literal,
                    });
                }
                ExpressionType::Literal => {
                    if literals.contains(&input_char) {
                        // This is the next digit in a series of digits
                        stack_top.chars.push(input_char);
                    } else if input_char == ')' {
                        // Pop the stack until we find the matching '(' character
                        // Pop the ')' character
                        parse_stack.pop();
                        while !parse_stack.is_empty() {
                            let frame = parse_stack.pop();
                            
                        }
                    }
                }
            }
        }
        Ok(0)
    }
}

#[cfg(test)]
mod test {
    use super::basic_calculator::*;

    // Each character in an input string belongs to zero or more valid token classes.
    // For example, "1" is valid for literal values, * is valid for the multiplication operator
    // AND the exponentiation operator "**", and so on.
    //
    // As we iterate through the expression string, we check if each character is a
    // valid member of the current token type. If not, that either means
    //
    // 1. The hypothesis for the current token type is incorrect (it's a different type)
    // 2. The current token has ended, and this character is a new token
    // 3. The current token is invalid in this context i.e. it's a syntax error

    #[test]
    pub fn test_expr_eval() {
        // Invalid expressions
        assert_eq!(EMPTY_EXPR_MSG, eval_string_expr("").unwrap_err());
        assert_eq!(EMPTY_EXPR_MSG, eval_string_expr("()").unwrap_err());
        assert_eq!(SYNTAX_ERROR_MSG, eval_string_expr("17 +").unwrap_err());
        assert_eq!(SYNTAX_ERROR_MSG, eval_string_expr("+  17").unwrap_err());
        assert_eq!(SYNTAX_ERROR_MSG, eval_string_expr("*  17").unwrap_err());
        assert_eq!(SYNTAX_ERROR_MSG, eval_string_expr("+").unwrap_err());
        assert_eq!(SYNTAX_ERROR_MSG, eval_string_expr("(1+1)+1)").unwrap_err());

        // Unnested expressions
        //
        // Infix
        assert_eq!(11, eval_string_expr("10 + 1").unwrap());
        assert_eq!(0, eval_string_expr("001-1").unwrap());
        assert_eq!(42, eval_string_expr("21*2").unwrap());
        assert_eq!(2, eval_string_expr("8 / 4").unwrap());
        assert_eq!(1024, eval_string_expr("2 ** 10").unwrap());

        // Prefix
        assert_eq!(-300000, eval_string_expr("-300000").unwrap());

        // Postfix
        assert_eq!(120, eval_string_expr("5!").unwrap());

        // Nested expressions
        //
    }
}
