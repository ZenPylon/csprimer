use std::collections::HashMap;
use std::fs;
use std::io;

struct TextFileChar {
    line_number: usize,
    col_number: usize,
    char: char,
}

fn main() -> io::Result<()> {
    let open_close_map: HashMap<char, char> = HashMap::from([('(', ')'), ('{', '}'), ('[', ']')]);
    let close_open_map: HashMap<char, char> = HashMap::from([(')', '('), ('}', '{'), (']', '[')]);
    let mut syntax_stack: Vec<TextFileChar> = Vec::new();

    let contents = fs::read_to_string("./stretch.rkt")?;
    let mut char_idx = 1;
    let mut line_idx = 1;

    for char_ in contents.chars() {
        if char_ == '\n' {
            char_idx = 1;
            line_idx += 1;
            continue;
        }

        if open_close_map.contains_key(&char_) {
            syntax_stack.push(TextFileChar {
                line_number: line_idx,
                col_number: char_idx,
                char: char_,
            });
        } else if close_open_map.contains_key(&char_) {
            let maybe_stack_top = syntax_stack.pop();

            match maybe_stack_top {
                None => {
                    println!("Unmatched '{char_}': Line {line_idx}, column {char_idx}.")
                }
                Some(stack_top) => {
                    let matching_open = *close_open_map.get(&char_).unwrap();
                    if matching_open != stack_top.char {
                        println!("Syntax error: Line {line_idx}, column {char_idx}.");
                        println!(
                            "Improperly closed '{}' from: Line {}, column {}.\n",
                            stack_top.char, stack_top.line_number, stack_top.col_number
                        );
                    }
                }
            }
        }

        char_idx += 1;
    }

    for unclosed in syntax_stack {
        println!(
            "Unmatched '{}': Line {}, column {}.",
            unclosed.char, unclosed.line_number, unclosed.col_number
        );
    }

    Ok(())

    // If end of file, ensure stack is empty
}
