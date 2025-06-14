use std::fs;
use std::path::Path;

use color_eyre::eyre::Result;
use crossterm::event::{self, KeyCode, KeyEventKind};
use indexmap::IndexMap;
use ratatui::layout::{Alignment, Constraint, Flex, Layout, Position};
use ratatui::style::{Color, Style, Stylize as _};
use ratatui::text::{Line, Span, Text};
use ratatui::widgets::{Block, Clear, List, ListItem, Paragraph, Wrap};
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
    min_x: usize,
    max_x: usize,
    min_y: usize,
    max_y: usize,
    cursor_x: usize,
    cursor_y: usize,
    selected: Option<usize>,
    show_delete_popup: bool,
}

enum InputMode {
    Normal,
    Insert,
    Move,
    Edit,
}

impl App {
    fn new() -> Self {
        Self {
            mode: InputMode::Normal,
            input: String::new(),
            messages: Self::load_messages().unwrap(),
            prev_messages: IndexMap::new(),
            min_x: 0,
            max_x: 0,
            min_y: 0,
            max_y: 0,
            cursor_x: 0,
            cursor_y: 0,
            selected: None,
            show_delete_popup: false,
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

        if !path.exists() {
            return Ok(IndexMap::new());
        }

        let content = fs::read_to_string(path)?;
        let messages = serde_json::from_str(content.trim())?;

        Ok(messages)
    }

    fn clamp_x(&self, new_x: usize) -> usize {
        new_x.clamp(self.min_x, self.max_x)
    }

    fn clamp_y(&self, new_y: usize) -> usize {
        new_y.clamp(self.min_y, self.max_y)
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
        self.prev_messages.clear();
        self.selected = None;
        self.cursor_y = 0;
    }

    fn calc_cursor_min_x(&self) -> usize {
        let keys: Vec<&usize> = self.messages.keys().collect();
        let key = *keys[self.cursor_y];

        key.to_string().len() + 2
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

        self.min_x = 0;
        self.max_x = self.input.chars().count();

        self.move_cursor_right();
    }

    fn delete_char(&mut self) {
        if self.cursor_x == self.min_x {
            return;
        }

        let before_char_to_delete = self.input.chars().take(self.cursor_x - 1);
        let after_char_to_delete = self.input.chars().skip(self.cursor_x);

        self.input = before_char_to_delete.chain(after_char_to_delete).collect();

        self.min_x = 0;
        self.max_x = self.input.chars().count();

        self.move_cursor_left();
    }

    fn submit_message(&mut self) {
        if self.input.is_empty() {
            return;
        }

        self.messages
            .insert(self.messages.keys().len(), self.input.clone());
        self.reset_insert();
    }

    fn replace_message(&mut self) {
        if self.input.is_empty() {
            return;
        }

        self.messages[self.cursor_y] = self.input.clone();
        self.reset_insert();
    }

    fn change_selected(&mut self) {
        if self.selected.is_some() {
            self.prev_messages.clear();
            self.selected = None;
        } else {
            self.prev_messages = self.messages.clone();
            self.selected = Some(self.cursor_y);
        }
    }

    fn get_help_message<'a>(&self) -> Vec<Span<'a>> {
        match self.mode {
            InputMode::Normal => vec![
                "(Normal) ".bold(),
                "Esc: ".bold(),
                "exit. ".into(),
                "i: insert ".bold().green(),
                "mode. ".into(),
                "m: move ".bold().blue(),
                "mode.".into(),
            ],
            InputMode::Insert => vec![
                "(Insert) ".bold().green(),
                "Esc: ".bold(),
                "switch to normal mode. ".into(),
                "Enter: ".bold().green(),
                "save message.".into(),
            ],
            InputMode::Move => vec![
                "( Move ) ".bold().blue(),
                "Esc: ".bold().blue(),
                "switch to normal mode. ".into(),
                "Enter: ".bold().blue(),
                "select or unselect. ".into(),
                "e: edit ".bold().yellow(),
                "mode. ".into(),
                "backspace: remove ".bold().red(),
                "message.".into(),
            ],
            InputMode::Edit => vec![
                "( Edit ) ".bold().yellow(),
                "Esc: ".bold().blue(),
                "switch to move mode. ".into(),
                "Enter: ".bold().yellow(),
                "save changes.".into(),
            ],
        }
    }

    fn run(mut self, mut terminal: DefaultTerminal) -> Result<()> {
        loop {
            terminal.draw(|frame| self.render(frame))?;

            if let Some(key) = event::read()?.as_key_press_event() {
                if self.show_delete_popup {
                    match key.code {
                        KeyCode::Char('n') | KeyCode::Esc => self.show_delete_popup = false,
                        KeyCode::Char('y') => {
                            self.messages.shift_remove_index(self.cursor_y);
                            self.selected = None;
                            self.show_delete_popup = false;

                            if self.messages.is_empty() {
                                self.max_y = 0;
                                self.mode = InputMode::Normal;
                            } else {
                                self.max_y = self.messages.len() - 1;
                                self.move_cursor_up();
                            }
                        }
                        _ => {}
                    }
                }

                match self.mode {
                    InputMode::Normal => match key.code {
                        KeyCode::Esc => return self.save_messages(),
                        KeyCode::Char('i') => self.mode = InputMode::Insert,
                        KeyCode::Char('m') => {
                            self.mode = InputMode::Move;

                            self.min_y = 0;
                            self.max_y = self.messages.len() - 1;
                        }
                        _ => {}
                    },

                    InputMode::Insert if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => {
                            self.mode = InputMode::Normal;
                            self.reset_insert();
                        }
                        KeyCode::Left => self.move_cursor_left(),
                        KeyCode::Right => self.move_cursor_right(),
                        KeyCode::Char(character) => self.add_char(character),
                        KeyCode::Backspace => self.delete_char(),
                        KeyCode::Enter => self.submit_message(),
                        _ => {}
                    },

                    InputMode::Move if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => {
                            self.mode = InputMode::Normal;
                            self.reset_move();
                        }

                        KeyCode::Char('e') => {
                            self.mode = InputMode::Edit;

                            self.input = self.messages[self.cursor_y].clone();
                            self.max_x = self.input.chars().count();
                            self.cursor_x = self.max_x;
                        }

                        KeyCode::Backspace => {
                            self.show_delete_popup = true;
                        }

                        KeyCode::Up => self.move_cursor_up(),
                        KeyCode::Down => self.move_cursor_down(),
                        KeyCode::Enter => self.change_selected(),
                        _ => {}
                    },

                    InputMode::Edit if key.kind == KeyEventKind::Press => match key.code {
                        KeyCode::Esc => self.mode = InputMode::Move,
                        KeyCode::Left => self.move_cursor_left(),
                        KeyCode::Right => self.move_cursor_right(),
                        KeyCode::Char(character) => self.add_char(character),
                        KeyCode::Backspace => self.delete_char(),
                        KeyCode::Enter => {
                            self.mode = InputMode::Move;
                            self.replace_message();
                        }
                        _ => {}
                    },

                    _ => {}
                }
            }
        }
    }

    fn render(&self, frame: &mut Frame) {
        let help_message = self.get_help_message();

        let width = frame.area().width as usize;
        let help_width = help_message.iter().map(|s| s.content.to_string()).collect::<String>().len();
        let help_height = if help_width >= width { 2 } else { 1 };

        let vertical = Layout::vertical([
            Constraint::Length(help_height),
            Constraint::Length(3),
            Constraint::Min(1),
        ]);

        let [help_area, input_area, messages_area] = vertical.areas(frame.area());

        let help_widget = Paragraph::new(Text::from(Line::from(help_message))).wrap(Wrap { trim: true });
        frame.render_widget(help_widget, help_area);

        let input_text = if !self.input.is_empty() && matches!(self.mode, InputMode::Insert) {
            Span::raw(self.input.as_str())
        } else {
            Span::styled("Type your message here...", Style::default().gray())
        };

        let input_widget =
            Paragraph::new(input_text).block(Block::bordered().title("Input").border_style(
                match self.mode {
                    InputMode::Insert => Style::default().fg(Color::Green),
                    _ => Style::default().fg(Color::White),
                },
            ));

        frame.render_widget(input_widget, input_area);

        match self.mode {
            InputMode::Insert => frame.set_cursor_position(Position::new(
                input_area.x + self.cursor_x as u16 + 1,
                input_area.y + 1,
            )),
            InputMode::Move => frame.set_cursor_position(Position::new(
                messages_area.x + 1,
                messages_area.y + self.cursor_y as u16 + 1,
            )),
            InputMode::Edit => frame.set_cursor_position(Position::new(
                messages_area.x + self.cursor_x as u16 + 1 + self.calc_cursor_min_x() as u16,
                messages_area.y + self.cursor_y as u16 + 1,
            )),
            _ => {}
        }

        let message_list: Vec<ListItem> = self
            .messages
            .iter()
            .enumerate()
            .map(|(pos, (k, mut v))| {
                let is_selected = self.selected.is_some() && pos == self.cursor_y;

                let style = match self.mode {
                    InputMode::Move if is_selected => Style::default().bg(Color::LightBlue),
                    InputMode::Edit if is_selected => Style::default().bg(Color::LightYellow),
                    _ => Style::default(),
                };

                if matches!(self.mode, InputMode::Edit) && pos == self.cursor_y {
                    v = &self.input;
                }

                let content = Line::from(Span::raw(format!("{k}: {v}"))).style(style);

                ListItem::new(content)
            })
            .collect();

        let messages_widget = List::new(message_list)
            .style(match self.mode {
                InputMode::Move => Style::default().fg(Color::Blue),
                InputMode::Edit => Style::default().fg(Color::Yellow),
                _ => Style::default().fg(Color::White),
            })
            .block(Block::bordered().title("Messages"));

        frame.render_widget(messages_widget, messages_area);

        if self.show_delete_popup {
            let vertical = Layout::vertical([Constraint::Length(4)]).flex(Flex::Center);
            let horizontal = Layout::horizontal([Constraint::Length(40)]).flex(Flex::Center);

            let area = frame.area();
            let [area] = vertical.areas(area);
            let [area] = horizontal.areas(area);

            let block = Block::bordered()
                .title("Are you sure?")
                .style(Style::default().fg(Color::Red));

            let drop_message = vec![
                vec![
                    "Press ".into(),
                    "'y'".bold().red(),
                    " to remove message. ".into(),
                ],
                vec![
                    "Press ".into(),
                    "'n' or 'esc'".bold().red(),
                    " to cancel.".into(),
                ],
            ];

            let text = Paragraph::new(Text::from(
                drop_message
                    .into_iter()
                    .map(Line::from)
                    .collect::<Vec<Line>>(),
            ))
            .block(block)
            .alignment(Alignment::Center)
            .wrap(Wrap { trim: true });

            frame.render_widget(Clear, area);
            frame.render_widget(text, area);
        }
    }
}
