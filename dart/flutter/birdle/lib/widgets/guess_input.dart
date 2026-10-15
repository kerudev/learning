import 'package:flutter/material.dart';

class GuessInput extends StatefulWidget {
  const GuessInput({super.key, required this.onSubmitGuess});

  final void Function(String) onSubmitGuess;

  @override
  State<GuessInput> createState() => _GuessInputState();
}

class _GuessInputState extends State<GuessInput> {
  final TextEditingController _textEditingController = TextEditingController();
  final FocusNode _focusNode = FocusNode();

  @override
  void dispose() {
    _textEditingController.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  void _onSubmit() {
    widget.onSubmitGuess(_textEditingController.text.trim());
    _textEditingController.clear();
    _focusNode.requestFocus();
  }

  Widget _input() {
    return Expanded(
      child: Padding(
        padding: const EdgeInsets.all(8.0),
        child: TextField(
          maxLength: 5,
          focusNode: _focusNode,
          autofocus: true,
          decoration: InputDecoration(
            filled: true,
            fillColor: Colors.white,
            suffixIcon: _submitButton(),
            border: OutlineInputBorder(
              borderRadius: BorderRadius.all(Radius.circular(35)),
            ),
          ),
          controller: _textEditingController,
          onSubmitted: (input) {
            _onSubmit();
          },
        ),
      ),
    );
  }

  Widget _submitButton() {
    return IconButton(
      icon: const Icon(Icons.arrow_circle_up),
      onPressed: _onSubmit,
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(children: [_input()]);
  }
}
