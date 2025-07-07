# 📚 learning

This repo contains small projects and code snippets I've used to learn
programming languages or libraries I'm interested in.

## The workflow

To work a new branch:

```sh
git checkout -b <branch>
mkdir -p $(git branch --show-current)
code $(git branch --show-current)
```

To go back to the top level:

```sh
code $(git rev-parse --show-toplevel)
```

## What I want to learn

### Python
- **pandas**: data analysis. Uses DataFrames (table-like structures) as the core component. 
  - Docs: https://pandas.pydata.org/docs/

### Rust
- **serde**: I've worked with it on a surface level, but I want to know it more in depth.
  - Home: https://serde.rs/

- **tokio**: really popular on the Rust community. Would be helpful to reinforce my concurrency knowledge.
  - Home: https://tokio.rs/

### C
- **Clay**: modern layout library. Can render with raylib, WebGL, etc. 
  - Home: https://www.nicbarker.com/clay

### Web
- **NextJS**: similar to React, but with backend management too.
  - Home: https://nextjs.org/
  - Docs: https://nextjs.org/learn

- **WebGL**: graphics engine in the browser.
  - Docs: https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API

### Zig
- I've always been curious about it.
  - Home: https://ziglang.org/
  - Docs: https://ziglang.org/learn/
  - Book: https://pedropark99.github.io/zig-book/

### Go
- I want to know why it got so popular and explore it!
  - Home: https://go.dev/
  - Docs: https://go.dev/learn/

### Arduino
- Planning on buying a starter kit soon, as that stuff interests me and I want
to dip my toes into embedded programming.

### assembly
- Why not?
- I'd like to learn the NASM (Intel) before GAS (AT&T).
  - Docs (syscalls x86_64): https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

### Others
- Maybe I'll tinker with V or Odin some day.

## What I have learned

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
  - [second part](./cpp/opengl/learnopengl)

- **raylib**: [docs](https://www.raylib.com/)
  - [2d-camera](./c/raylib/2d-camera)

### C++
- **learnopengl**: [home](https://learnopengl.com/)
  - [first part](./c/opengl/learnopengl) (lessons use C++ libraries, so I moved to C++ to save time)
  - [04_transformations](./cpp/opengl/learnopengl/04_transformations/)

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
