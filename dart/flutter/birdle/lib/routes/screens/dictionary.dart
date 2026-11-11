import 'package:birdle/widgets/filter_input.dart';
import 'package:flutter/material.dart';

import 'package:birdle/utils/context.dart';

class DictionaryScreen extends StatefulWidget {
  const DictionaryScreen({super.key});

  @override
  State<DictionaryScreen> createState() => _DictionaryScreenState();
}

class _DictionaryScreenState extends State<DictionaryScreen> {
  final TextEditingController _textController = TextEditingController();

  bool _hasFilter(String s) => (_textController.text.isEmpty)
      ? true
      : s.startsWith(_textController.text);

  Widget _asCell(String text) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: ConstrainedBox(
        constraints: BoxConstraints(maxWidth: 80, maxHeight: 80),
        child: Container(
          decoration: BoxDecoration(
            border: Border.all(color: Colors.black),
            borderRadius: BorderRadius.all(Radius.circular(8)),
            color: Colors.white,
          ),
          child: Center(
            child: Text(text, style: TextTheme.of(context).headlineSmall),
          ),
        ),
      ),
    );
  }

  Widget _filter() {
    return FilterInput(controller: _textController, onChanged: () => setState(() {}));
  }

  Widget _sliverGrid(List<Widget> items) {
    final px = 20.0 * gContext.difficulty;

    return SliverGrid.builder(
      itemCount: items.length,
      gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
        maxCrossAxisExtent: px,
        mainAxisExtent: 100,
        crossAxisSpacing: 8,
        mainAxisSpacing: 8,
      ),
      itemBuilder: (context, index) => items[index],
    );
  }

  Widget _grid() {
    final items = gContext.legalWords.where(_hasFilter).map(_asCell).toList();

    return Expanded(
      child: Center(child: CustomScrollView(slivers: [_sliverGrid(items)])),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Column(children: [_filter(), _grid()]);
  }
}
