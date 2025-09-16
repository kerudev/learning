# 📚 learning

This repo contains small projects and code snippets I've used to learn
programming languages or libraries I'm interested in.

## My workflow

I follow the same workflow throughout the lifetime of a branch.

To speed things up, I use the following aliases:

```sh
alias commit="git commit -m "$1""
alias add="git add ."
alias push="git push"
alias branch="git branch --show-current"
alias toplevel="git rev-parse --show-toplevel"
alias prev="git rev-parse --symbolic-full-name --abbrev-ref=loose @{-1}"
alias master="git checkout master"
```

To work a new branch:

```sh
git checkout -b <branch>
mkdir -p $(branch)
code $(branch)
```

To make a new commit:

```sh
add
commit "[$(branch)] commit message"
```

To rebase master after completing work on a branch:

```sh
push
code $(toplevel)
master
git rebase $(prev)
```

After that, I do repeat the process for the next branch.

## What I want to learn

### Python
- [**collections**](https://docs.python.org/3.13/library/collections.html): standard library.
- [**functools**](https://docs.python.org/3.13/library/functools.html): standard library.
- [**itertools**](https://docs.python.org/3.13/library/itertools.html): standard library.
- [**struct**](https://docs.python.org/3.13/library/struct.html): standard library.
- [**pandas**](https://pandas.pydata.org/docs/): data analysis library.

### Rust
- [**serde**](https://serde.rs/): I've worked with it on a surface level, but I want to know it more in depth.
- [**tokio**](https://tokio.rs/): really popular on the Rust community. Would be helpful to reinforce my concurrency knowledge.

### C
- [**Clay**](https://www.nicbarker.com/clay): modern layout library. Can render with raylib, WebGL, etc. 

### Web
- [**NextJS**](https://nextjs.org/): similar to React, but with backend management too.
- [**WebGL**](https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API): graphics engine in the browser.

### Zig
- [**lang**](https://ziglang.org/): I've always been curious about it. 

### Go
- [**lang**](https://go.dev/): I want to know why it got so popular and explore it!

### assembly
- Why not?
- I'd like to learn the NASM (Intel) before GAS (AT&T).
  - Docs (syscalls x86_64): https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

### Other languages
- V: general purpose language ([repo](https://github.com/vlang/v))
- Odin: Data-Oriented language ([repo](https://github.com/odin-lang/Odin))
- Gleam: language for building scalable systems ([repo](https://github.com/gleam-lang/gleam))
- Amber: compiles to Bash ([repo](https://github.com/amber-lang/amber))
- Carbon: experimental successor of C++ ([repo](https://github.com/carbon-language/carbon-lang))

### Others
- [LLVM tutorials](https://llvm.org/docs/tutorial/)
- [NextJS docs](https://nextjs.org/learn)
- [Go docs](https://go.dev/learn/)
- [Zig docs](https://ziglang.org/learn/)
- [Zig book](https://pedropark99.github.io/zig-book/)
- More on my [Ideas](https://github.com/stars/kerudev/lists/ideas) star list.

## What I have learned

### Arduino
- **book**: [docs](https://docs.arduino.cc/)
  - [02_spaceship_interface](./arduino/book/02_spaceship_interface)
  - [03_love_o_meter](./arduino/book/03_love_o_meter)
  - [04_color_mixing_lamp](./arduino/book/04_color_mixing_lamp)
  - [05_mood_cue](./arduino/book/05_mood_cue)
  - [06_light_theremin](./arduino/book/06_light_theremin)
  - [07_keyboard_instrument](./arduino/book/07_keyboard_instrument)

### C
- **lang**: [docs](https://en.cppreference.com/w/c/language)
  - [hello-blank](./c/lang/hello-blank)
  - [rock-paper-scissors](./c/lang/rock-paper-scissors)
  - [todo-list](./c/lang/todo-list)

- **opengl**: [docs](https://www.opengl.org/)
  - [tetrahedron](./c/opengl/tetrahedron)

- **learnopengl**: [home](https://learnopengl.com/)
  - [00_window](./c/opengl/learnopengl/00_window)
  - [01_triangle](./c/opengl/learnopengl/01_triangle)
  - [02_shaders](./c/opengl/learnopengl/02_shaders)
  - [03_textures](./c/opengl/learnopengl/03_textures)
  - [second part](./cpp/opengl/learnopengl) (lessons use C++ libraries, so I moved to C++ to save time)

- **raylib**: [docs](https://www.raylib.com/)
  - [2d-camera](./c/raylib/2d-camera)

### C++
- **learnopengl**: [home](https://learnopengl.com/)
  - [first part](./c/opengl/learnopengl) (lessons use C++ libraries, so I moved to C++ to save time)
  - [04_transformations](./cpp/opengl/learnopengl/04_transformations)
  - [05_coordinate_systems](./cpp/opengl/learnopengl/05_coordinate_systems)
  - [06_camera](./cpp/opengl/learnopengl/06_camera)
  - [10_colors](./cpp/opengl/learnopengl/10_colors)
  - [11_basic_lighting](./cpp/opengl/learnopengl/11_basic_lighting)
  - [12_materials](./cpp/opengl/learnopengl/12_materials)
  - [13_lighting_maps](./cpp/opengl/learnopengl/13_lighting_maps)
  - [14_light_casters](./cpp/opengl/learnopengl/14_light_casters)
  - [15_multiple_lights](./cpp/opengl/learnopengl/15_multiple_lights)
  - [20_model_loading](./cpp/opengl/learnopengl/20_model_loading)

### PowerShell
- **lang**: [docs](https://learn.microsoft.com/en-us/powershell/scripting/how-to-use-docs?view=powershell-7.5)
  - [wordle](./powershell/lang/wordle)

### Python
- **pickle**: [docs](https://docs.python.org/3/library/pickle.html)
  - [hangman](./python/pickle/hangman)

### Rust
- **ratatui**: [docs](https://ratatui.rs/)
  - [user-input](./rust/ratatui/user-input)
  - [table](./rust/ratatui/table)
