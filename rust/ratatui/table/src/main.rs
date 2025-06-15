use color_eyre::Result;
use crossterm::event::{self, KeyCode, KeyModifiers};
use itertools::Itertools;
use ratatui::layout::{Constraint, Layout, Margin, Rect};
use ratatui::style::{self, Color, Modifier, Style, Stylize};
use ratatui::text::Text;
use ratatui::widgets::{
    Block, BorderType, Cell, HighlightSpacing, Paragraph, Row, Scrollbar, ScrollbarOrientation,
    ScrollbarState, Table, TableState,
};
use ratatui::{DefaultTerminal, Frame};
use style::palette::tailwind;
use unicode_width::UnicodeWidthStr;

const PALETTES: [tailwind::Palette; 4] = [
    tailwind::BLUE,
    tailwind::EMERALD,
    tailwind::INDIGO,
    tailwind::RED,
];

const INFO_TEXT: [&str; 2] = [
    "(Esc) quit | (↑) move up | (↓) move down | (←) move left | (→) move right",
    "(Shift + →) next color | (Shift + ←) previous color",
];

const ITEM_HEIGHT: usize = 4;
const SHOWN_COLS: usize = 3;

fn main() -> Result<()> {
    color_eyre::install()?;
    App::new();

    let terminal = ratatui::init();
    let app_result = App::new().run(terminal);
    ratatui::restore();
    app_result
}

struct TableColors {
    buffer_bg: Color,
    header_bg: Color,
    header_fg: Color,
    row_fg: Color,
    selected_row_style_fg: Color,
    selected_column_style_fg: Color,
    selected_cell_style_fg: Color,
    normal_row_color: Color,
    alt_row_color: Color,
    footer_border_color: Color,
}

impl TableColors {
    const fn new(color: &tailwind::Palette) -> Self {
        Self {
            buffer_bg: tailwind::SLATE.c950,
            header_bg: color.c900,
            header_fg: tailwind::SLATE.c200,
            row_fg: tailwind::SLATE.c200,
            selected_row_style_fg: color.c400,
            selected_column_style_fg: color.c400,
            selected_cell_style_fg: color.c600,
            normal_row_color: tailwind::SLATE.c950,
            alt_row_color: tailwind::SLATE.c900,
            footer_border_color: color.c400,
        }
    }
}

struct Data {
    name: String,
    address: String,
    email: String,
    pet: String,
    language: String,
    job: String,
    beer: String
}

