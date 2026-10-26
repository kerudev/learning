import 'package:flutter/material.dart';

class GuessInput extends StatefulWidget {
  const GuessInput({
    super.key,
    required this.controller,
    required this.onSubmitGuess,
  });

  final TextEditingController controller;
  final void Function(String) onSubmitGuess;

  @override
  State<GuessInput> createState() => _GuessInputState();
}

class _GuessInputState extends State<GuessInput> {
  final FocusNode _focusNode = FocusNode();

  @override
  void dispose() {
    widget.controller.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  void _onSubmit() {
    widget.onSubmitGuess(widget.controller.text.trim());
    widget.controller.clear();
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
          controller: widget.controller,
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
