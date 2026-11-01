import 'dart:io';

import 'package:flutter/foundation.dart';

import 'package:birdle/utils/game.dart';
import 'package:birdle/utils/fs.dart';
import 'package:birdle/utils/http.dart';

/// Handles the game's context.
class Context {
  late int difficulty;

  final String wordsRoot = "words";

  late String wordsPath;
  late String allPath;
  late String poolPath;
  late String usedPath;

  /// Every word that can be legally entered as a guess.
  late List<String> allLegalGuesses;

  /// Words that can be chosen as the hidden word.
  late List<String> legalWords;

  /// Additional words accepted as guesses beyond those in [legalWords].
  late List<String> legalGuesses;

  Context._(int difficulty);

  static Future<Context> load(int difficulty) async {
    final self = Context._(difficulty);
    await self.setDifficulty(difficulty);
    return self;
  }

  Future<void> setDifficulty(int d) async {
    difficulty = d;

    setPaths(d);
    await ensureWords(d);
    setWords();
  }

  void setPaths(int d) {
    print("1");

    wordsPath = "$wordsRoot/$d";

    allPath = "$wordsPath/all.txt";
    poolPath = "$wordsPath/pool.txt";
    usedPath = "$wordsPath/used.txt";
  }

  Future<void> ensureWords(int d) async {
    if (await File(allPath).existsSync()) return;
    print("2");

    final words = (await fetchWords(d)).toSet();
    writeFile(allPath, words);
  }

  void setWords() {
    print("3");

    legalWords = kIsWeb
        ? ['aback', 'abase', 'abate', 'abbey', 'abbot']
        : readFile(poolPath).toList();

    legalGuesses = kIsWeb
        ? [
            'aback',
            'abase',
            'abate',
            'abbey',
            'abbot',
            'abhor',
            'abide',
            'abled',
            'abode',
            'abort',
          ]
        : readFile(allPath).toList();

    allLegalGuesses = [...legalWords, ...legalGuesses];
  }
}

late Context gContext;

Future<void> initContext() async {
  gContext = await Context.load(Game.defaultDifficulty);
}
