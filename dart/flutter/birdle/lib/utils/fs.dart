// Source: https://github.com/kerudev/learning/blob/master/powershell/lang/wordle/src/Fs.ps1

import 'dart:io';

const filesPath = "words";
const knownWordsPath = "$filesPath/known.txt";
const availableWordsPath = "$filesPath/available.txt";
const usedWordsPath = "$filesPath/used.txt";

void writeFileMode(
  String path,
  Set<String> lines, [
  FileMode mode = FileMode.write,
]) {
  var file = File(path);

  if (!file.existsSync()) {
    Directory(filesPath).createSync();
    file.createSync();
  }

  final contents = lines.join("\n") + (lines.length == 1 ? "\n" : "");

  file.writeAsStringSync(contents, mode: mode);
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
