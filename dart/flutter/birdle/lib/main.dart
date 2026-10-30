import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

import 'package:birdle/root.dart';
import 'package:birdle/utils/fs.dart';
import 'package:birdle/utils/http.dart';

void main() async {
  // Prefetch words so the whole code in Root stays sync
  if (!kIsWeb) await _loadFile(knownWordsPath);

  runApp(const Root());
}

Future<Null> _loadFile(String path) async {
  if (await File(path).exists()) return;

  final words = (await fetchWords()).toSet();
  writeFile(knownWordsPath, words);
  writeFile(availableWordsPath, words);
}