impl Data {
    const fn ref_array(&self) -> [&String; 7] {
        [
            &self.name,
            &self.address,
            &self.email,
            &self.pet,
            &self.language,
            &self.job,
            &self.beer,
        ]
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn name(&self) -> &str {
        &self.name
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn address(&self) -> &str {
        &self.address
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn email(&self) -> &str {
        &self.email
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn pet(&self) -> &str {
        &self.pet
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn language(&self) -> &str {
        &self.language
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn job(&self) -> &str {
        &self.job
    }

    // https://github.com/rust-lang/rust/issues/139338
    #[allow(clippy::missing_const_for_fn)]
    fn beer(&self) -> &str {
        &self.beer
    }
}

struct App {
    state: TableState,
    items: Vec<Data>,
    total_cols: usize,
    avg_col_length: usize,
    longest_item_lens: Vec<u16>,
    scroll_y_state: ScrollbarState,
    scroll_x_state: ScrollbarState,
    offset_x: usize,
    colors: TableColors,
    color_index: usize,
}

impl App {
    fn new() -> Self {
        let data_vec = generate_fake_names();
        let lengths = constraint_len_calculator(&data_vec);
        let total_cols = lengths.len();

        let total_length = lengths.iter().sum::<u16>() as usize;
        let avg_col_length = total_length / (total_cols - SHOWN_COLS);

        Self {
            state: TableState::default().with_selected(0),
            longest_item_lens: lengths,
            total_cols,
            avg_col_length,
            scroll_y_state: ScrollbarState::new((data_vec.len() - 1) * ITEM_HEIGHT),
            scroll_x_state: ScrollbarState::new(total_length),
            offset_x: 0,
            colors: TableColors::new(&PALETTES[0]),
            color_index: 0,
            items: data_vec,
        }
    }

    pub fn next_row(&mut self) {
        let i = match self.state.selected() {
            Some(i) => {
                if i >= self.items.len() - 1 {
                    0
                } else {
                    i + 1
                }
            }
            None => 0,
        };

        self.state.select(Some(i));
        self.scroll_y_state = self.scroll_y_state.position(i * ITEM_HEIGHT);
    }

    pub fn previous_row(&mut self) {
        let i = match self.state.selected() {
            Some(i) => {
                if i == 0 {
                    self.items.len() - 1
                } else {
                    i - 1
                }
            }
            None => 0,
        };

        self.state.select(Some(i));
        self.scroll_y_state = self.scroll_y_state.position(i * ITEM_HEIGHT);
    }

    pub fn next_column(&mut self) {
        let mut i = match self.state.selected_column() {
            Some(i) => {
                if i + self.offset_x >= self.total_cols - 1 {
                    self.offset_x = 0;
                    0
                } else {
                    i + 1
                }
            }
            None => 0,
        };

        if i > SHOWN_COLS - 1 {
            self.offset_x += 1;
            i -= 1;
        }

        self.state.select_column(Some(i));
        self.scroll_x_state = self.scroll_x_state.position(self.offset_x * self.avg_col_length);
    }

    pub fn previous_column(&mut self) {
        let i = match self.state.selected_column() {
            Some(i) => {
                if i == 0 && self.offset_x == 0 {
                    self.offset_x = self.total_cols - SHOWN_COLS;
                    SHOWN_COLS
                } else if i == 0 {
                    self.offset_x -= 1;
                    0
                } else {
                    i - 1
                }
            }
            None => 0,
        };

        self.state.select_column(Some(i));
        self.scroll_x_state = self.scroll_x_state.position(self.offset_x * self.avg_col_length);
    }

    pub const fn next_color(&mut self) {
        self.color_index = (self.color_index + 1) % PALETTES.len();
    }

    pub const fn previous_color(&mut self) {
        let count = PALETTES.len();
        self.color_index = (self.color_index + count - 1) % count;
    }

    pub const fn set_colors(&mut self) {
        self.colors = TableColors::new(&PALETTES[self.color_index]);
    }

    fn run(mut self, mut terminal: DefaultTerminal) -> Result<()> {
        loop {
            terminal.draw(|frame| self.render(frame))?;

            if let Some(key) = event::read()?.as_key_press_event() {
                let shift_pressed = key.modifiers.contains(KeyModifiers::SHIFT);
                match key.code {
                    KeyCode::Char('q') | KeyCode::Esc => return Ok(()),
                    KeyCode::Char('j') | KeyCode::Down => self.next_row(),
                    KeyCode::Char('k') | KeyCode::Up => self.previous_row(),
                    KeyCode::Char('l') | KeyCode::Right if shift_pressed => self.next_color(),
                    KeyCode::Char('h') | KeyCode::Left if shift_pressed => {
                        self.previous_color();
                    }
                    KeyCode::Char('l') | KeyCode::Right => self.next_column(),
                    KeyCode::Char('h') | KeyCode::Left => self.previous_column(),
                    _ => {}
                }
            }
        }
    }

    fn render_table(&mut self, frame: &mut Frame, area: Rect) {
        let header_style = Style::default()
            .fg(self.colors.header_fg)
            .bg(self.colors.header_bg);

        let selected_row_style = Style::default()
            .add_modifier(Modifier::REVERSED)
            .fg(self.colors.selected_row_style_fg);

        let selected_col_style = Style::default().fg(self.colors.selected_column_style_fg);
        let selected_cell_style = Style::default()
            .add_modifier(Modifier::REVERSED)
            .fg(self.colors.selected_cell_style_fg);

        let header_titles: Vec<&str> = vec!["Name", "Address", "Email", "Pet", "Language", "Job", "Beer"]
            .into_iter()
            .skip(self.offset_x)
            .take(SHOWN_COLS)
            .collect();

        let header = header_titles
            .into_iter()
            .map(Cell::from)
            .collect::<Row>()
            .style(header_style)
            .height(1);

        let rows = self.items.iter().enumerate().map(|(i, data)| {
            let color = match i % 2 {
                0 => self.colors.normal_row_color,
                _ => self.colors.alt_row_color,
            };

            let item = data.ref_array();

            item.into_iter()
                .skip(self.offset_x)
                .take(SHOWN_COLS)
                .map(|content| Cell::from(Text::from(format!("\n{content}\n"))))
                .collect::<Row>()
                .style(Style::new().fg(self.colors.row_fg).bg(color))
                .height(4)
        });
        
        let [len_col1, len_col2, len_col3] = [
            self.longest_item_lens[self.offset_x],
            self.longest_item_lens[self.offset_x + 1],
            self.longest_item_lens[self.offset_x + 2],
        ];
            
        let bar = " █ ";
        let table = Table::new(
            rows,
            [
                Constraint::Length(len_col1 + 1),
                Constraint::Min(len_col2 + 1),
                Constraint::Min(len_col3),
            ],
        )
        .header(header)
        .row_highlight_style(selected_row_style)
        .column_highlight_style(selected_col_style)
        .cell_highlight_style(selected_cell_style)
        .highlight_symbol(Text::from(vec![
            "".into(),
            bar.into(),
            bar.into(),
            "".into(),
        ]))
        .bg(self.colors.buffer_bg)
        .highlight_spacing(HighlightSpacing::Always);

        frame.render_stateful_widget(table, area, &mut self.state);
    }

    fn render_scrollbar_y(&mut self, frame: &mut Frame, area: Rect) {
        frame.render_stateful_widget(
            Scrollbar::new(ScrollbarOrientation::VerticalRight),
            area.inner(Margin {
                vertical: 1,
                horizontal: 1,
            }),
            &mut self.scroll_y_state,
        );
    }

    fn render_scrollbar_x(&mut self, frame: &mut Frame, area: Rect) {
        frame.render_stateful_widget(
            Scrollbar::new(ScrollbarOrientation::HorizontalBottom),
            area,
            &mut self.scroll_x_state,
        );
    }

    fn render_footer(&self, frame: &mut Frame, area: Rect) {
        let info_footer = Paragraph::new(Text::from_iter(INFO_TEXT))
            .style(
                Style::new()
                    .fg(self.colors.row_fg)
                    .bg(self.colors.buffer_bg),
            )
            .centered()
            .block(
                Block::bordered()
                    .border_type(BorderType::Double)
                    .border_style(Style::new().fg(self.colors.footer_border_color)),
            );
        frame.render_widget(info_footer, area);
    }

    fn render(&mut self, frame: &mut Frame) {
        let vertical = Layout::vertical([Constraint::Min(5), Constraint::Length(4)]);
        let rects = vertical.split(frame.area());

        self.set_colors();

        self.render_table(frame, rects[0]);
        self.render_scrollbar_y(frame, rects[0]);
        self.render_scrollbar_x(frame, rects[0]);
        self.render_footer(frame, rects[1]);
    }
}

fn generate_fake_names() -> Vec<Data> {
    use fakeit::{address, contact, name, animal, language, job, beer};

    (0..20)
        .map(|_| {
            let name = name::full();
            let address = format!(
                "{}\n{}, {} {}",
                address::street(),
                address::city(),
                address::state(),
                address::zip()
            );

            let email = contact::email();
            let pet = animal::animal();
            let language = language::random();
            let job = job::title();
            let beer = beer::name();

            Data {
                name,
                address,
                email,
                pet,
                language,
                job,
                beer,
            }
        })
        .sorted_by(|a, b| a.name.cmp(&b.name))
        .collect()
}

fn calc_column_len<F>(items: &[Data], func: F) -> usize 
where
    F: Fn(&Data) -> &str
{
    items
        .iter()
        .map(func)
        .map(UnicodeWidthStr::width)
        .max()
        .unwrap_or(0)
}

fn constraint_len_calculator(items: &[Data]) -> Vec<u16> {
    let name_len = calc_column_len(items, Data::name);

    let address_len = items
        .iter()
        .map(Data::address)
        .flat_map(str::lines)
        .map(UnicodeWidthStr::width)
        .max()
        .unwrap_or(0);

    let email_len = calc_column_len(items, Data::email);
    let pet_len = calc_column_len(items, Data::pet);
    let language_len = calc_column_len(items, Data::language);
    let job_len = calc_column_len(items, Data::job);
    let beer_len = calc_column_len(items, Data::beer);

    vec![
        name_len as u16,
        address_len as u16,
        email_len as u16,
        pet_len as u16,
        language_len as u16,
        job_len as u16,
        beer_len as u16,
    ]
}
