# flutter/lang/birdle

Example and exercises from https://docs.flutter.dev/learn/pathway/tutorial/create-an-app

In this project I learned how to:
- Create a Flutter app.
- What the Widget tree is.
- How to use `StatelessWidget` and `StatefulWidget`.
- How to use `Row`, `Column`, `Expanded`, `Padding`, `BoxDecoration`, `AnimatedContainer`, ...
- How to use `Text`, `Icon`, ...
- How to install packages (`flutter pub get [name]`).
- How to make `http` requests.
- How to handle files with `dart:io`.
- How to cleanup a new project:

  ```sh
  flutter clean && flutter pub get
  ```

- How to install dev dependencies:

  ```sh
  flutter pub get dev:[name]
  ```

- How to sort import statements with `import_sorter`:

  ```sh
  flutter pub run import_sorter:main --no-comments
  ```

Run on your preferred device with `flutter run -d [device]`.

Use `--host-vmservice-port=[port]` and `--disable-service-auth-codes` to open
the debug server on the same URL.

```sh
flutter run --host-vmservice-port=55555 --disable-service-auth-codes -d linux
```

Note that running on `chrome` won't have random words as those are read from
files, which can't be accessed by the browser so easily.
