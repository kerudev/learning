import 'package:flutter/material.dart';

import 'package:birdle/screens/game/widgets/grid.dart';
import 'package:birdle/screens/game/widgets/guess_input.dart';
import 'package:birdle/screens/game/widgets/keyboard.dart';
import 'package:birdle/shared/utils/context.dart';
import 'package:birdle/shared/utils/game.dart';
import 'package:birdle/shared/widgets/difficulty.dart';
import 'package:birdle/shared/widgets/message.dart';

class GameScreen extends StatefulWidget {
  const GameScreen({super.key});

  @override
  State<GameScreen> createState() => _GameScreenState();
}

class _GameScreenState extends State<GameScreen> {
  late Game _game = Game();

  final TextEditingController _textController = TextEditingController();

  final Map<String, HitType> _keyStates = {};

  void _onDifficultyChange(int d) {
    setState(() {
      gContext.setDifficulty(d);
      _game = Game();
    });
  }

  void _onSubmit(String guess) {
    setState(() {
      try {
        _updateKeyboard(_game.guess(guess));
        if (_game.didWin) {
          Message("You won!", color: Colors.green).show(context);
        }
      } on ArgumentError catch (e) {
        Message(e.message, color: Colors.redAccent).show(context);
      }
    });
  }

  void _updateKeyboard(Word guess) {
    for (final letter in guess) {
      _keyStates[letter.char] = letter.type;
    }
  }

  void _onKeyPress(String char) {
    if (_textController.text.length < gContext.difficulty) {
      _textController.text += char;
    }
  }

  Widget _inputBar() {
    return ConstrainedBox(
      constraints: const BoxConstraints(maxWidth: 400),
      child: GuessInput(controller: _textController, onSubmit: _onSubmit),
    );
  }

  List<Widget> _children() {
    final List<Widget> children = [
      Column(
        children: [
          Difficulty(gContext.difficulty, onClick: _onDifficultyChange),
          SizedBox(height: 8),
          Grid(_game.guesses),
          SizedBox(height: 8),
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
