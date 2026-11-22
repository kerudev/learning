import 'package:flutter/material.dart';

import 'package:birdle/screens/dictionary/widgets/grid.dart';
import 'package:birdle/screens/dictionary/widgets/filter_input.dart';

class DictionaryScreen extends StatefulWidget {
  const DictionaryScreen({super.key});

  @override
  State<DictionaryScreen> createState() => _DictionaryScreenState();
}

class _DictionaryScreenState extends State<DictionaryScreen> {
  final TextEditingController _textController = TextEditingController();

  Widget _filter() {
    return FilterInput(
      controller: _textController,
      onChanged: () => setState(() {}),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Column(children: [_filter(), Grid()]);
  }
}
