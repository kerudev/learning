import 'package:flutter/material.dart';

import 'package:birdle/widgets/grid.dart';
import 'package:birdle/widgets/difficulty.dart';
import 'package:birdle/widgets/message.dart';
import 'package:birdle/widgets/guess_input.dart';
import 'package:birdle/widgets/keyboard.dart';

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

  void _onDifficultyChange(int d) {
    setState(() {
      _game.guessLength = d;
    });
  }

  void _updateKeyboard(Word guess) {
    for (final letter in guess) {
      _keyStates[letter.char] = letter.type;
    }
  }

  void _onKeyPress(String char) {
    if (_textEditingController.text.length < 5) {
      _textEditingController.text = _textEditingController.text + char;
    }
  }

  Widget _inputBar() {
    return ConstrainedBox(
      constraints: const BoxConstraints(maxWidth: 400),
      child: GuessInput(
        controller: _textEditingController,
        onSubmitGuess: (guess) {
          setState(() {
            try {
              _updateKeyboard(_game.guess(guess));

              if (_game.didWin) {
                ScaffoldMessenger.of(context).removeCurrentSnackBar();
                ScaffoldMessenger.of(context)
                    .showSnackBar(Message("You won!", color: Colors.green));
              }
            } on ArgumentError catch (e) {
              ScaffoldMessenger.of(context).removeCurrentSnackBar();
              ScaffoldMessenger.of(context)
                  .showSnackBar(Message(e.message, color: Colors.redAccent));
            }
          });
        },
      ),
    );
  }

  List<Widget> _children() {
    final List<Widget> children = [
      Column(
        children: [
          Difficulty(_game.guessLength, onClick: _onDifficultyChange),
          Grid(_game.guesses),
        ],
      ),
    ];

    if (!_game.didWin) {
      children.addAll([
        _inputBar(),
        Keyboard(states: _keyStates, onClick: _onKeyPress),
      ]);
    }

    return children;
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Column(children: _children()),
    );
  }
}
