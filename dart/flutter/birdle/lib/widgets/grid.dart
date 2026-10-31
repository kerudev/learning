import 'package:flutter/material.dart';

import 'package:birdle/utils/game.dart';
import 'package:birdle/widgets/tile.dart';

class Grid extends StatefulWidget {
  const Grid(this.guesses, {super.key});

  final List<Word> guesses;

  @override
  State<Grid> createState() => _GridState();
}

class _GridState extends State<Grid> {
  Widget _newCell(Letter letter) {
    return Padding(
      padding: const EdgeInsets.all(2.5),
      child: Tile(letter.char, letter.type.asColor()),
    );
  }

  Widget _newRow(Word guess) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: guess.map(_newCell).toList(),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Column(children: widget.guesses.map(_newRow).toList());
  }
}
