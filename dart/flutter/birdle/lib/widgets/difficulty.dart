import 'package:flutter/material.dart';

import 'package:birdle/widgets/tile.dart';
import 'package:birdle/utils/game.dart';

class Difficulty extends StatefulWidget {
  Difficulty(this.value, {super.key, required this.onClick});

  final void Function(int d) onClick;

  final List<int> levels = [for (var i = 4; i <= 7; i++) i];
  final int value;

  @override
  State<Difficulty> createState() => _DifficultyState();
}

class _DifficultyState extends State<Difficulty> {
  Tile _asTile(int key) {
    final k = key.toString();
    final hit = widget.value == key ? HitType.partial : HitType.none;

    return Tile(
      k,
      hit.asColor(),
      animate: false,
      onClick: () => (widget.onClick(key)),
    );
  }

  Widget _newCell(int k) {
    return Padding(
      padding: const EdgeInsets.all(2.5),
      child: _asTile(k) as Widget,
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: widget.levels.map(_newCell).toList(),
    );
  }
}
