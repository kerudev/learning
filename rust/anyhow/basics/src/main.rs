// Next: errors/sandwich.rs

use anyhow::Result;
use basics::{Sandwich, Song};

fn main() -> Result<()> {
    let sandwiches = vec![
        Sandwich {
            ingredients: vec![
                String::from("bread")
            ],
        },
        Sandwich {
            ingredients: vec![
                String::from("tomato"),
                String::from("lettuce"),
                String::from("bacon"),
            ],
        },
        Sandwich {
            ingredients: vec![
                String::from("bread"),
                String::from("mayo"),
                String::from("mustard"),
                String::from("tomato"),
                String::from("lettuce"),
                String::from("bacon"),
                String::from("bread"),
            ],
        },
        Sandwich {
            ingredients: vec![
                String::from("bread"),
                String::from("tomato"),
                String::from("lettuce"),
                String::from("bacon"),
                String::from("bread"),
            ],
        },
    ];

    let songs = vec![
        Song {
            name: String::from("Coldplay - Yellow"),
            genre: String::from("pop"),
            volume: 50,
        },
        Song {
            name: String::from("Avril Lavigne - Complicated"),
            genre: String::from("pop rock"),
            volume: 120,
        },
        Song {
            name: String::from("Black Sabbath - War Pigs"),
            genre: String::from("heavy metal"),
            volume: 10,
        },
        Song {
            name: String::from("RHCP - Around the world"),
            genre: String::from("rock"),
            volume: 60,
        },
    ];

    println!(
        "You prepared some sandwiches and songs to set the mood this afternoon.\n\
        Maybe it takes some tries for you to find the right setting...\n"
    );

    let song_idx = rand::random_range(0..songs.len());
    let sandwich_idx = rand::random_range(0..sandwiches.len());

    println!("Playing song #{} and eating sandwich #{}\n", song_idx, sandwich_idx);
    
    songs[song_idx].play()?;
    sandwiches[sandwich_idx].eat()?;

    Ok(())
}
