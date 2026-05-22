/**
 * Idiomatic Rust error handling:
 * 1) Create an error enum
 * 2) Group errors inside enums that make sense (for example, IO errors are
 *    separated from Int errors, because that makes sense).
 * 3) Don't return other libraries' errors so
 *    a) The external library can't break your public API.
 *    b) You can change your own implementation without breaking the API.
 *    An exception is made with the standard library, as it makes sense that
 *    the user might want to handle IO errors or similar.
 * 4) Make enum non-exhaustive (the compiler will tell you to handle new
 *    possible variants with a wildcard).
 *
 * 5a) Implement Debug + Display + Error
 */

use std::error::Error;
use std::fmt::{Display, Formatter};

#[derive(Debug)]
#[non_exhaustive]
pub enum SandwichError {
    NoBread,
    Empty,
    TooBig(usize),
}

impl Display for SandwichError {
    fn fmt(&self, f: &mut Formatter) -> std::fmt::Result {
        use SandwichError::*;

        match self {
            NoBread => write!(f, "I forgot the bread!"),
            Empty => write!(f, "I forgot to put something between the bread!"),
            TooBig(n) => write!(f, "Can't fit the sandwich in my mouth! It's floors {} high", n),
        }
    }
}

impl Error for SandwichError {} // 5) Implement Error (it can' be derived)

pub struct Sandwich {
    pub ingredients: Vec<String>,
}

impl ToString for Sandwich {
    fn to_string(&self) -> String {
        format!("{}", self.ingredients.join(", "))
    }
}

impl Sandwich {
    pub fn eat(&self) -> Result<(), SandwichError> {
        let height = self.ingredients.len();
        let has_bread = self.ingredients.contains(&"bread".to_string());
        let is_empty = has_bread && height == 1;

        println!("> Eating a sandwich ({})", self.to_string());

        if !has_bread {
            Err(SandwichError::NoBread)
        } else if is_empty {
            Err(SandwichError::Empty)
        } else if height > 5 {
            Err(SandwichError::TooBig(height))
        } else {
            println!("  - This sandwich deserves a Michelin-star!");
            Ok(())
        }
    }
}
