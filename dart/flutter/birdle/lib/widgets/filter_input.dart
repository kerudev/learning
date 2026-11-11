import 'package:flutter/material.dart';

import 'package:birdle/utils/context.dart';

class FilterInput extends StatefulWidget {
  const FilterInput({
    super.key,
    required this.controller,
    required this.onChanged,
  });

  final TextEditingController controller;
  final void Function() onChanged;

  @override
  State<FilterInput> createState() => _FilterInputState();
}

class _FilterInputState extends State<FilterInput> {
  final FocusNode _focusNode = FocusNode();

  @override
  void dispose() {
    widget.controller.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  void _onChanged() {
    widget.onChanged();
    _focusNode.requestFocus();
  }

  Widget _input() {
    return Expanded(
      child: Padding(
        padding: const EdgeInsets.all(8.0),
        child: TextField(
          maxLength: gContext.difficulty,
          focusNode: _focusNode,
          autofocus: true,
          decoration: InputDecoration(
            filled: true,
            fillColor: Colors.white,
            border: OutlineInputBorder(
              borderRadius: BorderRadius.all(Radius.circular(36)),
            ),
          ),
          controller: widget.controller,
          onChanged: (input) {
            _onChanged();
          },
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(children: [_input()]);
  }
}
