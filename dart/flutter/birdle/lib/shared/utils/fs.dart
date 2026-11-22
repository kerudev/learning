// Source: https://github.com/kerudev/learning/blob/master/powershell/lang/wordle/src/Fs.ps1

import 'dart:io';

void writeFileMode(
  String path,
  Set<String> lines, [
  FileMode mode = FileMode.write,
]) {
  var file = File(path);

  if (!file.existsSync()) {
    file.createSync(recursive: true);
  }

  final contents = lines.join("\n");
  if (contents.isEmpty) return;

  file.writeAsStringSync("$contents\n", mode: mode);
}

void writeFile(String path, Set<String> lines) {
  writeFileMode(path, lines, FileMode.write);
}

void appendFile(String path, Set<String> lines) {
  writeFileMode(path, lines, FileMode.append);
}

Set<String> readFile(String path) {
  var file = File(path);

  return file.existsSync() && file.statSync().size > 0
      ? file.readAsLinesSync().toSet()
      : {};
}
