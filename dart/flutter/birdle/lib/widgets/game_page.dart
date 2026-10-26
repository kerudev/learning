import 'package:flutter/material.dart';

import 'package:birdle/widgets/guess_input.dart';
import 'package:birdle/widgets/keyboard.dart';
import 'package:birdle/widgets/tile.dart';

import 'package:birdle/utils/game.dart';

class GamePage extends StatefulWidget {
  const GamePage({super.key});

  @override
  State<GamePage> createState() => _GamePageState();
}

class _GamePageState extends State<GamePage> {
  final Game _game = Game();

  final TextEditingController _textEditingController = TextEditingController();

  final Map<String, HitType> _keyStates = {};

  void _updateKeyboard(String guess) {
    Word word = _game.hiddenWord;
    List<String> chars = guess.split("");

    Map<String, bool?> positions = {};

    for (var i = 0; i < chars.length; i++) {
      positions[chars[i]] = null;

      for (var j = 0; j < word.length; j++) {
        String char = word[j].char;

        if (positions[char] != true && chars[i] == char) {
          positions[char] = i == j;
        }
      }
    }

    for (MapEntry e in positions.entries) {
      if (e.value == null) {
        _keyStates[e.key] = HitType.miss;
      } else {
        _keyStates[e.key] = e.value ? HitType.hit : HitType.partial;
      }
    }
  }

  void _onKeyPress(String char) {
    _textEditingController.text = _textEditingController.text + char;
  }

  Widget _newCell(Letter letter) {
    return Padding(
      padding: const EdgeInsets.all(2.5),
      child: Tile(letter.char, letter.type),
    );
  }

  Widget _newRow(Word guess) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: guess.map(_newCell).toList(),
    );
  }

  Widget _inputBar() {
    return ConstrainedBox(
      constraints: const BoxConstraints(maxWidth: 400),
      child: GuessInput(
        controller: _textEditingController,
        onSubmitGuess: (guess) {
          setState(() {
            _game.guess(guess);
            _updateKeyboard(guess);
          });
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Column(
        children: [
          ..._game.guesses.map(_newRow),
          _inputBar(),
          Keyboard(states: _keyStates, onClick: _onKeyPress),
        ],
      ),
    );
  }
}
