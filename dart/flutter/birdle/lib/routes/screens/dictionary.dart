import 'package:flutter/material.dart';

import 'package:birdle/utils/context.dart';

class DictionaryScreen extends StatefulWidget {
  const DictionaryScreen({super.key});

  @override
  State<DictionaryScreen> createState() => _DictionaryScreenState();
}

class _DictionaryScreenState extends State<DictionaryScreen> {
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

  @override
  Widget build(BuildContext context) {
    final items = gContext.legalWords.map(_asCell).toList();

    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Center(
        child: CustomScrollView(
          slivers: [
            SliverGrid.builder(
              itemCount: items.length,
              gridDelegate: const SliverGridDelegateWithMaxCrossAxisExtent(
                maxCrossAxisExtent: 100,
                mainAxisExtent: 100,
                crossAxisSpacing: 8,
                mainAxisSpacing: 8,
              ),
              itemBuilder: (context, index) => items[index],
            ),
          ],
        ),
      ),
    );
  }
}
