// Source: https://github.com/kerudev/learning/blob/master/powershell/lang/wordle/src/Api.ps1

import 'dart:convert';

import 'package:http/http.dart' as http;

var uri = Uri.https("random-word-api.herokuapp.com", "word", {
  "length": "5",
  // "number": "5",
  "number": "500",
});

Future<Set<String>> fetchWords() async {
  final Set<String> words = {};

  for (int i = 0; i < 8; i++) {
    final res = await http.get(uri);
    final data = json.decode(res.body).cast<String>().toList();
    words.addAll(data);
  }

  return words;
}
