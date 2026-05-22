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
 * 5b) Use thiserror to reduce steps.
 */

use anyhow::Result;
use thiserror::Error;

#[non_exhaustive]
#[derive(Debug, Error)]
pub enum SongError {
    #[error("Music is too low ({0}%). Turn it up a little")]
    TooLow(u16),
    #[error("Music is too high ({0}%). Turn it down!")]
    TooHigh(u16),
    #[error("'{0}' is not my type of beat *skips song*")]
    NotMyType(String),
}

pub struct Song {
    pub name: String,
    pub genre: String,
    pub volume: u16,
}

impl ToString for Song {
    fn to_string(&self) -> String {
        format!("{} (Genre: {} - Volume: {}%)", self.name, self.genre, self.volume)
    }
}

impl Song {
    pub fn play(&self) -> Result<(), SongError> {
        println!("> Now playing: {}", self.to_string());

        if self.volume < 20 {
            Err(SongError::TooLow(self.volume))
        } else if self.volume > 80 {
            Err(SongError::TooHigh(self.volume))
        } else if self.genre != "rock" {
            Err(SongError::NotMyType(self.name.clone()))
        } else {
            println!("  - This song hits the spot!");
            Ok(())
        }
    }
}
