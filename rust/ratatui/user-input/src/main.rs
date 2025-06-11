use std::fmt;

use color_eyre::eyre::Result;
use crossterm::event::{self, KeyCode, KeyEventKind};
use ratatui::layout::{Constraint, Layout, Position};
use ratatui::style::{Color, Modifier, Style, Stylize};
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
    cursor_index: usize,
    messages: Vec<String>,
}

enum InputMode {
    Normal,
    Insert
}

impl fmt::Display for InputMode {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match *self {
            Self::Normal => write!(f, "Normal"),
            Self::Insert => write!(f, "Insert"),
        }
    }
}

impl App {
    fn new() -> Self {
        Self {
            mode: InputMode::Normal,
            input: String::new(),
            cursor_index: 0,
            messages: Vec::new(),
        }
    }

    fn run(mut self, mut terminal: DefaultTerminal) -> Result<()> {
        loop {
            terminal.draw(|frame| self.render(frame))?;

            if let Some(key) = event::read()?.as_key_press_event() {
                match self.mode {
                    InputMode::Normal => match key.code {
                        KeyCode::Char('i') => self.mode = InputMode::Insert,
                        KeyCode::Char('q') => return Ok(()),
                        _ => {}
                    },
                    InputMode::Insert if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Char(character) => self.add_char(character),
                        KeyCode::Backspace => self.delete_char(),
                        KeyCode::Enter => self.submit(),
                        KeyCode::Left => self.move_cursor_left(),
                        KeyCode::Right => self.move_cursor_right(),
                        KeyCode::Esc => self.mode = InputMode::Normal,
                        _ => {},
                    },
                    InputMode::Insert => {},
                }
            }
        }
    }

    fn clamp_cursor(&self, new_cursor_pos: usize) -> usize {
        new_cursor_pos.clamp(0, self.input.chars().count())
    }

    fn move_cursor_left(&mut self) {
        let cursor_moved_left = self.cursor_index.saturating_sub(1);
        self.cursor_index = self.clamp_cursor(cursor_moved_left);
    }

    fn move_cursor_right(&mut self) {
        let cursor_moved_right = self.cursor_index.saturating_add(1);
        self.cursor_index = self.clamp_cursor(cursor_moved_right);
    }

    const fn reset_cursor(&mut self) {
        self.cursor_index = 0;
    }

    fn byte_index(&self) -> usize {
        self.input
            .char_indices()
            .map(|(i, _)| i)
            .nth(self.cursor_index)
            .unwrap_or(self.input.len())
    }

    fn add_char(&mut self, character: char) {
        let index = self.byte_index();
        self.input.insert(index, character);
        self.move_cursor_right();
    }

    fn delete_char(&mut self) {
        if self.cursor_index == 0 { return; }

        let before_char_to_delete = self.input.chars().take(self.cursor_index - 1);
        let after_char_to_delete = self.input.chars().skip(self.cursor_index);

        self.input = before_char_to_delete.chain(after_char_to_delete).collect();
        self.move_cursor_left();
    }

    fn submit(&mut self) {
        self.messages.push(self.input.clone());
        self.input.clear();
        self.reset_cursor();
    }

    fn get_help_message<'a>(&self) -> (Vec<Span<'a>>, Style) {
        match self.mode {
            InputMode::Normal => (
                vec![
                    format!("({}) ", self.mode.to_string()).bold(),
                    "Press ".into(),
                    "q".bold(),
                    " to exit, ".into(),
                    "i".bold(),
                    " to change to ".into(),
                    "insert".bold(),
                    " mode.".into(),
                ],
                Style::default().add_modifier(Modifier::RAPID_BLINK),
            ),
            InputMode::Insert => (
                vec![
                    format!("({}) ", self.mode.to_string()).bold(),
                    "Press ".into(),
                    "Esc".bold(),
                    " to stop editing, ".into(),
                    "Enter".bold(),
                    " to record the message".into(),
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

        let input = Paragraph::new(self.input.as_str())
            .style(match self.mode {
                InputMode::Normal => Style::default().fg(Color::White),
                InputMode::Insert => Style::default().fg(Color::Green),
            })
            .block(Block::bordered().title("Input"));
        frame.render_widget(input, input_area);

        match self.mode {
            InputMode::Normal => {}
            InputMode::Insert => frame.set_cursor_position(Position::new(
                input_area.x + self.cursor_index as u16 + 1,
                input_area.y + 1,
            )),
        }

        let message_list: Vec<ListItem> = self
            .messages
            .iter()
            .enumerate()
            .map(|(i, m)| {
                let content = Line::from(Span::raw(format!("{i}: {m}")));
                ListItem::new(content)
            })
            .collect();

        let messages = List::new(message_list).block(Block::bordered().title("Messages"));
        frame.render_widget(messages, messages_area);
    }
}
