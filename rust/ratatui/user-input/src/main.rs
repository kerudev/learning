use std::fs;
use std::path::Path;

use indexmap::IndexMap;
use color_eyre::eyre::Result;
use crossterm::event::{self, KeyCode, KeyEventKind};
use ratatui::layout::{Constraint, Layout, Position};
use ratatui::style::{Color, Style, Stylize};
use ratatui::text::{Line, Span, Text};
use ratatui::widgets::{Block, List, ListItem, Paragraph};
use ratatui::{DefaultTerminal, Frame};


fn main() -> Result<()> {
    color_eyre::install()?;
    let terminal = ratatui::init();
    let result = App::new().run(terminal);
    ratatui::restore();
    result
}

struct App {
    mode: InputMode,
    input: String,
    messages: IndexMap<usize, String>,
    prev_messages: IndexMap<usize, String>,
    cursor_x: usize,
    cursor_y: usize,
    selected: Option<usize>,
}

enum InputMode {
    Normal,
    Insert,
    Move,
}

impl App {
    fn new() -> Self {
        Self {
            mode: InputMode::Normal,
            input: String::new(),
            messages: Self::load_messages().unwrap(),
            prev_messages: IndexMap::new(),
            cursor_x: 0,
            cursor_y: 0,
            selected: None,
        }
    }

    fn save_messages(&self) -> Result<()> {
        let file = fs::OpenOptions::new()
            .read(true)
            .write(true)
            .create(true)
            .truncate(true)
            .open("messages.json")?;

        serde_json::to_writer_pretty(file, &self.messages)?;

        Ok(())
    }

    fn load_messages() -> Result<IndexMap<usize, String>> {
        let path = Path::new("messages.json");

        if !path.exists() { return Ok(IndexMap::new()); }

        let content = fs::read_to_string(path)?;
        let messages = serde_json::from_str(content.trim())?;

        Ok(messages)
    }

