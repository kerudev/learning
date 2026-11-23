import 'package:flutter/material.dart';

import 'package:birdle/shared/utils/context.dart';

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
  late bool _isButtonDisabled = false;

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

  void _fetch() async {
    _isButtonDisabled = true;
    setState(() {});

    await gContext.writeWords(gContext.difficulty);
    gContext.setWords();

    _isButtonDisabled = false;
    setState(() {
      widget.onChanged();
    });
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
            hintText: "Type to filter",
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

  Widget _loading() {
    return SizedBox(
      width: 24,
      height: 24,
      child: CircularProgressIndicator(
        strokeWidth: 2,
        color: Colors.black,
        // strokeCap: StrokeCap.round,
      ),
    );
  }

  Widget _icon() {
    return Icon(
      Icons.add_circle_outline_rounded,
      color: _isButtonDisabled ? Colors.black : Colors.lightBlue,
    );
  }

  Widget _addRandomButton() {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SizedBox(
        height: 48,
        child: FloatingActionButton.extended(
          icon: _isButtonDisabled ? _loading() : _icon(),
          label: Text(
            "Random",
            style: TextStyle(
              color: _isButtonDisabled ? Colors.black : Colors.lightBlue,
              fontWeight: FontWeight.bold,
            ),
          ),
          backgroundColor: _isButtonDisabled
              ? Colors.grey.shade300
              : Colors.white,
          onPressed: _isButtonDisabled ? null : _fetch,
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [_input(), _addRandomButton()],
    );
  }
}
