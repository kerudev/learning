use std::error::Error;
use std::{env, fs};
use std::str::FromStr;

use serde::{Serialize, Deserialize};
use time::Date;
use time::macros::date;

#[derive(Debug, Default, Serialize, Deserialize)]
pub enum Priority {
    High,
    #[default]
    Med,
    Low,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct Attachment {
    size: u64,
    height: u64,
    width: u64,
}

#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)] 
#[serde(default)]
#[serde(rename_all = "camelCase")]
pub struct Task {
    id: u64,

    #[serde(alias = "description")]
    title: String,

    #[serde(alias = "deadline")]
    end_date: Date,

    priority: Priority,

    #[serde(flatten)]
    attachment: Option<Attachment>,
}

impl Default for Task {
    fn default() -> Self {
        return Self {
            id: 0,
            title: "Unnamed".into(),
            end_date: date!(1970-01-01),
            priority: Priority::Med,
            attachment: None,    
        }
    }
}

pub enum Action {
    Ser,
    De,
}

impl FromStr for Action {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "ser" => Ok(Action::Ser),
            "de" => Ok(Action::De),
            _ => Err(())
        }
    }
}

impl Action {
    fn to_list() -> Vec<String> {
        vec!["ser".into(), "de".into()]
    }
}

fn serialize() -> Result<(), Box<dyn Error>> {
    let tasks = vec![
        Task {
            id: 0,
            title: "Cook nice food".into(),
            end_date: date!(2026-06-24),
            priority: Priority::Low,
            attachment: Some(Attachment {
                size: 200,
                height: 128,
                width: 320,
            }),
        },
        Task {
            id: 1,
            title: "Take care of plants".into(),
            end_date: date!(2026-06-01),
            priority: Priority::Med,
            attachment: Some(Attachment {
                size: 400,
                height: 400,
                width: 300,
            }),
        },
        Task {
            id: 2,
            title: "Finish reading book".into(),
            end_date: date!(2026-05-30),
            priority: Priority::High,
            attachment: None,
        },
    ];

    println!("============================================================");
    println!("> Serializing");
    println!("============================================================");

    println!();
    println!("- Before: {:?}", &tasks);
    println!();

    let serialized = serde_json::to_string(&tasks)?;
    println!("- After:  {:?}", serialized);
    println!();
    
    println!("============================================================");
    println!("> Writing to 'tasks-ser.json'");
    println!("============================================================");

    let file = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open("json/tasks-ser.json")?;

    serde_json::to_writer_pretty(file, &tasks)?;

    Ok(())
}

fn deserialize() -> Result<(), Box<dyn Error>> {
    println!("============================================================");
    println!("> Deserializing");
    println!("============================================================");
    println!();

    if fs::exists("json/tasks-ser.json")? {
        let content = fs::read_to_string("json/tasks-ser.json")?;
        let tasks: Vec<Task> = serde_json::from_str(content.trim())?;

        println!("- From 'tasks-ser.json': {:?}", tasks);
    } else {
        println!("- From 'tasks-ser.json': execute 'cargo run ser'");
    }

    let content = fs::read_to_string("json/tasks-de.json")?;
    let tasks: Vec<Task> = serde_json::from_str(content.trim())?;

    println!();
    println!("- From 'tasks-de.json': {:?}", tasks);

    Ok(())
}

fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("Provide one of the following: {:?}", Action::to_list());
    }

    match Action::from_str(&args[1]).unwrap() {
        Action::Ser => serialize()?,
        Action::De => deserialize()?,
    }

    Ok(())
}
