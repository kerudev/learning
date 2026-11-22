import 'dart:convert';

import 'package:http/http.dart' as http;

Future<Set<String>> fetchWords(int length) async {
  final uri = Uri.https("random-word-api.herokuapp.com", "word", {
    "number": "500",
    "length": length.toString(),
  });

  final res = await http.get(uri);
  final data = json.decode(res.body);
  final words = Set<String>.from(data);

  return words;
}
