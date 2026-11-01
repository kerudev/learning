// Source: https://github.com/kerudev/learning/blob/master/powershell/lang/wordle/src/Api.ps1

import 'dart:convert';

import 'package:http/http.dart' as http;

const nFetches = 8;

var uri = Uri.https("random-word-api.herokuapp.com", "word", {
  "number": "500",
  "length": "5",
});

Future<Set<String>> fetchWords(int length) async {
  uri = uri.replace(
    queryParameters: {"number": "500", "length": length.toString()},
  );

  final Set<String> words = {};

  for (int i = 0; i < nFetches; i++) {
    final res = await http.get(uri);
    final data = json.decode(res.body).cast<String>().toList();
    words.addAll(data);
  }

  return words;
}
