import 'package:birdle/utils/game.dart';
import 'package:flutter/material.dart';

import 'package:birdle/widgets/tile.dart';

class Keyboard extends StatelessWidget {
  Keyboard({super.key, required this.states});

  final Map<String, HitType> states;

  final List<List<String>> keys = [
    ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"],
    ["A", "S", "D", "F", "G", "H", "J", "K", "L"],
    ["Z", "X", "C", "V", "B", "N", "M"],
  ];

  Tile _asTile(String key) {
    final k = key.toLowerCase();
    return Tile(k, states[k] ?? HitType.none);
  }

  Widget _newCell(String k) {
    return Padding(
      padding: const EdgeInsets.all(2.5),
      child: _asTile(k) as Widget,
    );
  }

  Widget _newRow(List<String> row) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: row.map(_newCell).toList(),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Column(children: keys.map(_newRow).toList());
  }
}
