use std::fs;
use std::path::Path;

use indexmap::IndexMap;
use color_eyre::eyre::Result;
use crossterm::event::{self, KeyCode, KeyEventKind};
use ratatui::layout::{Constraint, Layout, Position};
use ratatui::style::{Color, Modifier, Style, Stylize};
use ratatui::text::{Line, Span, Text};
use ratatui::widgets::{Block, List, ListItem, Paragraph};
use ratatui::{DefaultTerminal, Frame};

// TODO display the selected ID moving

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
    cursor_x: usize,
    cursor_y: usize,
    selected: Option<usize>,
}

enum InputMode {
    Normal,
    Insert,
    Edit,
}

impl App {
    fn new() -> Self {
        Self {
            mode: InputMode::Normal,
            input: String::new(),
            messages: Self::load_messages().unwrap(),
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
                        KeyCode::Char('q') => return self.save_messages(),
                        KeyCode::Char('i') => self.mode = InputMode::Insert,
                        KeyCode::Char('e') => self.mode = InputMode::Edit,
                        _ => {}
                    },
                    InputMode::Insert if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => self.mode = InputMode::Normal,
                        KeyCode::Left => self.move_cursor_left(),
                        KeyCode::Right => self.move_cursor_right(),
                        KeyCode::Char(character) => self.add_char(character),
                        KeyCode::Backspace => self.delete_char(),
                        KeyCode::Enter => self.submit_message(),
                        _ => {},
                    },
                    InputMode::Edit if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => self.mode = InputMode::Normal,
                        KeyCode::Up => self.move_cursor_up(),
                        KeyCode::Down => self.move_cursor_down(),
                        KeyCode::Enter => self.change_message_pos(),
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
        self.cursor_y = self.clamp_y(self.cursor_y.saturating_sub(1));
    }

    fn move_cursor_down(&mut self) {
        self.cursor_y = self.clamp_y(self.cursor_y.saturating_add(1));
    }

    const fn reset_cursor(&mut self) {
        self.cursor_x = 0;
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
        self.input.clear();
        self.reset_cursor();
    }

    fn change_message_pos(&mut self) {
        let keys: Vec<&usize> = self.messages.keys().collect();
        let over = keys[self.cursor_y].clone();

        match self.selected {
            Some(key) => {
                // TODO change this function
                self.messages.move_index(key, over); 
                self.selected = None;
            },
            None => self.selected = Some(over),
        }
    }

    fn get_help_message<'a>(&self) -> (Vec<Span<'a>>, Style) {
        match self.mode {
            InputMode::Normal => (
                vec![
                    "(Normal) ".bold().white(),
                    "q".bold(),
                    ": quit. ".into(),
                    "i".bold(),
                    ": insert ".bold(),
                    "mode. ".into(),
                    "e".bold(),
                    ": edit ".bold(),
                    "mode.".into(),
                ],
                Style::default().add_modifier(Modifier::RAPID_BLINK),
            ),
            InputMode::Insert => (
                vec![
                    "(Insert) ".bold().green(),
                    "Esc".bold(),
                    ": switch to normal mode. ".into(),
                    "Enter".bold(),
                    ": save message.".into(),
                ],
                Style::default(),
            ),
            InputMode::Edit => (
                vec![
                    "( Edit ) ".bold().blue(),
                    "Esc".bold(),
                    ": switch to normal mode. ".into(),
                    "Enter".bold(),
                    ": save changes.".into(),
                ],
                Style::default(),
            ),
        }
    }

    fn render(&self, frame: &mut Frame) {
        let vertical = Layout::vertical([
            Constraint::Length(1),
            Constraint::Length(3),
            Constraint::Min(1),
        ]);

        let [help_area, input_area, messages_area] = vertical.areas(frame.area());

        let (msg, style) = self.get_help_message();

        let text = Text::from(Line::from(msg)).patch_style(style);
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
            InputMode::Edit => frame.set_cursor_position(Position::new(
                messages_area.x + 1,
                messages_area.y + self.cursor_y as u16 + 1,
            )),
            _ => {},
        }

        let message_list: Vec<ListItem> = self
            .messages
            .iter()
            .map(|(k, v)| {
                let content = Line::from(Span::raw(format!("{k}: {v}")));
                ListItem::new(content)
            })
            .collect();

        let title = if self.selected.is_some() { self.selected.unwrap().to_string() } else { "None".to_string() };

        let messages = List::new(message_list)
            .style(match self.mode {
                InputMode::Edit => Style::default().fg(Color::Blue),
                _ => Style::default().fg(Color::White),
            })
            .block(Block::bordered().title(title));
            // .block(Block::bordered().title("Messages"));

        frame.render_widget(messages, messages_area);
    }
}
