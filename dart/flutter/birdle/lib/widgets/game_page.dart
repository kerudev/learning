import 'package:flutter/material.dart';

import 'package:birdle/widgets/guess_input.dart';
import 'package:birdle/widgets/tile.dart';
import 'package:birdle/game.dart';

class GamePage extends StatefulWidget {
  const GamePage({super.key});

  @override
  State<GamePage> createState() => GamePageState();
}

class GamePageState extends State<GamePage> {
  final Game _game = Game();

  Widget _rowChild(({String char, HitType type}) letter) {
    return Padding(
      padding: const EdgeInsets.all(2.5),
      child: Tile(letter.char, letter.type),
    );
  }

  Widget _newRow(Word guess) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: guess.map(_rowChild).toList(),
    );
  }

  Widget _inputBar() {
    return GuessInput(
      onSubmitGuess: (guess) {
        setState(() => _game.guess(guess));
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Column(children: [..._game.guesses.map(_newRow), _inputBar()]),
    );
  }
}
