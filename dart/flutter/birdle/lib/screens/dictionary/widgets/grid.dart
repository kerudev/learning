import 'package:flutter/material.dart';

import 'package:birdle/shared/utils/context.dart';

class Grid extends StatefulWidget {
  const Grid({super.key});

  @override
  State<Grid> createState() => _GridState();
}

class _GridState extends State<Grid> {
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

  Widget _sliverGrid(List<Widget> items) {
    return SliverGrid.builder(
      itemCount: items.length,
      itemBuilder: (context, index) => items[index],
      gridDelegate: SliverGridDelegateWithMaxCrossAxisExtent(
        maxCrossAxisExtent: 20.0 * gContext.difficulty,
        mainAxisExtent: 100,
        crossAxisSpacing: 8,
        mainAxisSpacing: 8,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final items = gContext.legalWords.where(_hasFilter).map(_asCell).toList();

    return Expanded(
      child: Center(child: CustomScrollView(slivers: [_sliverGrid(items)])),
    );
  }
}