    fn run(mut self, mut terminal: DefaultTerminal) -> Result<()> {
        loop {
            terminal.draw(|frame| self.render(frame))?;

            if let Some(key) = event::read()?.as_key_press_event() {
                match self.mode {
                    InputMode::Normal => match key.code {
                        KeyCode::Esc => return self.save_messages(),
                        KeyCode::Char('i') => self.mode = InputMode::Insert,
                        KeyCode::Char('m') => self.mode = InputMode::Move,
                        _ => {}
                    },
                    InputMode::Insert if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => {
                            self.mode = InputMode::Normal;
                            self.reset_insert();
                        },
                        KeyCode::Left => self.move_cursor_left(),
                        KeyCode::Right => self.move_cursor_right(),
                        KeyCode::Char(character) => self.add_char(character),
                        KeyCode::Backspace => self.delete_char(),
                        KeyCode::Enter => self.submit_message(),
                        _ => {},
                    },
                    InputMode::Move if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => {
                            self.mode = InputMode::Normal;
                            self.reset_move();
                        },
                        KeyCode::Up => self.move_cursor_up(),
                        KeyCode::Down => self.move_cursor_down(),
                        KeyCode::Enter => self.change_selected(),
                        _ => {},
                    },
                    _ => {},
                }
            }
        }
    }

    fn clamp_x(&self, new_x: usize) -> usize {
        new_x.clamp(0, self.input.chars().count())
    }

    fn clamp_y(&self, new_y: usize) -> usize {
        new_y.clamp(0, self.messages.len() - 1)
    }

    fn move_cursor_left(&mut self) {
        self.cursor_x = self.clamp_x(self.cursor_x.saturating_sub(1));
    }

    fn move_cursor_right(&mut self) {
        self.cursor_x = self.clamp_x(self.cursor_x.saturating_add(1));
    }

    fn move_cursor_up(&mut self) {
        let new_y = self.cursor_y.saturating_sub(1);

        if self.selected.is_some() {
            self.messages.move_index(self.cursor_y, new_y);
            self.selected = Some(self.cursor_y);
        }

        self.cursor_y = self.clamp_y(new_y);
    }

    fn move_cursor_down(&mut self) {
        let new_y = self.cursor_y.saturating_add(1);

        if self.selected.is_some() && new_y < self.messages.len() {
            self.messages.move_index(self.cursor_y, new_y);
            self.selected = Some(self.cursor_y);
        }

        self.cursor_y = self.clamp_y(new_y);
    }

    fn reset_insert(&mut self) {
        self.input.clear();
        self.cursor_x = 0;
    }

    fn reset_move(&mut self) {
        self.messages = self.prev_messages.clone();
        self.prev_messages.clear();
        self.selected = None;
        self.cursor_y = 0;
    }

    fn byte_index(&self) -> usize {
        self.input
            .char_indices()
            .map(|(i, _)| i)
            .nth(self.cursor_x)
            .unwrap_or(self.input.len())
    }

    fn add_char(&mut self, character: char) {
        let index = self.byte_index();
        self.input.insert(index, character);
        self.move_cursor_right();
    }

    fn delete_char(&mut self) {
        if self.cursor_x == 0 { return; }

        let before_char_to_delete = self.input.chars().take(self.cursor_x - 1);
        let after_char_to_delete = self.input.chars().skip(self.cursor_x);

        self.input = before_char_to_delete.chain(after_char_to_delete).collect();
        self.move_cursor_left();
    }

    fn submit_message(&mut self) {
        if self.input.len() == 0 { return; }

        self.messages.insert(self.messages.keys().len(), self.input.clone());
        self.reset_insert();
    }

    fn change_selected(&mut self) {
        match self.selected {
            Some(_) => {
                self.prev_messages.clear();
                self.selected = None;
            },
            None => {
                self.prev_messages = self.messages.clone();
                self.selected = Some(self.cursor_y);
            },
        }
    }

    fn get_help_message<'a>(&self) -> Vec<Span<'a>> {
        match self.mode {
            InputMode::Normal => vec![
                "(Normal) ".bold(),
                "Esc".bold(),
                ": exit. ".into(),
                "i".bold(),
                ": insert ".bold(),
                "mode. ".into(),
                "m".bold(),
                ": move ".bold(),
                "mode.".into(),
            ],
            InputMode::Insert => vec![
                "(Insert) ".bold().green(),
                "Esc".bold().green(),
                ": switch to normal mode. ".into(),
                "Enter".bold().green(),
                ": save message.".into(),
            ],
            InputMode::Move => vec![
                "( Move ) ".bold().blue(),
                "Esc".bold().blue(),
                ": switch to normal mode. ".into(),
                "Enter".bold().blue(),
                ": select or unselect.".into(),
            ],
        }
    }

    fn render(&self, frame: &mut Frame) {
        let vertical = Layout::vertical([
            Constraint::Length(1),
            Constraint::Length(3),
            Constraint::Min(1),
        ]);

        let [help_area, input_area, messages_area] = vertical.areas(frame.area());

        let text = Text::from(Line::from(self.get_help_message()));
        let help_message = Paragraph::new(text);
        frame.render_widget(help_message, help_area);

        let input_text = if self.input.len() == 0 {
            Span::styled("Type your message here...", Style::default().gray())
        } else {
            Span::raw(self.input.as_str())
        };

        let input = Paragraph::new(input_text)
            .block(Block::bordered().title("Input")
                .border_style(match self.mode {
                    InputMode::Insert => Style::default().fg(Color::Green),
                    _ => Style::default().fg(Color::White),
                }
            ));

        frame.render_widget(input, input_area);

        match self.mode {
            InputMode::Insert => frame.set_cursor_position(Position::new(
                input_area.x + self.cursor_x as u16 + 1,
                input_area.y + 1,
            )),
            InputMode::Move => frame.set_cursor_position(Position::new(
                messages_area.x + 1,
                messages_area.y + self.cursor_y as u16 + 1,
            )),
            _ => {},
        }

        let message_list: Vec<ListItem> = self
            .messages
            .iter()
            .enumerate()
            .map(|(pos, (k, v))| {
                let style = if self.selected.is_some() && pos == self.cursor_y {
                    Style::default().bg(Color::White)
                } else {
                    Style::default()
                };

                let content = Line::from(Span::raw(format!("{k}: {v}"))).style(style);

                ListItem::new(content)
            })
            .collect();

        let messages = List::new(message_list)
            .style(match self.mode {
                InputMode::Move => Style::default().fg(Color::Blue),
                _ => Style::default().fg(Color::White),
            })
            .block(Block::bordered().title("Messages"));

        frame.render_widget(messages, messages_area);
    }
}
